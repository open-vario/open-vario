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

#ifndef DIAGNOSTICMANAGER_H
#define DIAGNOSTICMANAGER_H

#include "IDiagnosticLink.h"
#include "TaskHelper.h"
#include "FlightRecorder.h"
#include "DataSerializer.h"
#include "DataDeserializer.h"
#include "Semaphore.h"

namespace open_vario
{

class ConfigManager;
class DeviceManager;
class TimeManager;
class ProfileManager;
class ModeManager;

/** \brief Diagnostic manager */
class DiagnosticManager : public IDiagnosticLinkListener
{
    public:

        /** \brief Constructor */
        DiagnosticManager(ConfigManager& config_manager, DeviceManager& device_manager, 
                          TimeManager& time_manager, ProfileManager& profile_manager, 
                          ModeManager& mode_manager, FlightRecorder& flight_recorder, 
                          IDiagnosticLink& diag_link);


        /** \brief Initialize the device manager */
        bool init();


        /** \brief Get the state of the diagnostic link */
        bool isLinkEnabled() const { return m_diag_link.isAvailable(); }
        


        /** \brief Called when the link is available */
        virtual void onLinkAvailable() override;

        /** \brief Called when the link is not available */
        virtual void onLinkNotAvailable() override;


    private:

        /** \brief Reception states */
        enum RxState
        {
            /** \brief Wait first header byte */
            RXSTATE_WAIT_HEADER_1 = 0,
            /** \brief Wait second header byte */
            RXSTATE_WAIT_HEADER_2,
            /** \brief Wait command */
            RXSTATE_WAIT_CMD,
            /** \brief Wait length (lower byte) */
            RXSTATE_WAIT_LENGTH_LOW,
            /** \brief Wait length (higher byte) */
            RXSTATE_WAIT_LENGTH_HIGH,
            /** \brief Wait data */
            RXSTATE_WAIT_DATA
        };

        /** \brief Diagnostic command handler */
        typedef bool (DiagnosticManager::*DiagCmdHandler)(DataDeserializer&);


        /** \brief Timeout in milliseconds for command frame reception */
        static const uint32_t CMD_FRAME_TIMEOUT = 100u;

        /** \brief Size in bytes of a command frame header :
         *         - Header bytes : 2
         *         - Command : 1
         *         - Length : 2
         *         => Total : 5
         */
        static const uint32_t CMD_FRAME_HEADER_SIZE = 5u;

        /** \brief Maximum size in bytes of the data part of a command frame */
        static const uint32_t CMD_FRAME_MAX_DATA_SIZE = 2048u;

        /** \brief Maximum size in bytes of a command frame => header + data */
        static const uint32_t CMD_FRAME_MAX_SIZE = CMD_FRAME_HEADER_SIZE + CMD_FRAME_MAX_DATA_SIZE;


        /** \brief First header byte */
        static const uint8_t FIRST_HEADER_BYTE = 0xAAu;
        /** \brief Second header byte */
        static const uint8_t SECOND_HEADER_BYTE = 0x55u;


        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Device manager */
        DeviceManager& m_device_manager;

        /** \brief Time manager */
        TimeManager& m_time_manager;
        
        /** \brief Profile manager */
        ProfileManager& m_profile_manager;

        /** \brief Mode manager */
        ModeManager& m_mode_manager;

        /** \brief Flight recorder */
        FlightRecorder& m_flight_recorder;

        /** \brief Diagnostic link */
        IDiagnosticLink& m_diag_link;


        /** \brief Indicate if the diagnostic link is enable */
        bool m_diag_link_enabled;

        /** \brief Diagnostic link enabled semaphore */
        Semaphore m_diag_link_enabled_sem;

        /** \brief Diagnostic command task */
        TaskHelper<2048u> m_diag_cmd_task;

        /** \brief Rx command */
        uint8_t m_rx_cmd;

        /** \brief Rx data size */
        uint32_t m_rx_data_size;

        /** \brief Data buffer for received commands and responses */
        uint8_t m_data_buffer[CMD_FRAME_MAX_DATA_SIZE];

        /** \brief Response data serializer */
        DataSerializer m_resp_serializer;

        /** \brief Last read flight data */
        FlightData m_flight_data;

        /** \brief Number of flight data left to send */
        uint32_t m_flight_data_left;

        /** \brief Indicate is a flight data is already available */
        bool m_flight_data_available;




        /** \brief Diagnostic command task */
        void diagCmdTask(void* unused);

        /** \brief Handle received commands */
        void handleCommand();

        /** \brief Send response */
        void sendResponse(const uint8_t cmd, const void* data, const size_t size);


        /** \brief Handle : Read device info */
        bool readDeviceInfo(DataDeserializer& deserializer);

        /** \brief Handle : Read flight count */
        bool readFlightCount(DataDeserializer& deserializer);

        /** \brief Handle : Read flight */
        bool readFlight(DataDeserializer& deserializer);

        /** \brief Handle : Read next flight data */
        bool readNextFlightData(DataDeserializer& deserializer);

        /** \brief Handle : Erase all flights */
        bool eraseAllFlights(DataDeserializer& deserializer);
};

}

#endif // DIAGNOSTICMANAGER_H
