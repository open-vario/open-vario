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

#ifndef MS56XX_H
#define MS56XX_H

#include "IBarometricSensor.h"

namespace open_vario
{



/** \brief Base class for MS56XX barometric sensors implementations */
class Ms56xx : public IBarometricSensor
{
    public:

        /** \brief Constructor */
        Ms56xx();


        /** \brief Configure the barometric sensor */
        virtual bool configure();

        /** \brief Read the pressure (1 = 0.01mbar) */
        virtual bool readPressure(uint32_t& pressure);

        /** \brief Read the temperature (1 = 0.1°C) */
        virtual bool readTemperature(int16_t& temperature);


    protected:

        /** \brief Calibration data */
        struct CalibrationData
        {
            /** \brief Reserved */
            uint16_t reserved;
            /** \brief Pressure sensitivity | SENSt1*/
            uint16_t c1;
            /** \brief Pressure offset | OFFt1 */
            uint16_t c2;
            /** \brief Temperature coefficient of pressure sensitivity | TCS */
            uint16_t c3;
            /** \brief Temperature coefficient of pressure offset | TCO */
            uint16_t c4;
            /** \brief Reference temperature | Tref */
            uint16_t c5;
            /** \brief Temperature coefficient of the temperature | TSENSE */
            uint16_t c6;
            /** \brief 4-bit CRC */
            uint16_t crc;
        };


        /** \brief Reset the chip */
        virtual bool reset() = 0;

        /** \brief Read calibration data from the PROM */
        virtual bool readCalibrationData(CalibrationData& calib_data) = 0;

        /** \brief Read digital pressure value */
        virtual bool readD1(uint32_t& d1) = 0;

        /** \brief Read temperature value */
        virtual bool readD2(uint32_t& d2) = 0;



        /** \brief Reset command */
        static const uint8_t RESET_CMD = 0x1Eu;

        /** \brief Convert D1 command */
        static const uint8_t CONVERT_D1 = 0x48u;

        /** \brief Convert D2 command */
        static const uint8_t CONVERT_D2 = 0x58u;

        /** \brief ADC read command */
        static const uint8_t ADC_READ = 0x00u;

        /** \brief PROM read command */
        static const uint8_t PROM_READ = 0xA0u;


    private:

        /** \brief Indicate the chip has been configured */
        bool m_configured;
            
        /** \brief Calibration data */
        CalibrationData m_calib_data;

        /** \brief Pressure */
        uint32_t m_pressure;

        /** \brief Temperature */
        int16_t m_temperature;


        /** \brief Check the 4-bit CRC of the calibration data */
        bool checkPromCrc4();

};

}

#endif // MS56XX_H
