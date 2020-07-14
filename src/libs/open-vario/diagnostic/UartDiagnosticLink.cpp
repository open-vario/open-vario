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

#include "UartDiagnosticLink.h"

namespace open_vario
{

/** \brief Constructor */
UartDiagnosticLink::UartDiagnosticLink(IUart& uart)
: m_uart(uart)
, m_listener(NULL)
, m_is_available(false)
{
}

/** \brief Initialize the link */
bool UartDiagnosticLink::init()
{
    return m_uart.configure();
}

/** \brief Write data through the diagnostic link */
bool UartDiagnosticLink::write(const uint8_t* const data, const uint32_t size)
{
    bool ret = false;
    if (m_is_available)
    {
        ret = m_uart.write(data, size);
    }
    return ret;
}

/** \brief Read data from the diagnostic link */
bool UartDiagnosticLink::read(uint8_t* const data, const uint32_t size, const uint32_t timeout)
{
    bool ret = false;
    if (m_is_available)
    {
        ret = m_uart.read(data, size, timeout);
    }
    return ret;
}

/** \brief Enable link */
void UartDiagnosticLink::enableLink()
{
    m_is_available = true;
    if (m_listener != NULL)
    {
        m_listener->onLinkAvailable();
    }
}

/** \brief Disable link */
void UartDiagnosticLink::disableLink()
{
    m_is_available = false;
    if (m_listener != NULL)
    {
        m_listener->onLinkNotAvailable();
    }
}

}
