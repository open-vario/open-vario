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

#ifndef SIMUTEMPERATURESENSOR_H
#define SIMUTEMPERATURESENSOR_H

#include "SimuDevice.h"
#include "ITemperatureSensor.h"


namespace open_vario
{


/** \brief Simulated temperature sensor */
class SimuTemperatureSensor : public SimuDevice, public ITemperatureSensor
{
    public:

        /** \brief Constructor */
        SimuTemperatureSensor(ISimulator& simulator, const std::string& name);

        /** \brief Get the type of the device */
        static const std::string type() { return "temp_sensor"; }


        /** \brief Configure the temperature sensor */
        virtual bool configure();

        /** \brief Read the temperature (1 = 0.1°C) */
        virtual bool readTemperature(int16_t& temperature);


        /** \brief Set the temperature (1 = 0.1°C) */
        void setTemperature(const int16_t temperature) {  m_temperature = temperature; }

    private:

        /** \brief Current temperature */
        int16_t m_temperature;
};

}

#endif // SIMUBAROMETRICSENSOR_H
