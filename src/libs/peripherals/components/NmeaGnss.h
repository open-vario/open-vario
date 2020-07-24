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

#ifndef NMEAGNSS_H
#define NMEAGNSS_H

#include "IGnss.h"
#include "Mutex.h"
#include "ITask.h"

namespace open_vario
{

class IUart;

/** \brief Base class for all global navigation satellite systems implementations using NMEA frames to provide navigation data */
class NmeaGnss : public IGnss
{
    public:

        /** \brief Constructor */
        NmeaGnss(IUart& uart, ITask& rx_task);


        /** \brief Check if the GNSS hardware is present and working */
        virtual bool probe();

        
        /** \brief Read the current navigation data */
        virtual bool readData(NavigationData& nav_data);


    protected:

        /** \brief Get the UART device */
        IUart& uart() { return m_uart; }

        /** \brief Start receiving NMEA frame */
        bool startRx();

        /** \brief Send an NMEA frame */
        bool sendFrame(const char* nmea_frame, const size_t size, const bool compute_cs);

        /** \brief Handle a received NMEA frame (must be implemented in child classes if needed) */
        virtual void handleRxFrame(const char frame_type[], const char* frame_params) {}


        /** \brief Get the next parameter in a NMEA frame */
        const char* getNextFrameParam(const char* frame);

        /** \brief Convert a string representing a N digits unsigned integer to an integer */
        uint32_t convertNDigitsInt(const char number[], const uint8_t digits, const uint8_t radix);

        /** \brief Convert a time parameter of a NMEA frame */
        bool convertTimeParam(const char* time, IRtc::DateTime& date_time);
        
        /** \brief Convert a date parameter of a NMEA frame */
        bool convertDateParam(const char* date, IRtc::DateTime& date_time);

        /** \brief Convert a coordinates parameter of a NMEA frame */
        bool convertCoordinates(const char* coordinates, double& coordinate);

        /** \brief Convert an altitude parameter of a NMEA frame */
        bool convertAltitude(const char* altitude, uint32_t& alti);

        /** \brief Convert a speed parameter of a NMEA frame */
        bool convertSpeed(const char* speed, uint32_t& spd);

        /** \brief Convert a track angle parameter of a NMEA frame */
        bool convertTrackAngle(const char* track_angle, uint16_t& ta);

        /** \brief Convert a dilution of precision parameter of a NMEA frame */
        bool convertDop(const char* dop, double& fdop);
        

    private:


        /** \brief UART to receive NMEA frames */
        IUart& m_uart;

        /** \brief Receive task */
        ITask& m_rx_task;

        /** \brief Mutex for concurrent access to internal data */
        Mutex m_mutex;

        /** \brief Data validity indicator */
        bool m_data_valid;

        /** \brief Current navigation data */
        NavigationData m_nav_data;

        /** \brief Buffer to store received frames */
        char m_frame_buffer[128u];




        /** \brief Decode the received frame */
        void decodeFrame();

        /** \brief Receive task method */
        void receiveTask(void* const param);


        

        enum NmeaFrameDecodingState
        {
            /** \brief Wait start of frame */
            NFDS_WAIT_START = 0,
            /** \brief Wait end of frame '\r' */
            NFDS_WAIT_EOF_CR = 1,
            /** \brief Wait end of frame '\n' */
            NFDS_WAIT_EOF_LF = 2,
            /** \brief Wait end of checksum */
            NFDS_WAIT_CS = 3
        };

        /** \brief Inter-frame timeout in milliseconds */
        static const uint32_t INTER_FRAME_TIMEOUT = 1500u;

        /** \brief Inter char timeout in milliseconds */
        static const uint32_t INTER_CHAR_TIMEOUT = 50u;

};

}

#endif // NMEAGNSS_H
