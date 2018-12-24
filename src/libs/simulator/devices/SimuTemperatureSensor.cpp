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

#include "SimuTemperatureSensor.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuTemperatureSensor::SimuTemperatureSensor(ISimulator& simulator, const std::string& name)
: SimuDevice(simulator, type(), name)
, m_temperature(0u)
{}


/** \brief Configure the temperature sensor */
bool SimuTemperatureSensor::configure()
{
    // Nothing to do
    return true;
}

/** \brief Read the temperature (1 = 0.1°C) */
bool SimuTemperatureSensor::readTemperature(int16_t& temperature)
{
    temperature = m_temperature;
    return true;
}


}
