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

#include "FlightRecorder.h"
#include "OpenVarioConfig.h"
#include "OpenVarioTasks.h"
#include "IOpenVarioApp.h"
#include "LogMacro.h"


namespace open_vario
{


/** \brief Constructor */
FlightRecorder::FlightRecorder(ConfigManager& config_manager, TimeManager& time_manager, ProfileManager& profile_manager, IFileSystem& file_system)
: m_config_manager(config_manager)
, m_time_manager(time_manager)
, m_profile_manager(profile_manager)
, m_file_system(file_system)

, m_config_values(OV_CONFIG_GROUP_FLIGHT_RECORDER, "Flight recorder")
, m_config_record_period(OV_CONFIG_VALUE_FR_RECORD_PERIOD, "Recording period", 1000u, 250u, 30000u, true)
, m_config_flight_detector_enabled(OV_CONFIG_VALUE_FR_FLIGHT_DETECTOR_EN, "Flight detector enabled", false, true)
, m_config_pressure_recording_enabled(OV_CONFIG_VALUE_FR_PRESSURE_REC_EN, "Pressure recording enabled", false, true)
, m_config_temperature_recording_enabled(OV_CONFIG_VALUE_FR_TEMPERATURE_REC_EN, "Temperature recording enabled", true, true)
, m_config_acceleration_recording_enabled(OV_CONFIG_VALUE_FR_ACCELERATION_REC_EN, "Acceleration recording enabled", true, true)
, m_config_gnss_recording_enabled(OV_CONFIG_VALUE_FR_GNSS_REC_EN, "GNSS recording enabled", true, true)

, m_record_period(0u)
, m_task("Flight recorder task", OV_TASK_PRIO_FLIGHT_RECORDER)
, m_record_timer()
, m_record_trigger_sem(0u, 1u)
, m_is_recording(false)
, m_flight_detector_enabled(false)


, m_pressure_recording_enabled(false)
, m_temperature_recording_enabled(false)
, m_acceleration_recording_enabled(false)
, m_gnss_recording_enabled(false)

, m_altimeter_evt_handler()
, m_barometer_evt_handler()
, m_thermometer_evt_handler()
, m_variometer_evt_handler()
, m_gnss_evt_handler()

, m_flight_data()
, m_flight_data_mutex()
, m_flight_data_buffer()

, m_flight_data_adapter()
{
    // Register configuration values
    m_config_values.registerConfigValue(m_config_record_period);
    m_config_values.registerConfigValue(m_config_flight_detector_enabled);
    m_config_values.registerConfigValue(m_config_pressure_recording_enabled);
    m_config_values.registerConfigValue(m_config_temperature_recording_enabled);
    m_config_values.registerConfigValue(m_config_acceleration_recording_enabled);
    m_config_values.registerConfigValue(m_config_gnss_recording_enabled);
    m_config_manager.registerConfigValueGroup(m_config_values);
}

/** \brief Initialize the flight recorder */
bool FlightRecorder::init()
{
    bool ret;

    LOG_INFO("Initializing flight recorder...");

    // Initialize the file system
    ret = m_file_system.init();
    if (ret)
    {
        LOG_INFO("%u flight(s) stored", m_file_system.getFileCount());

        // Load configuration values
        ret = m_config_manager.getConfigValue<uint16_t>(OV_CONFIG_GROUP_FLIGHT_RECORDER, OV_CONFIG_VALUE_FR_RECORD_PERIOD, m_record_period);
        ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_FLIGHT_RECORDER, OV_CONFIG_VALUE_FR_FLIGHT_DETECTOR_EN, m_flight_detector_enabled);
        ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_FLIGHT_RECORDER, OV_CONFIG_VALUE_FR_PRESSURE_REC_EN, m_pressure_recording_enabled);
        ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_FLIGHT_RECORDER, OV_CONFIG_VALUE_FR_TEMPERATURE_REC_EN, m_temperature_recording_enabled);
        ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_FLIGHT_RECORDER, OV_CONFIG_VALUE_FR_ACCELERATION_REC_EN, m_acceleration_recording_enabled);
        ret = ret && m_config_manager.getConfigValue<bool>(OV_CONFIG_GROUP_FLIGHT_RECORDER, OV_CONFIG_VALUE_FR_GNSS_REC_EN, m_gnss_recording_enabled);
        if (ret)
        {
            // Register to sensor events
            m_altimeter_evt_handler = NANO_STL_EVENT_HANDLER_M(FlightRecorder, onAltimeterValues, const AltimeterValues&);
            ret = ret && IOpenVarioApp::getInstance().getAltimeter().altimeterValuesEvent().bind(m_altimeter_evt_handler);
            
            m_barometer_evt_handler = NANO_STL_EVENT_HANDLER_M(FlightRecorder, onBarometerValues, const BarometerValues&);
            ret = ret && IOpenVarioApp::getInstance().getBarometer().barometerValuesEvent().bind(m_barometer_evt_handler);

            m_thermometer_evt_handler = NANO_STL_EVENT_HANDLER_M(FlightRecorder, onThermometerValues, const ThermometerValues&);
            ret = ret && IOpenVarioApp::getInstance().getThermometer().thermometerValuesEvent().bind(m_thermometer_evt_handler);
            
            m_variometer_evt_handler = NANO_STL_EVENT_HANDLER_M(FlightRecorder, onVariometerValues, const VariometerValues&);
            ret = ret && IOpenVarioApp::getInstance().getVariometer().variometerValuesEvent().bind(m_variometer_evt_handler);

            // Register to GNSS events
            m_gnss_evt_handler = NANO_STL_EVENT_HANDLER_M(FlightRecorder, onGnssValues, const IGnss::NavigationData&);
            ret = ret && IOpenVarioApp::getInstance().getGnssProvider().gnssDataEvent().bind(m_gnss_evt_handler);
        }

        // Start the task
        ITask::TaskMethod task_method = TASK_METHOD(FlightRecorder, task);
        ret = ret && m_task.start(task_method, nullptr);
    }

    return ret;
}

/** \brief Start the flight recorder */
bool FlightRecorder::start()
{
    bool ret;

    LOG_INFO("Starting flight recorder...");

    // Start the record timer
    ret = m_record_timer.start(*this, m_record_period);
    if (ret)
    {
        // If flight detector is disabled, start recording
        if (m_flight_detector_enabled)
        {
            LOG_INFO("Waiting take off to start flight recording...");
        }
        else
        {
            ret = startRecording();
        }
    }

    return ret;
}

/** \brief Start recording flight data */
bool FlightRecorder::startRecording()
{
    bool ret = false;

    if (!m_is_recording)
    {
        LOG_INFO("Starting flight recording...");

        m_is_recording = true;
        m_record_trigger_sem.post(false);

        ret = true;
    }

    return ret;
}

/** \brief Stop recording flight data */
bool FlightRecorder::stopRecording()
{
    bool ret = false;

    if (!m_is_recording)
    {
        LOG_INFO("Stopping flight recording...");

        m_is_recording = false;
        m_record_trigger_sem.post(false);

        ret = true;
    }

    return ret;
}

/** \brief Get the number of stored flights */
uint32_t FlightRecorder::getNumberOfFlights() const
{
    return m_file_system.getFileCount();
}

/** \brief Open a flight file */
bool FlightRecorder::openFlightFile(const uint32_t file_number, FlightFileHeader& file_header)
{
    bool ret;

    // Open flight file
    IFileSystem::FileInfo file_info;
    ret = m_file_system.openFile(file_number, file_info);
    if (ret)
    {
        // Read header
        uint32_t read;
        ret = m_file_system.readFromFile(&file_header, sizeof(FlightFileHeader), read);
        if (read == sizeof(file_header))
        {
            // Compute number of flight data acquisitions
            file_header.flight_data_count = (file_info.size - sizeof(FlightFileHeader)) / sizeof(FlightData); 

            // Configure flight data adpater
            m_flight_data_adapter.configure(file_header.pressure_available, file_header.temperature_available, file_header.accel_available, file_header.gnss_available);
        }
        else
        {
            // File too short
            m_file_system.closeReadFile();
            ret = false;
        }
    }

    return ret;
}

/** \brief Read flight data from the opened flight file */
bool FlightRecorder::readFlightData(FlightData& flight_data)
{
    bool ret;
    uint32_t read = 0;
    uint8_t flight_data_buffer[sizeof(FlightData)];

    // Read data from file
    ret = m_file_system.readFromFile(flight_data_buffer, m_flight_data_adapter.size(), read);
    if (ret)
    {
        // Check data size
        ret = (read == sizeof(flight_data));
        if (ret)
        {
            // Deserialize data
            m_flight_data_adapter.deserialize(flight_data, flight_data_buffer);
        }
    }

    return ret;
}

/** \brief Close the flight file */
bool FlightRecorder::closeFlightFile()
{
    return m_file_system.closeReadFile();
}

/** \brief Erase all flight files */
bool FlightRecorder::eraseAllFlightFiles()
{
    bool ret = false;

    // Check if recording is in progress
    if (!m_is_recording)
    {
        ret = m_file_system.format();
    }

    return ret;
}

/** \brief Method which will be called when the timer elapsed */
void FlightRecorder::timerElapsed(ITimer& timer)
{
    (void)timer;
    m_record_trigger_sem.post(false);
}

/** \brief Flight recorder's task method */
void FlightRecorder::task(void* const param)
{
    (void)param;
    bool is_recording = false;

    // Initialize the data adpater
    uint8_t flight_data_buffer[sizeof(FlightData)];
    FlightDataAdapter flight_data_adapter(m_pressure_recording_enabled, m_temperature_recording_enabled, m_acceleration_recording_enabled, m_gnss_recording_enabled);

    // Task loop
    while (true)
    {
        // Wait for record trigger
        if (m_record_trigger_sem.wait(IOpenVarioApp::getInstance().getOs().getInfiniteTimeoutValue()))
        {
            // Get flight data
            m_flight_data_mutex.lock();
            FlightData flight_data(m_flight_data);
            m_flight_data_mutex.unlock();

            // Check if data must be stored to file
            if (m_is_recording)
            {
                // Check if recording is started
                if (!is_recording)
                {
                    // Create a new flight file
                    char temp[32u] = {0};
                    NANO_STL_SNPRINTF(temp, sizeof(temp) - 1u, "Flight #%u", (m_file_system.getFileCount() + 1u));
                    if (m_file_system.createFile(temp))
                    {
                        LOG_INFO("New flight file : %s", temp);

                        // Write header
                        FlightFileHeader file_header = {0};
                        file_header.period = m_record_period;
                        file_header.pressure_available = m_pressure_recording_enabled;
                        file_header.temperature_available = m_temperature_recording_enabled;
                        file_header.accel_available = m_acceleration_recording_enabled;
                        file_header.gnss_available = m_gnss_recording_enabled;
                        file_header.pilot = m_profile_manager.getPilot();
                        file_header.glider = m_profile_manager.getGlider();
                        m_time_manager.getDateTime(file_header.date_time);
                        if (!m_file_system.writeToFile(&file_header, sizeof(FlightFileHeader)))
                        {
                            LOG_ERROR("Unable to write flight file header");
                        }
                    }
                    else
                    {
                        LOG_ERROR("Unable to create new flight file");
                    }

                    // Write flight data buffer into file
                    while (m_flight_data_buffer.read(flight_data))
                    {
                        flight_data_adapter.serialize(flight_data, flight_data_buffer);
                        if (!m_file_system.writeToFile(flight_data_buffer, flight_data_adapter.size()))
                        {
                            LOG_ERROR("Unable to write data into flight file");
                        }
                    }

                    is_recording = true;
                }
                else
                {
                    // Store data into the file
                    flight_data_adapter.serialize(flight_data, flight_data_buffer);
                    if (!m_file_system.writeToFile(flight_data_buffer, flight_data_adapter.size()))
                    {
                        LOG_ERROR("Unable to write data into flight file");
                    }
                }
            }
            else
            {
                // Check if recording is stopped
                if (is_recording)
                {
                    // Close the current flight file
                    if (m_file_system.closeWrittenFile())
                    {
                        LOG_INFO("Flight file closed");
                    }
                    else
                    {
                        LOG_ERROR("Unable to close the flight file");
                    }

                    is_recording = false;
                }
                else
                {
                    // Save data into the circular buffer
                    m_flight_data_buffer.write(flight_data);
                }
            }
        }
    }
}

/** \brief Called when new altimeter values have been computed */
void FlightRecorder::onAltimeterValues(const AltimeterValues& alti_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_flight_data.altitude = alti_values.main_alti;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new barometer values have been computed */
void FlightRecorder::onBarometerValues(const BarometerValues& baro_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_flight_data.pressure = baro_values.pressure;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new thermometer values have been computed */
void FlightRecorder::onThermometerValues(const ThermometerValues& temp_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_flight_data.temperature = temp_values.temperature;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new variometer values have been computed */
void FlightRecorder::onVariometerValues(const VariometerValues& vario_values)
{
    // Save values
    m_flight_data_mutex.lock();
    m_flight_data.vario = vario_values.vario;
    m_flight_data_mutex.unlock();
}

/** \brief Called when new GNSS values have been computed */
void FlightRecorder::onGnssValues(const IGnss::NavigationData& nav_data)
{
    // Save values
    m_flight_data_mutex.lock();
    m_flight_data.latitude = nav_data.latitude;
    m_flight_data.longitude = nav_data.longitude;
    m_flight_data.speed = nav_data.speed;
    m_flight_data.track_angle = nav_data.track_angle;
    m_flight_data_mutex.unlock();
}

}
