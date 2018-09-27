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

#ifndef MS56XXSPI_H
#define MS56XXSPI_H

#include "Ms56xx.h"

namespace open_vario
{

class ISpi;


/** \brief MS56XX barometric sensor on SPI bus implementation */
class Ms56xxSpi : public Ms56xx
{
    public:

        /** \brief Constructor */
        Ms56xxSpi(ISpi& spi, const uint8_t chip_select);


    protected:


        /** \brief Reset the chip */
        virtual bool reset();

        /** \brief Read calibration data from the PROM */
        virtual bool readCalibrationData(CalibrationData& calib_data);

        /** \brief Read digital pressure value */
        virtual bool readD1(uint32_t& d1);

        /** \brief Read temperature value */
        virtual bool readD2(uint32_t& d2);


    private:

        /** \brief SPI bus */
        ISpi& m_spi;

        /** \brief Chip select */
        const uint8_t m_chip_select;


        /** \brief Read a converted value */
        bool readConvertedValue(const uint8_t cmd, uint32_t& value);
};

}

#endif // MS56XXSPI_H
