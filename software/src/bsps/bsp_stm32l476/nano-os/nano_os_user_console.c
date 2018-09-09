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



#include "uart.h"
#include "nano_os.h"
#include "nano_os_user.h"
#include "nano_os_port.h"
#include "nano_os_tools.h"

/* Check if module is enabled */
#if (NANO_OS_CONSOLE_ENABLED == 1u)

/** \brief Initialize the hardware drivers of the user module which will send/receive the console packets */
nano_os_error_t NANO_OS_USER_ConsoleHwInit(void)
{
    nano_os_error_t ret = NOS_ERR_SUCCESS;

    /* Initialize UART */
    UART_Init();

    return ret;
}

/** \brief Initialize the user module which will send/receive the console packets */
nano_os_error_t NANO_OS_USER_ConsoleInit(void)
{
    // Nothing to do
    return NOS_ERR_SUCCESS;
}

/** \brief Wait forever for an incoming char on the console link (this function must be blocking) */
nano_os_error_t NANO_OS_USER_ConsoleReadChar(char* const c)
{
    nano_os_error_t ret = NOS_ERR_SUCCESS;

    /* Wait for a char on the UART */
    UART_Receive(NANO_OS_CAST(uint8_t*, c), 1u);

    return ret;
}

/** \brief Write a string on the console link */
nano_os_error_t NANO_OS_USER_ConsoleWriteString(const char* const string)
{
    nano_os_error_t ret = NOS_ERR_SUCCESS;

    /* Send the string on the UART */
    UART_Send(NANO_OS_CAST(const uint8_t*, string), STRNLEN(string, 255u));

    return ret;
}


#endif /* (NANO_OS_CONSOLE_ENABLED == 1u) */
