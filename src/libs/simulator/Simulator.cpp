/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Simulator.h"
#include "IOpenVarioApp.h"
#include "OpenVarioConfig.h"
#include "LogMacro.h"

#include "requests.pb.h"
#include "responses.pb.h"
#include "notifications.pb.h"

#include "SimuBarometricSensor.h"
#include "SimuTemperatureSensor.h"


namespace open_vario
{


/** \brief Constructor */
Simulator::Simulator(ConfigManager& config_manager)
: m_config_manager(config_manager)

, m_config_values(OV_CONFIG_GROUP_SIMULATOR, "Simulator")
, m_config_simu_enabled(OV_CONFIG_VALUE_SIMU_ENABLED, "simu_enabled", true, true)
, m_config_simu_ip_address(OV_CONFIG_VALUE_SIMU_IP_ADDRESS, "simu_ip_address", "127.0.0.1", true)
, m_config_simu_port(OV_CONFIG_VALUE_SIMU_PORT, "simu_port", 45678u, true)

, m_socket()
, m_task("Simulator task", 0u)
, m_notification_endpoint()
, m_connected(false)
, m_simulated_devices()

, m_altimeter_evt_handler()
, m_barometer_evt_handler()
, m_thermometer_evt_handler()
, m_variometer_evt_handler()
{}

/** \brief Configure the simulator */
bool Simulator::configure()
{
    bool ret = true;

    // Register configuration values
    ret = ret && m_config_values.registerConfigValue(m_config_simu_enabled);
    ret = ret && m_config_values.registerConfigValue(m_config_simu_ip_address);
    ret = ret && m_config_values.registerConfigValue(m_config_simu_port);
    ret = ret && m_config_manager.registerConfigValueGroup(m_config_values);
    if (ret)
    {
        // Register to altimeter, variometer and accelerometer values
        
        m_altimeter_evt_handler = NANO_STL_EVENT_HANDLER_M(Simulator, onAltimeterValues, const AltimeterValues&);
        ret = ret && IOpenVarioApp::getInstance().getAltimeter().altimeterValuesEvent().bind(m_altimeter_evt_handler);
        
        m_barometer_evt_handler = NANO_STL_EVENT_HANDLER_M(Simulator, onBarometerValues, const BarometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getBarometer().barometerValuesEvent().bind(m_barometer_evt_handler);

        m_thermometer_evt_handler = NANO_STL_EVENT_HANDLER_M(Simulator, onThermometerValues, const ThermometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getThermometer().thermometerValuesEvent().bind(m_thermometer_evt_handler);
        
        m_variometer_evt_handler = NANO_STL_EVENT_HANDLER_M(Simulator, onVariometerValues, const VariometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getVariometer().variometerValuesEvent().bind(m_variometer_evt_handler);
    }

    return ret;
}

/** \brief Start the simulator */
bool Simulator::start()
{
    bool ret = true;

    // Load configuration values
    bool simu_enabled = false;
    ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_SIMULATOR, OV_CONFIG_VALUE_SIMU_ENABLED, simu_enabled);

    char ip_address[IpEndPoint::IP_ADDRESS_MAX_SIZE + 1u] = {};
    ret = ret && m_config_manager.getStringConfigValue(OV_CONFIG_GROUP_SIMULATOR, OV_CONFIG_VALUE_SIMU_IP_ADDRESS, ip_address);

    uint16_t port = 0u;
    ret = ret && m_config_manager.getConfigValue<uint16_t>(OV_CONFIG_GROUP_SIMULATOR, OV_CONFIG_VALUE_SIMU_PORT, port);

    // Check if simu is enabled
    if (ret && simu_enabled)
    {
        // Try to bind the socket
        IpEndPoint simu_endpoint;
        simu_endpoint.port = port;
        NANO_STL_STRNCPY(simu_endpoint.address, ip_address, sizeof(ip_address));

        ret = m_socket.open();
        ret = ret && m_socket.bindAddr(simu_endpoint);
        if(!ret)
        {
            LOG_ERROR("[Simulator] : Unable to bind socket on %s:%u", ip_address, port);
        }
        else
        {
            // Start the simulator task
            ITask::TaskMethod task_method = TASK_METHOD(Simulator, task);
            ret = m_task.start(task_method, nullptr);
            if (!ret)
            {
                LOG_ERROR("[Simulator] : Unable to start the task");
            }
        }
    }

    return ret;
}

/** \brief Register a simulated device */
bool Simulator::registerSimuDevice(ISimuDevice& simu_device)
{
    const bool ret = m_simulated_devices.pushBack(&simu_device);
    return ret;
}

/** \brief Simulator task's method */
void Simulator::task(void* const param)
{
    (void)param;

    IpEndPoint simu_endpoint;
    static uint8_t buffer[4096u];
    size_t received;
    SimuRequest simu_request;
    SimuResponse simu_response;
    bool send_response;

    // Task loop
    while(true)
    {
        // Wait for incoming requests
        received = 0u;
        bool ret = m_socket.receiveFrom(buffer, sizeof(buffer), received, simu_endpoint, IOpenVarioApp::getInstance().getOs().getInfiniteTimeoutValue());
        if (ret)
        {
            // Decode request
            ret = simu_request.ParseFromArray(buffer, received);
            if (ret)
            {
                send_response = true;
                if (m_connected)
                {
                    switch (simu_request.Requests_case())
                    {
                        case SimuRequest::kConnect:
                        {
                            // Connection request, refuse connection since we are already connected
                            ConnectResponse* connect_response = new ConnectResponse();
                            connect_response->set_accept(false);
                            simu_response.set_allocated_connect(connect_response);     
                            break;
                        }

                        case SimuRequest::kDisconnect:
                        {
                            // Disconnection request
                            m_connected = false;

                            // Prepare response
                            DisconnectResponse* disconnect_response = new DisconnectResponse();
                            simu_response.set_allocated_disconnect(disconnect_response);
                            break;
                        } 

                        case SimuRequest::kListSensors:
                        {
                            // Sensor list request
                            ListSensorsResponse* list_sensors_response = new ListSensorsResponse();
                            simu_response.set_allocated_list_sensors(list_sensors_response);
                            for (nano_stl::nano_stl_size_t i = 0; i < m_simulated_devices.getCount(); i++)
                            {
                                bool add_device = true;
                                ListSensorsResponse_SensorType sensor_type = ListSensorsResponse_SensorType_ST_UNKNOWN;
                                ListSensorsResponse_SensorValueType sensor_value_type = ListSensorsResponse_SensorValueType_SVT_UNKNOWN;

                                // Check simulated device type
                                ISimuDevice* simu_device = m_simulated_devices[i];
                                if (simu_device->getType() == SimuBarometricSensor::type())
                                {
                                    sensor_type = ListSensorsResponse_SensorType_ST_PRESSURE;
                                    sensor_value_type = ListSensorsResponse_SensorValueType_SVT_UINT;
                                }
                                else if (simu_device->getType() == SimuTemperatureSensor::type())
                                {
                                    sensor_type = ListSensorsResponse_SensorType_ST_TEMPERATURE;
                                    sensor_value_type = ListSensorsResponse_SensorValueType_SVT_INT;
                                }
                                else
                                {
                                    // Unmanaged simulated device type
                                    add_device = false;
                                }
                                if (add_device)
                                {
                                    // Add device to the list
                                    ListSensorsResponse_Sensor* sensor = list_sensors_response->add_sensors();
                                    sensor->set_id(i);
                                    sensor->set_name(simu_device->getName());
                                    sensor->set_type(sensor_type);
                                    sensor->set_value_type(sensor_value_type);
                                }
                            }
                            break;
                        }

                        case SimuRequest::kUpdateSensor:
                        {
                            // Update sensor request
                            bool success = false;
                            UpdateSensorResponse* update_sensor_response = new UpdateSensorResponse();
                            simu_response.set_allocated_update_sensor(update_sensor_response);

                            // Check sensor id and type
                            const UpdateSensorRequest& req = simu_request.update_sensor();
                            if (req.id() < m_simulated_devices.getCount())
                            {
                                ISimuDevice* simu_device = m_simulated_devices[req.id()];
                                if (simu_device->getType() == SimuBarometricSensor::type())
                                {
                                    SimuBarometricSensor& baro_sensor = *dynamic_cast<SimuBarometricSensor*>(simu_device);
                                    if (req.Values_case() == UpdateSensorRequest::kUintValue)
                                    {
                                        baro_sensor.setPressure(req.uint_value());
                                        success = true;
                                    }
                                }
                                else if (simu_device->getType() == SimuTemperatureSensor::type())
                                {
                                    SimuTemperatureSensor& temp_sensor = *dynamic_cast<SimuTemperatureSensor*>(simu_device);
                                    if (req.Values_case() == UpdateSensorRequest::kIntValue)
                                    {
                                        temp_sensor.setTemperature(req.int_value());
                                        success = true;
                                    }
                                }
                                else
                                {
                                    // Unmanaged simulated device type
                                }
                            }

                            // Overall result
                            update_sensor_response->set_success(success);
                            break;
                        }

                        case SimuRequest::kPing:
                        {
                            // Ping request
                            PingResponse* ping_response = new PingResponse();
                            ping_response->set_number(simu_request.ping().number());
                            simu_response.set_allocated_ping(ping_response);
                            break;
                        }

                        case SimuRequest::kConfigWrite:
                        {
                            // Configuration value write request

                            // Get group, id and data
                            uint32_t uint_value;
                            int32_t int_value;
                            float float_value;
                            double double_value;
                            bool bool_value;
                            const uint16_t group_id = static_cast<uint16_t>(simu_request.config_write().group_id());
                            const uint16_t value_id = static_cast<uint16_t>(simu_request.config_write().value_id());
                            const char* data = nullptr;
                            switch (simu_request.config_write().Values_case())
                            {
                                case ConfigValueWriteRequest::kUintValue:
                                {
                                    uint_value = simu_request.config_write().uint_value();
                                    data = reinterpret_cast<char*>(&uint_value);
                                    break;
                                }
                                case ConfigValueWriteRequest::kIntValue:
                                {
                                    int_value = simu_request.config_write().int_value();
                                    data = reinterpret_cast<char*>(&int_value);
                                    break;
                                }
                                case ConfigValueWriteRequest::kFloatValue:
                                {
                                    float_value = simu_request.config_write().float_value();
                                    data = reinterpret_cast<char*>(&float_value);
                                    break;
                                }
                                case ConfigValueWriteRequest::kDoubleValue:
                                {
                                    double_value = simu_request.config_write().double_value();
                                    data = reinterpret_cast<char*>(&double_value);
                                    break;
                                }
                                case ConfigValueWriteRequest::kStringValue:
                                {
                                    data = simu_request.config_write().string_value().c_str();
                                    break;
                                }
                                case ConfigValueWriteRequest::kBoolValue:
                                {
                                    bool_value = simu_request.config_write().bool_value();
                                    data = reinterpret_cast<char*>(&bool_value);
                                    break;
                                }
                                default:
                                {
                                    // Unknown type
                                    ret = false;
                                    break;
                                }
                            }
                            if (data != nullptr)
                            {
                                // Write configuration value
                                ret = m_config_manager.setArrayConfigValue<char>(group_id, value_id, data);
                            }

                            // Prepare response
                            ConfigValueWriteResponse* config_write_response = new ConfigValueWriteResponse();
                            config_write_response->set_success(ret);
                            simu_response.set_allocated_config_write(config_write_response);
                            break;
                        }

                        case SimuRequest::kConfigRead:
                        {
                            // Configuration value read request
                            ConfigValueReadResponse* config_read_response = new ConfigValueReadResponse();

                            // Get group and id
                            const uint16_t group_id = static_cast<uint16_t>(simu_request.config_read().group_id());
                            const uint16_t value_id = static_cast<uint16_t>(simu_request.config_read().value_id());
                            
                            // Get group
                            IConfigValueGroup* config_group;
                            ret = m_config_manager.getConfigValueGroup(group_id, config_group);
                            if (ret)
                            {
                                // Get config value infos
                                ConfigManager::ConfigValueInfos config_value_infos;
                                ret = m_config_manager.getConfigValueInfos(group_id, value_id, config_value_infos);
                                if (ret)
                                {
                                    // Fill response with informations
                                    config_read_response->set_value_group_name(std::string(config_group->name()));
                                    config_read_response->set_value_name(std::string(config_value_infos.name));
                                    config_read_response->set_value_type(std::string(config_value_infos.type));
                                    config_read_response->set_value_size(config_value_infos.size);
                                    config_read_response->set_has_min_max(config_value_infos.has_min_max);
                                    config_read_response->set_is_reset_only(config_value_infos.is_reset_only);

                                    // Get value
                                    char buffer[512] = { 0 };
                                    ret = m_config_manager.getConfigValue(group_id, value_id, buffer);
                                    if (ret)
                                    {
                                        if (NANO_STL_STRNCMP(config_value_infos.type, "uint", 4u) == 0)
                                        {
                                            config_read_response->set_uint_value(*reinterpret_cast<uint32_t*>(buffer));
                                            if (config_value_infos.has_min_max)
                                            {
                                                config_read_response->set_uint_min_value(*reinterpret_cast<const uint32_t*>(config_value_infos.min_value));
                                                config_read_response->set_uint_max_value(*reinterpret_cast<const uint32_t*>(config_value_infos.max_value));
                                            }
                                        }
                                        else if (NANO_STL_STRNCMP(config_value_infos.type, "int", 3u) == 0)
                                        {
                                            config_read_response->set_int_value(*reinterpret_cast<int32_t*>(buffer));
                                            if (config_value_infos.has_min_max)
                                            {
                                                config_read_response->set_int_min_value(*reinterpret_cast<const int32_t*>(config_value_infos.min_value));
                                                config_read_response->set_int_max_value(*reinterpret_cast<const int32_t*>(config_value_infos.max_value));
                                            }
                                        }
                                        else if (NANO_STL_STRNCMP(config_value_infos.type, "float", 5u) == 0)
                                        {
                                            config_read_response->set_float_value(*reinterpret_cast<float*>(buffer));
                                            if (config_value_infos.has_min_max)
                                            {
                                                config_read_response->set_float_min_value(*reinterpret_cast<const float*>(config_value_infos.min_value));
                                                config_read_response->set_float_max_value(*reinterpret_cast<const float*>(config_value_infos.max_value));
                                            }
                                        }
                                        else if (NANO_STL_STRNCMP(config_value_infos.type, "double", 6u) == 0)
                                        {
                                            config_read_response->set_double_value(*reinterpret_cast<double*>(buffer));
                                            if (config_value_infos.has_min_max)
                                            {
                                                config_read_response->set_double_min_value(*reinterpret_cast<const double*>(config_value_infos.min_value));
                                                config_read_response->set_double_max_value(*reinterpret_cast<const double*>(config_value_infos.max_value));
                                            }
                                        }
                                        else if (NANO_STL_STRNCMP(config_value_infos.type, "string", 6u) == 0)
                                        {
                                            config_read_response->set_string_value(std::string(buffer));
                                            if (config_value_infos.has_min_max)
                                            {
                                                config_read_response->set_uint_min_value(*reinterpret_cast<const uint32_t*>(config_value_infos.min_value));
                                                config_read_response->set_uint_max_value(*reinterpret_cast<const uint32_t*>(config_value_infos.max_value));
                                            }
                                        }
                                        else if (NANO_STL_STRNCMP(config_value_infos.type, "bool", 4u) == 0)
                                        {
                                            config_read_response->set_bool_value(*reinterpret_cast<bool*>(buffer));
                                            if (config_value_infos.has_min_max)
                                            {
                                                config_read_response->set_bool_min_value(*reinterpret_cast<const bool*>(config_value_infos.min_value));
                                                config_read_response->set_bool_max_value(*reinterpret_cast<const bool*>(config_value_infos.max_value));
                                            }
                                        }
                                        else
                                        {
                                            // Unknown data type
                                            ret = false;
                                        }
                                    }
                                }
                            }
       
                            // Finalize response
                            config_read_response->set_success(ret);
                            simu_response.set_allocated_config_read(config_read_response);
                            break;
                        }

                        default:
                        {
                            // Unknown request => ignore request
                            send_response = false;
                            break;
                        }
                    }
                }
                else
                {
                    switch (simu_request.Requests_case())
                    {
                        case SimuRequest::kConnect:
                        {
                            // Connection request, extract notification endpoint
                            if (simu_request.connect().has_notification_endpoint())
                            {
                                const ConnectRequest_NotificationEndpoint& notif_ep = simu_request.connect().notification_endpoint();
                                if (notif_ep.ip_address().empty())
                                {
                                    NANO_STL_STRNCPY(m_notification_endpoint.address, simu_endpoint.address, IpEndPoint::IP_ADDRESS_MAX_SIZE);
                                }
                                else
                                {
                                    NANO_STL_STRNCPY(m_notification_endpoint.address, notif_ep.ip_address().c_str(), IpEndPoint::IP_ADDRESS_MAX_SIZE);
                                }
                                if (notif_ep.port() == 0u)
                                {
                                    m_notification_endpoint.port = simu_endpoint.port;
                                }
                                else
                                {
                                    m_notification_endpoint.port = notif_ep.port();
                                }
                            }
                            else
                            {
                                m_notification_endpoint = simu_endpoint;
                            }

                            // Prepare response
                            ConnectResponse* connect_response = new ConnectResponse();
                            connect_response->set_accept(true);
                            simu_response.set_allocated_connect(connect_response);

                            // Simulator is now connected
                            m_connected = true;
                            break;
                        }

                        default:
                        {
                            // Since we are not connected => ignore request
                            send_response = false;
                            break;
                        }
                    }
                }

                // Send response
                if (send_response)
                {
                    buffer[0u] = RESPONSE_FRAME;
                    simu_response.SerializeToArray(&buffer[1u], sizeof(buffer) - 1u);
                    ret = m_socket.sendTo(buffer, simu_response.ByteSizeLong() + 1u, simu_endpoint);
                    if (!ret && m_connected)
                    {
                        // Unable to send response => disconnect
                        m_connected = false;
                    }
                }
            }
        }
    }
}

/** \brief Called when new altimeter values have been computed */
void Simulator::onAltimeterValues(const AltimeterValues& alti_values)
{
    static uint8_t buffer[4096u];
    static SimuNotification simu_notification;

    // Check if connected
    if (m_connected)
    {
        // Prepare altitude notification
        AltitudeNotification* alti_notification = new AltitudeNotification();
        alti_notification->set_main_altitude(alti_values.main_alti);
        alti_notification->set_altitude_1(alti_values.alti_1);
        alti_notification->set_altitude_2(alti_values.alti_2);
        alti_notification->set_altitude_3(alti_values.alti_3);
        alti_notification->set_altitude_4(alti_values.alti_4);
        alti_notification->set_min_altitude(alti_values.min_alti);
        alti_notification->set_max_altitude(alti_values.max_alti);
        simu_notification.set_allocated_altitude(alti_notification);

        // Send altitude notification
        buffer[0u] = NOTIFICATION_FRAME;
        simu_notification.SerializeToArray(&buffer[1u], sizeof(buffer) - 1u);
        const bool ret = m_socket.sendTo(buffer, simu_notification.ByteSizeLong() + 1u, m_notification_endpoint);
        if (!ret)
        {
            m_connected = false;
        }
    }
}

/** \brief Called when new barometer values have been computed */
void Simulator::onBarometerValues(const BarometerValues& baro_values)
{
    static uint8_t buffer[4096u];
    static SimuNotification simu_notification;

    // Check if connected
    if (m_connected)
    {
        // Prepare pressure notification
        PressureNotification* pressure_notification = new PressureNotification();
        pressure_notification->set_pressure(baro_values.pressure);
        pressure_notification->set_min_pressure(baro_values.min_pressure);
        pressure_notification->set_max_pressure(baro_values.max_pressure);
        simu_notification.set_allocated_pressure(pressure_notification);

        // Send vario notification
        buffer[0u] = NOTIFICATION_FRAME;
        simu_notification.SerializeToArray(&buffer[1u], sizeof(buffer) - 1u);
        const bool ret = m_socket.sendTo(buffer, simu_notification.ByteSizeLong() + 1u, m_notification_endpoint);
        if (!ret)
        {
            m_connected = false;
        }
    }
}

/** \brief Called when new thermometer values have been computed */
void Simulator::onThermometerValues(const ThermometerValues& temp_values)
{
    static uint8_t buffer[4096u];
    static SimuNotification simu_notification;

    // Check if connected
    if (m_connected)
    {
        // Prepare temperature notification
        TemperatureNotification* temp_notification = new TemperatureNotification();
        temp_notification->set_temperature(temp_values.temperature);
        temp_notification->set_min_temperature(temp_values.min_temperature);
        temp_notification->set_max_temperature(temp_values.max_temperature);
        simu_notification.set_allocated_temperature(temp_notification);

        // Send temperature notification
        buffer[0u] = NOTIFICATION_FRAME;
        simu_notification.SerializeToArray(&buffer[1u], sizeof(buffer) - 1u);
        const bool ret = m_socket.sendTo(buffer, simu_notification.ByteSizeLong() + 1u, m_notification_endpoint);
        if (!ret)
        {
            m_connected = false;
        }
    }
}

/** \brief Called when new variometer values have been computed */
void Simulator::onVariometerValues(const VariometerValues& vario_values)
{
    static uint8_t buffer[4096u];
    static SimuNotification simu_notification;

    // Check if connected
    if (m_connected)
    {
        // Prepare vario notification
        VarioNotification* vario_notification = new VarioNotification();
        vario_notification->set_vario(vario_values.vario);
        vario_notification->set_min_vario(vario_values.min_vario);
        vario_notification->set_max_vario(vario_values.max_vario);
        simu_notification.set_allocated_vario(vario_notification);

        // Send vario notification
        buffer[0u] = NOTIFICATION_FRAME;
        simu_notification.SerializeToArray(&buffer[1u], sizeof(buffer) - 1u);
        const bool ret = m_socket.sendTo(buffer, simu_notification.ByteSizeLong() + 1u, m_notification_endpoint);
        if (!ret)
        {
            m_connected = false;
        }
    }
}

}
