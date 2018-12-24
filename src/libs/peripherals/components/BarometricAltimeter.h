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

#ifndef BAROMETRICALTIMETER_H
#define BAROMETRICALTIMETER_H

#include "IAltimeterSensor.h"
#include "IBarometricSensor.h"

namespace open_vario
{



/** \brief Barometric altimeter sensor */
class BarometricAltimeter : public IBarometricSensor, public IAltimeterSensor
{
    public:

        /** \brief Constructor */
        BarometricAltimeter(IBarometricSensor& baro_sensor);


        /** \brief Configure the altimeter sensor */
        virtual bool configure();

        /** \brief Read the altitude (1 = 0.1m) */
        virtual bool readAltitude(uint32_t& altitude);

        /** \brief Set a reference altitude (1 = 0.1m) */
        virtual bool setReferenceAltitude(const uint32_t ref_altitude);

        /** \brief Read the pressure (1 = 0.01mbar) */
        virtual bool readPressure(uint32_t& pressure);

        /** \brief Read the temperature (1 = 0.1°C) */
        virtual bool readTemperature(int16_t& temperature);


    private:

        /** \brief Barometric sensor */
        IBarometricSensor& m_baro_sensor;

        /** \brief Indicate the chip has been configured */
        bool m_configured;
            
        /** \brief Reference temperature */
        double m_ref_temp;

        /** \brief Reference pressure */
        double m_ref_pressure;

        /** \brief Reference altitude */
        double m_ref_alti;

        /** \brief Pressure */
        uint32_t m_pressure;

        /** \brief Temperature */
        int16_t m_temperature;


};

}

#endif // BAROMETRICALTIMETER_H
