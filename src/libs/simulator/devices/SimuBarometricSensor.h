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

#ifndef SIMUBAROMETRICSENSOR_H
#define SIMUBAROMETRICSENSOR_H

#include "SimuDevice.h"
#include "IBarometricSensor.h"
#include "SimuTemperatureSensor.h"


namespace open_vario
{


/** \brief Simulated barometric sensor */
class SimuBarometricSensor : public SimuDevice, public IBarometricSensor
{
    public:

        /** \brief Constructor */
        SimuBarometricSensor(ISimulator& simulator, const std::string& name, SimuTemperatureSensor& temperature_sensor);

        /** \brief Get the type of the device */
        static const std::string type() { return "baro_sensor"; }


        /** \brief Configure the barometric sensor */
        virtual bool configure();

        /** \brief Read the temperature (1 = 0.1°C) */
        virtual bool readTemperature(int16_t& temperature) { return m_temperature_sensor.readTemperature(temperature); }

        /** \brief Read the pressure (1 = 0.01mbar) */
        virtual bool readPressure(uint32_t& pressure);


        /** \brief Set the pressure (1 = 0.01mbar) */
        void setPressure(const uint32_t pressure) {  m_pressure = pressure; }


    private:

        /** \brief Associated simulated temperature sensor */
        SimuTemperatureSensor& m_temperature_sensor;

        /** \brief Current pressure */
        uint32_t m_pressure;
};

}

#endif // SIMUBAROMETRICSENSOR_H
