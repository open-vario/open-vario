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

#include "UsbCdcDiagnosticLink.h"

namespace open_vario
{

/** \brief Constructor */
UsbCdcDiagnosticLink::UsbCdcDiagnosticLink(IUsbDeviceCdc& usb_cdc)
: m_usb_cdc(usb_cdc)
, m_listener(NULL)
, m_is_available(false)
{
}

/** \brief Initialize the link */
bool UsbCdcDiagnosticLink::init()
{
    return m_usb_cdc.start();
}

/** \brief Write data through the diagnostic link */
bool UsbCdcDiagnosticLink::write(const uint8_t* const data, const uint32_t size)
{
    bool ret = false;
    if (m_is_available)
    {
        ret = m_usb_cdc.write(data, size);
    }
    return ret;
}

/** \brief Read data from the diagnostic link */
bool UsbCdcDiagnosticLink::read(uint8_t* const data, const uint32_t size, const uint32_t timeout)
{
    bool ret = false;
    if (m_is_available)
    {
        uint32_t left = size;
        uint32_t read_size = 0;
        uint8_t* p_data = data;
        do
        {
            ret = m_usb_cdc.read(data, left, read_size, timeout);
            if (ret)
            {
                left -= read_size;
                p_data += read_size;
            }
        }
        while (ret && (left != 0));
    }
    return ret;
}

/** \brief Called when the USB cable has been plugged */
void UsbCdcDiagnosticLink::onUsbPlugged()
{
    m_is_available = true;
    if (m_listener != NULL)
    {
        m_listener->onLinkAvailable();
    }
}

/** \brief Called when the USB cable has been unplugged */
void UsbCdcDiagnosticLink::onUsbUnplugged()
{
    m_is_available = false;
    if (m_listener != NULL)
    {
        m_listener->onLinkNotAvailable();
    }
}

}
