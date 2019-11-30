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

#ifndef MS56XXI2C_H
#define MS56XXI2C_H

#include "Ms56xx.h"

namespace open_vario
{

class II2c;


/** \brief MS56XX barometric sensor on I2C bus implementation */
class Ms56xxI2c : public Ms56xx
{
    public:

        /** \brief Constructor */
        Ms56xxI2c(II2c& i2c, const uint8_t address);


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

        /** \brief I2C bus */
        II2c& m_i2c;

        /** \brief I2C address */
        const uint8_t m_address;


        /** \brief Read a converted value */
        bool readConvertedValue(uint8_t cmd, uint32_t& value);
};

}

#endif // MS56XXI2C_H
