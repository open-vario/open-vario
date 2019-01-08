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

#include "NanoOsConsole.h"
#include "IOpenVarioApp.h"


namespace open_vario
{


/** \brief Console commands */
const nano_os_console_cmd_desc_t NanoOsConsole::m_console_commands[] = 
                                                            {
                                                                {"ov_version", "Display Open Vario version", &NanoOsConsole::cmdHandler},
                                                                {"ov_logs", "Display Open Vario logs", &NanoOsConsole::cmdHandler}
                                                            };


/** \brief Uart driver to use for Nano-OS console */
static IUart* nano_os_console_uart = nullptr;


/** \brief Constructor */
NanoOsConsole::NanoOsConsole(IUart& console_uart, ILogHistory& log_history)
: m_log_history(log_history)
, m_console_command_group()
, m_console_command_handlers()
{
    nano_os_console_uart = &console_uart;

    // Register handlers
    m_console_command_handlers.pushBack(&NanoOsConsole::versionCmdHandler);
    m_console_command_handlers.pushBack(&NanoOsConsole::logsCmdHandler);
}

/** \brief Initialize the commands */
bool NanoOsConsole::init()
{
    nano_os_error_t ret;

    m_console_command_group.user_data = this;
    m_console_command_group.commands = m_console_commands;
    m_console_command_group.command_count = sizeof(m_console_commands) / sizeof(nano_os_console_cmd_desc_t);
    ret = NANO_OS_CONSOLE_RegisterCommands(&m_console_command_group);

    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Handle the 'ov_version' console command */
void NanoOsConsole::versionCmdHandler(const char* const params)
{
    NANO_OS_USER_ConsoleWriteString(IOpenVarioApp::getInstance().getVersion());
    NANO_OS_USER_ConsoleWriteString("\r\n");
}

/** \brief Handle the 'ov_logs' console command */
void NanoOsConsole::logsCmdHandler(const char* const params)
{
    Log log;
    NANO_OS_USER_ConsoleWriteString("Timestamp  |  Level  |  Message\r\n");
    while (m_log_history.readLog(log))
    {
        char temp[11u];
        ITOA(log.timestamp, temp, 10);
        NANO_OS_USER_ConsoleWriteString(temp);
        NANO_OS_USER_ConsoleWriteString(" - ");
        NANO_OS_USER_ConsoleWriteString(log.getLevelString());
        NANO_OS_USER_ConsoleWriteString(" - ");
        NANO_OS_USER_ConsoleWriteString(log.message.cStr());
        NANO_OS_USER_ConsoleWriteString("\r\n");
    }
}



/** \brief Generic Nano-OS console command handler */
void NanoOsConsole::cmdHandler(void* const user_data, const uint32_t command_id, const char* const params)
{
    // Get object
    NanoOsConsole& nano_os_console = *reinterpret_cast<NanoOsConsole*>(user_data);

    // Handle command
    if (command_id < nano_os_console.m_console_command_handlers.getCount())
    {
        CmdHandler cmd_handler = nano_os_console.m_console_command_handlers[command_id];
        (nano_os_console.*cmd_handler)(params);
    }
}


/////////////////// Nano-OS console module callbacks /////////////////////////

/** \brief Initialize the hardware drivers of the user module which will send/receive the console packets */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleHwInit(void)
{
    // Nothing to do, already initialized in OpenVarioBoard
    return NOS_ERR_SUCCESS;
}

/** \brief Initialize the user module which will send/receive the console packets */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleInit(void)
{
    // Nothing to do
    return NOS_ERR_SUCCESS;
}

/** \brief Wait forever for an incoming char on the console link (this function must be blocking) */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleReadChar(char* const c)
{
    nano_os_error_t ret = NOS_ERR_FAILURE;

    /* Wait for a char on the UART */
    if (nano_os_console_uart->read(reinterpret_cast<uint8_t*>(c), 1u, IOs::getInstance().getInfiniteTimeoutValue()))
    {
        ret = NOS_ERR_SUCCESS;
    }

    return ret;
}

/** \brief Write a string on the console link */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleWriteString(const char* const string)
{
    nano_os_error_t ret = NOS_ERR_FAILURE;

    /* Wait for a char on the UART */
    if (nano_os_console_uart->write(reinterpret_cast<const uint8_t*>(string), STRNLEN(string, 1024u)))
    {
        ret = NOS_ERR_SUCCESS;
    }

    return ret;
}


}
