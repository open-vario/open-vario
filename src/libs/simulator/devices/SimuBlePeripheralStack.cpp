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

#include "SimuBlePeripheralStack.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuBlePeripheralStack::SimuBlePeripheralStack(ISimulator& simulator, const std::string& name)
: SimuDevice(simulator, type(), name)
, m_device_config()
, m_ble_services(nullptr)
, m_listener(nullptr)
{}


/** \brief Start the BLE stack */
bool SimuBlePeripheralStack::start()
{
    return true;
}

/** \brief Stop the BLE stack */
bool SimuBlePeripheralStack::stop()
{
    return true;
}


}
