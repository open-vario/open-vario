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

#include "SimuBarometricSensor.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuBarometricSensor::SimuBarometricSensor(ISimulator& simulator, const std::string& name, SimuTemperatureSensor& temperature_sensor)
: SimuDevice(simulator, type(), name)
, m_temperature_sensor(temperature_sensor)
, m_pressure(101300u)
{}

/** \brief Configure the barometric sensor */
bool SimuBarometricSensor::configure()
{
    // Nothing to do
    return true;
}

/** \brief Read the pressure (1 = 0.01mbar) */
bool SimuBarometricSensor::readPressure(uint32_t& pressure)
{
    pressure = m_pressure;
    return true;
}


}
