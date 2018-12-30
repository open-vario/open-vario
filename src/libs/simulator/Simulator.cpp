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
        ret = IOpenVarioApp::getInstance().getAltimeter().registerListener(*this);
        ret = ret && IOpenVarioApp::getInstance().getBarometer().registerListener(*this);
        ret = ret && IOpenVarioApp::getInstance().getThermometer().registerListener(*this);
        ret = ret && IOpenVarioApp::getInstance().getVariometer().registerListener(*this); 
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
    ret = ret && m_config_manager.getConfigValue(OV_CONFIG_GROUP_SIMULATOR, OV_CONFIG_VALUE_SIMU_IP_ADDRESS, ip_address);

    uint16_t port = 0u;
    ret = ret && m_config_manager.getConfigValue<uint16_t>(OV_CONFIG_GROUP_SIMULATOR, OV_CONFIG_VALUE_SIMU_PORT, port);

    // Check if simu is enabled
    if (ret && simu_enabled)
    {
        // Try to bind the socket
        IpEndPoint simu_endpoint;
        simu_endpoint.port = port;
        strcpy(simu_endpoint.address, ip_address);

        ret = m_socket.open();
        ret = ret && m_socket.bindAddr(simu_endpoint);
        if(!ret)
        {
            LOG_ERROR("[Simulator] : Unable to bind socket on %s:%u", ip_address, port);
        }
        else
        {
            // Start the simulator task
            ret = m_task.start(*this, NULL);
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

/** \brief Method which will be called at the task's startup */
void Simulator::taskStart(void* const param)
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
                            for (uint32_t i = 0; i < m_simulated_devices.getCount(); i++)
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
                                    strncpy(m_notification_endpoint.address, simu_endpoint.address, IpEndPoint::IP_ADDRESS_MAX_SIZE);
                                }
                                else
                                {
                                    strncpy(m_notification_endpoint.address, notif_ep.ip_address().c_str(), IpEndPoint::IP_ADDRESS_MAX_SIZE);
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
