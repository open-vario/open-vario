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

#ifndef ISPI_H
#define ISPI_H

#include <cstdint>

namespace open_vario
{

/** \brief Interface for all SPI chip select drivers implementations */
class ISpiChipSelect
{
    public:

        /** \brief Configure the SPI chip select driver */
        virtual bool configure() = 0;

        /** \brief Enable the selected peripheral */
        virtual void enable(const uint8_t peripheral) = 0;

        /** \brief Disable the selected peripheral */
        virtual void disable(const uint8_t peripheral) = 0;
};


/** \brief Interface for all SPI implementations */
class ISpi
{
    public:
    
        /** \brief Polarity */
        enum Polarity
        {
            /** \brief Low (clock idle state is 0) */
            POL_LOW = 0u,
            /** \brief High (clock idle state is 1) */
            POL_HIGH = 1u
        };

        /** \brief Phase */
        enum Phase
        {
            /** \brief Low (data captured on first clock edge) */
            PHA_FIRST = 0u,
            /** \brief High (data captured on second clock edge) */
            PHA_SECOND = 1u
        };

        /** \brief SPI transfer description */
        struct XFer
        {
            /** \brief Buffer to write */
            const uint8_t* write_data;
            /** \brief Buffer for read data */
            uint8_t* read_data;
            /** \brief Buffer size in bytes */
            uint32_t size;
            /** \brief Chip select line */
            uint8_t cs;
            /** \brief Keep chip select line active between transfers */
            bool keep_cs_active;
            /** \brief Next transfer */
            const XFer* next;
        };


        /** \brief Configure the SPI */
        virtual bool configure() = 0;

        /** \brief Transfer data through the SPI */
        virtual bool xfer(const XFer& xfer) = 0;

};

}

#endif // ISPI_H
