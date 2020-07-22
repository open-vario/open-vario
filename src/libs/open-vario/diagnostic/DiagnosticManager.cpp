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

#include "DiagnosticManager.h"
#include "LogMacro.h"
#include "OperatingMode.h"
#include "OpenVarioConfig.h"
#include "nano-stl-conf.h"
#include "ModeManager.h"
#include "DeviceManager.h"

namespace open_vario
{

/** \brief Diagnostic commands */
enum DiagCmds
{
    /** \brief Read device info */
    DIAG_CMD_READ_DEVICE_INFO = 0x01u,

    /** \brief Handle : Read flight count */
    DIAG_CMD_READ_FLIGHT_COUNT,
    /** \brief Handle : Read flight */
    DIAG_CMD_READ_FLIGHT,
    /** \brief Handle : Read next flight data */
    DIAG_CMD_READ_NEXT_FLIGHT_DATA,
    /** \brief Handle : Erase all flights */
    DIAG_CMD_ERASE_ALL_FLIGHTS,

    /** \brief Handle : Read current flight data */
    DIAG_CMD_READ_CURRENT_FLIGHT_DATA,
    /** \brief Handle : Read current date time */
    DIAG_CMD_READ_CURRENT_DATE_TIME,
    /** \brief Handle : Set reference altitude */
    DIAG_CMD_SET_REFERENCE_ALTITUDE,
    
    /** \­brief Handle : Save configuration */
    DIAG_CMD_SAVE_CONFIGURATION,
    /** \­brief Handle : Reset configuration */
    DIAG_CMD_RESET_CONFIGURATION,

    /** \brief Max command number */
    DIAG_CMD_MAX,

    /** \brief Generic acknowledge */
    DIAG_CMD_ACK = 0x00u,
    /** \brief Generic non acknowledge */
    DIAG_CMD_NACK = 0xFFu
};


/** \brief Constructor */
DiagnosticManager::DiagnosticManager(ConfigManager& config_manager, DeviceManager& device_manager, 
                                     TimeManager& time_manager, ProfileManager& profile_manager, 
                                     ModeManager& mode_manager, FlightRecorder& flight_recorder, 
                                     IDiagnosticLink& diag_link)
: m_config_manager(config_manager)
, m_device_manager(device_manager)
, m_time_manager(time_manager)
, m_profile_manager(profile_manager)
, m_mode_manager(mode_manager)
, m_flight_recorder(flight_recorder)
, m_diag_link(diag_link)
, m_diag_link_enabled(false)
, m_diag_link_enabled_sem(0u, 1u)
, m_diag_cmd_task("Diagnostic task", 2u)
, m_rx_cmd(0)
, m_rx_data_size(0)
, m_data_buffer()
, m_resp_serializer(m_data_buffer, sizeof(m_data_buffer))
, m_flight_data()
, m_flight_data_left(0)
, m_flight_data_available(false)
, m_current_flight_data()
{
    NANO_STL_MEMSET(&m_current_flight_data, 0, sizeof(m_current_flight_data));
}


/** \brief Initialize the device manager */
bool DiagnosticManager::init()
{
    bool ret;

    // Initialize diagnostic link
    m_diag_link.registerListener(*this);
    ret = m_diag_link.init();
    if (ret)
    {
        // Register to sensor events
        m_altimeter_evt_handler = NANO_STL_EVENT_HANDLER_M(DiagnosticManager, onAltimeterValues, const AltimeterValues&);
        ret = ret && IOpenVarioApp::getInstance().getAltimeter().altimeterValuesEvent().bind(m_altimeter_evt_handler);
        
        m_barometer_evt_handler = NANO_STL_EVENT_HANDLER_M(DiagnosticManager, onBarometerValues, const BarometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getBarometer().barometerValuesEvent().bind(m_barometer_evt_handler);

        m_thermometer_evt_handler = NANO_STL_EVENT_HANDLER_M(DiagnosticManager, onThermometerValues, const ThermometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getThermometer().thermometerValuesEvent().bind(m_thermometer_evt_handler);
        
        m_variometer_evt_handler = NANO_STL_EVENT_HANDLER_M(DiagnosticManager, onVariometerValues, const VariometerValues&);
        ret = ret && IOpenVarioApp::getInstance().getVariometer().variometerValuesEvent().bind(m_variometer_evt_handler);

        // Register to GNSS events
        m_gnss_evt_handler = NANO_STL_EVENT_HANDLER_M(DiagnosticManager, onGnssValues, const IGnss::NavigationData&);
        ret = ret && IOpenVarioApp::getInstance().getGnssProvider().gnssDataEvent().bind(m_gnss_evt_handler);

        // Start diagnostic command task
        ITask::TaskMethod task_method = TASK_METHOD(DiagnosticManager, diagCmdTask);
        ret = m_diag_cmd_task.start(task_method, NULL);
    }
    
    return ret;
}

/** \brief Called when the link is available */
void DiagnosticManager::onLinkAvailable()
{
    LOG_INFO("Diagnostic link enabled!");
    m_mode_manager.switchToMode(OPMODE_DIAG);
    m_diag_link_enabled_sem.post(false);
}

/** \brief Called when the link is not available */
void DiagnosticManager::onLinkNotAvailable()
{
    LOG_INFO("Diagnostic link disabled!");
    m_mode_manager.switchToMode(OPMODE_RUN);
}

/** \brief Called when new altimeter values have been computed */
void DiagnosticManager::onAltimeterValues(const AltimeterValues& alti_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_current_flight_data.altitude = alti_values.main_alti;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new barometer values have been computed */
void DiagnosticManager::onBarometerValues(const BarometerValues& baro_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_current_flight_data.pressure = baro_values.pressure;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new thermometer values have been computed */
void DiagnosticManager::onThermometerValues(const ThermometerValues& temp_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_current_flight_data.temperature = temp_values.temperature;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new variometer values have been computed */
void DiagnosticManager::onVariometerValues(const VariometerValues& vario_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_current_flight_data.vario = vario_values.vario;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new GNSS values have been computed */
void DiagnosticManager::onGnssValues(const IGnss::NavigationData& nav_data)
{
    // Save values
    m_flight_data_mutex.lock();
    m_current_flight_data.latitude = nav_data.latitude;
    m_current_flight_data.longitude = nav_data.longitude;
    m_current_flight_data.speed = nav_data.speed;
    m_current_flight_data.track_angle = nav_data.track_angle;
    m_flight_data_mutex.unlock();
}

/** \brief Diagnostic command task */
void DiagnosticManager::diagCmdTask(void* unused)
{
    (void)unused;

    // Task loop
    uint32_t data_received = 0;
    RxState rx_state = RXSTATE_WAIT_HEADER_1;
    while (true)
    {
        // Check if link is available
        if (!m_diag_link.isAvailable())
        {
            // Wait for the link to be ready
            m_diag_link_enabled_sem.wait(IOs::getInstance().getInfiniteTimeoutValue());
            rx_state = RXSTATE_WAIT_HEADER_1;
            data_received = 0;
        }

        // Read byte
        uint8_t read_byte = 0;
        bool ret = m_diag_link.read(&read_byte, sizeof(read_byte), CMD_FRAME_TIMEOUT);
        if (ret)
        {
            // Rx state machine
            switch (rx_state)
            {
                case RXSTATE_WAIT_HEADER_1:
                {
                    // Wait header
                    if (read_byte == FIRST_HEADER_BYTE)
                    {
                        rx_state = RXSTATE_WAIT_HEADER_2;
                    }
                    break;
                }

                case RXSTATE_WAIT_HEADER_2:
                {
                    // Wait header
                    if (read_byte == SECOND_HEADER_BYTE)
                    {
                        rx_state = RXSTATE_WAIT_CMD;
                    }
                    else
                    {
                        rx_state = RXSTATE_WAIT_HEADER_1;
                    }
                    break;
                }

                case RXSTATE_WAIT_CMD:
                {
                    // Command
                    m_rx_cmd = read_byte;
                    rx_state = RXSTATE_WAIT_LENGTH_LOW;
                    break;
                }

                case RXSTATE_WAIT_LENGTH_LOW:
                {
                    // Length (lower byte)
                    m_rx_data_size = read_byte;
                    rx_state = RXSTATE_WAIT_LENGTH_HIGH;
                    break;
                }

                case RXSTATE_WAIT_LENGTH_HIGH:
                {
                    // Length (higher byte)
                    m_rx_data_size += static_cast<uint32_t>(read_byte) << 8u;
                    if (m_rx_data_size <= CMD_FRAME_MAX_DATA_SIZE)
                    {
                        if (m_rx_data_size == 0)
                        {
                            // Handle command
                            handleCommand();

                            // Reset state
                            rx_state = RXSTATE_WAIT_HEADER_1;
                        }
                        else
                        {
                            // Wait data
                            data_received = 0;
                            rx_state = RXSTATE_WAIT_DATA;   
                        }
                    }
                    else
                    {
                        rx_state = RXSTATE_WAIT_HEADER_1;
                    }
                    break;
                }

                case RXSTATE_WAIT_DATA:
                {
                    // Wait data
                    m_data_buffer[data_received] = read_byte;
                    data_received++;
                    if (data_received == m_rx_data_size)
                    {
                        // Handle command
                        handleCommand();

                        // Reset state
                        rx_state = RXSTATE_WAIT_HEADER_1;
                    }
                    break;
                }

                default:
                {
                    // Should never happen...
                    rx_state = RXSTATE_WAIT_HEADER_1;
                    break;
                }
            }
        }
        else
        {
            // Timeout => reset state
            rx_state = RXSTATE_WAIT_HEADER_1;
        }
    }
}

/** \brief Handle received commands */
void DiagnosticManager::handleCommand()
{
    /** \brief Command handler table */
    static const DiagCmdHandler diag_cmd_handlers[] = {
                                                        &DiagnosticManager::readDeviceInfo,

                                                        &DiagnosticManager::readFlightCount,
                                                        &DiagnosticManager::readFlight,
                                                        &DiagnosticManager::readNextFlightData,
                                                        &DiagnosticManager::eraseAllFlights,

                                                        &DiagnosticManager::readCurrentFlightData,
                                                        &DiagnosticManager::readCurrentDateTime,
                                                        &DiagnosticManager::setReferenceAltitude,

                                                        &DiagnosticManager::saveConfiguration,
                                                        &DiagnosticManager::resetConfiguration,
                                                      };

    bool ret = false;

    // Check command validity
    if ((m_rx_cmd > 0) && (m_rx_cmd < DIAG_CMD_MAX))
    {
        // Initialize serializer/deserializer
        DataDeserializer deserializer(m_data_buffer, m_rx_data_size);
        m_resp_serializer.clear();

        // Handle command
        DiagCmdHandler handler = diag_cmd_handlers[m_rx_cmd - 1u];
        ret = (this->*handler)(deserializer);
    }
    if (ret)
    {
        // Send response with data in the serializer
        sendResponse(DIAG_CMD_ACK, m_resp_serializer.getBuffer(), m_resp_serializer.getBufferSize());
    }
    else
    {
        // Error
        sendResponse(DIAG_CMD_NACK, NULL, 0);
    }
}

/** \brief Send response */
void DiagnosticManager::sendResponse(const uint8_t cmd, const void* data, const size_t size)
{
    // Header
    uint8_t header[CMD_FRAME_HEADER_SIZE];
    header[0u] = FIRST_HEADER_BYTE;
    header[1u] = SECOND_HEADER_BYTE;
    header[2u] = cmd;
    header[3u] = static_cast<uint8_t>(size & 0xFFu);
    header[4u] = static_cast<uint8_t>((size >> 8u) & 0xFFu);
    m_diag_link.write(header, sizeof(header));

    // Data
    m_diag_link.write(reinterpret_cast<const uint8_t*>(data), size);
}

/** \brief Handle : Read device info */
bool DiagnosticManager::readDeviceInfo(DataDeserializer& deserializer)
{
    IOpenVarioApp& ov_app = IOpenVarioApp::getInstance();
    m_resp_serializer.serialize(ov_app.getVersion());
    m_resp_serializer.serialize(ov_app.getSwManufacturer());

    m_resp_serializer.serialize(m_device_manager.getHwVersion());
    m_resp_serializer.serialize(m_device_manager.getHwManufacturer());
    m_resp_serializer.serialize(m_device_manager.getHwManufacturingDate());
    m_resp_serializer.serialize(m_device_manager.getHwSerialNumber());
    m_resp_serializer.serialize(m_device_manager.getDeviceName());

    return true;
}

/** \brief Handle : Read flight count */
bool DiagnosticManager::readFlightCount(DataDeserializer& deserializer)
{
    m_resp_serializer.serialize(m_flight_recorder.getNumberOfFlights());

    return true;
}

/** \brief Handle : Read flight */
bool DiagnosticManager::readFlight(DataDeserializer& deserializer)
{
    bool ret = false;

    // Close any previously opened flight file
    m_flight_recorder.closeFlightFile();

    // Get flight number
    uint32_t flight_number = 0;
    ret = deserializer.deserialize(flight_number);
    if (ret)
    {
        // Open selected flight
        FlightFileHeader file_header = {0};
        ret = m_flight_recorder.openFlightFile(flight_number, file_header);
        if (ret)
        {
            // Return flight informations
            m_resp_serializer.serialize(file_header.date_time);
            m_resp_serializer.serialize(file_header.glider);
            m_resp_serializer.serialize(file_header.pilot);
            m_resp_serializer.serialize(file_header.period);
            m_resp_serializer.serialize(file_header.flight_data_count);

            // Initialize read
            m_flight_data_left = file_header.flight_data_count;
            m_flight_data_available = false;
        }
    }

    return ret;
}

/** \brief Handle : Read next flight data */
bool DiagnosticManager::readNextFlightData(DataDeserializer& deserializer)
{
    bool ret = true;

    // Serialize available flight data
    if (m_flight_data_available)
    {
        m_resp_serializer.serialize(m_flight_data);
        m_flight_data_available = false;
        m_flight_data_left--;
    }

    while (ret && (m_flight_data_left != 0) && !m_flight_data_available)
    {
        // Read next flight data
        ret = m_flight_recorder.readFlightData(m_flight_data);
        if (ret)
        {
            // Try to serialize flight data
            if (m_resp_serializer.serialize(m_flight_data))
            {
                m_flight_data_left--;
            }
            else
            {
                // Not enough space, flight data will
                // be serialized on the next request
                m_flight_data_available = true;
            }
        }
    }
    if (m_flight_data_left == 0)
    {
        // Close flight file
        m_flight_recorder.closeFlightFile();
    }

    return ret;
}

/** \brief Handle : Erase all flights */
bool DiagnosticManager::eraseAllFlights(DataDeserializer& deserializer)
{
    return m_flight_recorder.eraseAllFlightFiles();
}

/** \brief Handle : Read current flight data */
bool DiagnosticManager::readCurrentFlightData(DataDeserializer& deserializer)
{
    m_flight_data_mutex.lock();
    bool ret = m_resp_serializer.serialize(m_current_flight_data);
    m_flight_data_mutex.unlock();
    return ret;
}

/** \brief Handle : Read current date time */
bool DiagnosticManager::readCurrentDateTime(DataDeserializer& deserializer)
{
    IRtc::DateTime date_time;
    bool ret = m_time_manager.getDateTime(date_time);
    if (ret)
    {
        ret = m_resp_serializer.serialize(date_time);
    }
    return ret;
}

/** \brief Handle : Set reference altitude */
bool DiagnosticManager::setReferenceAltitude(DataDeserializer& deserializer)
{
    bool ret;

    // Get altitude
    int32_t altitude = 0;
    ret = deserializer.deserialize(altitude);
    if (ret)
    {
        ret = IOpenVarioApp::getInstance().getAltimeter().setReferenceAltitude(altitude);
    }

    return ret;
}

/** \brief Handle : Save configuration */
bool DiagnosticManager::saveConfiguration(DataDeserializer& deserializer)
{
    const bool ret = m_config_manager.store();
    return ret;
}

/** \brief Handle : Reset configuration */
bool DiagnosticManager::resetConfiguration(DataDeserializer& deserializer)
{
    m_config_manager.resetToDefault();
    const bool ret = m_config_manager.store();
    return ret;
}

}
