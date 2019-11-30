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

#include "SimuUsbDeviceCdc.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuUsbDeviceCdc::SimuUsbDeviceCdc(ISimulator& simulator, const std::string& name)
: SimuDevice(simulator, type(), name)
, m_listener(nullptr)
, m_running(false)
{}


/** \brief Configure the USB CDC device */
bool SimuUsbDeviceCdc::configure()
{
    return true;
}

/** \brief Start the USB CDC device */
bool SimuUsbDeviceCdc::start()
{
    m_running = true;
    return true;
}

/** \brief Stop the USB CDC device */
bool SimuUsbDeviceCdc::stop()
{
    m_running = false;
    return false;
}

/** \brief Write data through the USB CDC device */
bool SimuUsbDeviceCdc::write(const uint8_t* const data, const uint32_t size)
{
    (void)data;
    (void)size;
    return true;
}

/** \brief Read data from the USB CDC device */
bool SimuUsbDeviceCdc::read(uint8_t* const data, const uint32_t size, uint32_t& read_size, const uint32_t timeout)
{
    (void)data;
    (void)size;
    (void)read_size;
    (void)timeout;
    return true;
}

/** \brief Register a listener */
bool SimuUsbDeviceCdc::registerListener(IUsbDeviceCdcListener& listener)
{
    m_listener = &listener;
    return true;
}


}
