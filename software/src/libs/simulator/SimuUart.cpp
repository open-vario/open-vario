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

#include "SimuUart.h"

#include <iostream>

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuUart::SimuUart(const std::string& name)
: SimuDevice(name)
{}

/** \brief Configure the UART */
bool SimuUart::configure()
{
    return true;
}

/** \brief Write data through the UART */
bool SimuUart::write(const uint8_t* const data, const uint32_t size)
{
    cout << "[" << getName() << "] : " << reinterpret_cast<const char*>(data) << endl;  
    return true;
}

/** \brief Read data through the UART */
bool SimuUart::read(uint8_t* const data, const uint32_t size, const uint32_t timeout)
{
    return false;
}

}
