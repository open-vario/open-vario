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

#ifndef IUART_H
#define IUART_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all UART implementations */
class IUart
{
    public:
    
        /** \brief Parity */
        enum Parity
        {
            /** \brief No parity */
            PARITY_NONE = 0u,
            /** \brief Parity even */
            PARITY_EVEN = 1u,
            /** \brief Parity odd */
            PARITY_ODD = 2u
        };

        /** \brief Stop bits */
        enum StopBits
        {
            /** \brief 0.5 stop bit */
            STOPBITS_HALF = 0u,
            /** \brief 1 stop bit */
            STOPBITS_ONE = 1u,
            /** \brief 1.5 stop bits */
            STOPBITS_ONE_HALF = 2u,
            /** \brief 2 stop bits */
            STOPBITS_TWO = 3u
        };

        /** \brief Flow control */
        enum FlowControl
        {
            /** \brief No flow control */
            FLOWCONTROL_NONE = 0u,
            /** \brief CTS/RTS */
            FLOWCONTROL_CTSRTS = 1u
        };

        /** \brief Configure the UART */
        virtual bool configure() = 0;

        /** \brief Write data through the UART */
        virtual bool write(const uint8_t* const data, const uint32_t size) = 0;

        /** \brief Read data through the UART */
        virtual bool read(uint8_t* const data, const uint32_t size, const uint32_t timeout) = 0;
};

}

#endif // IUART_H
