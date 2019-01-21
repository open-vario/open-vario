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

#ifndef FLIGHTRECORDER_H
#define FLIGHTRECORDER_H

#include "Timer.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "TaskHelper.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "IFileSystem.h"
#include "IAltimeter.h"
#include "IBarometer.h"
#include "IThermometer.h"
#include "IVariometer.h"
#include "FlightData.h"
#include "StaticRingBuffer.h"


namespace open_vario
{


/** \brief Flight recorder */
class FlightRecorder : public ITimerListener
{
    public:

        /** \brief Constructor */
        FlightRecorder(ConfigManager& config_manager, IFileSystem& file_system);


        /** \brief Initialize the flight recorder */
        bool init();

        /** \brief Start the flight recorder */
        bool start();

        /** \brief Start recording flight data */
        bool startRecording();

        /** \brief Stop recording flight data */
        bool stopRecording();

        /** \brief Get the number of stored flights */
        uint32_t getNumberOfFlights() const;

        /** \brief Open a flight file */
        bool openFlightFile(const uint32_t file_number);

        /** \brief Read flight data from the opened flight file */
        bool readFlightData(FlightData& flight_data);

        /** \brief Close the flight file */
        bool closeFlightFile();

        /** \brief Erase all flight files */
        bool eraseAllFlightFiles();


        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer);

    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief File system */
        IFileSystem& m_file_system;


       /** \brief Configuration values */
        ConfigValueGroup<3u> m_config_values;
        /** \brief Configuration value : recording period of flight data */
        MinMaxConfigValue<uint16_t> m_config_record_period;
        /** \brief Configuration value : flight detector enabled flag */
        ConfigValue<bool> m_config_fligh_detector_enabled;


        /** \brief Record period in milliseconds */
        uint16_t m_record_period;

        /** \brief Recorder task */
        TaskHelper<2048u> m_task;

        /** \brief Timer to start periodic recording of flight data */
        Timer m_record_timer;

        /** \brief Semaphore to trigger the start of periodic recording of flight data */
        Semaphore m_record_trigger_sem;

        /** \brief Indicate if recording has been started */
        bool m_is_recording;

        /** \brief Indicate if the flight detector is enabled */
        bool m_flight_detector_enabled;


        /** \brief Event handler to receive altimeter notifications */
        nano_stl::IEvent<const AltimeterValues&>::EventHandlerM m_altimeter_evt_handler;

        /** \brief Event handler to receive barometer notifications */
        nano_stl::IEvent<const BarometerValues&>::EventHandlerM m_barometer_evt_handler;

        /** \brief Event handler to receive thermometer notifications */
        nano_stl::IEvent<const ThermometerValues&>::EventHandlerM m_thermometer_evt_handler;

        /** \brief Event handler to receive variometer notifications */
        nano_stl::IEvent<const VariometerValues&>::EventHandlerM m_variometer_evt_handler;


        /** \brief Flight data */
        FlightData m_flight_data;

        /** \brief Mutex to protect access to flight data */
        Mutex m_flight_data_mutex;

        /** \brief Ring buffer to store flight data before recording */
        nano_stl::StaticRingBuffer<FlightData, 60u> m_flight_data_buffer;



        /** \brief Flight recorder's task method */
        void task(void* const param);

        /** \brief Called when new altimeter values have been computed */
        void onAltimeterValues(const AltimeterValues& alti_values);

        /** \brief Called when new barometer values have been computed */
        void onBarometerValues(const BarometerValues& baro_values);

        /** \brief Called when new thermometer values have been computed */
        void onThermometerValues(const ThermometerValues& temp_values);

        /** \brief Called when new variometer values have been computed */
        void onVariometerValues(const VariometerValues& vario_values);
};

}

#endif // FLIGHTRECORDER_H
