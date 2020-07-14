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

#ifndef NANOOSCONSOLE_H
#define NANOOSCONSOLE_H

#include "IUart.h"
#include "ILogHistory.h"

#include "nano_os_api.h"
#include "nano-stl.h"

namespace open_vario
{

class UartDiagnosticLink;

/** \brief Open-vario specific commands implementation for Nano-OS console */
class NanoOsConsole
{
    public:

        /** \brief Constructor */
        NanoOsConsole(IUart& console_uart, ILogHistory& log_history, UartDiagnosticLink& diag_link);

        /** \brief Initialize the commands */
        bool init();


    private:


        /** \brief Log history */
        ILogHistory& m_log_history;

        /** \brief Diagnostic link */
        UartDiagnosticLink& m_diag_link;

        /** \brief Console commmands group */
        nano_os_console_cmd_group_desc_t m_console_command_group;

        /** \brief Console commands */
        static const nano_os_console_cmd_desc_t m_console_commands[];

        /** \brief Console command handler prototype */
        typedef void (NanoOsConsole::*CmdHandler)(const char params[]);

        /** \brief Console command handlers */
        nano_stl::StaticVector<CmdHandler, 4u> m_console_command_handlers;


        /** \brief Handle the 'ov_version' console command */
        void versionCmdHandler(const char params[]);

        /** \brief Handle the 'ov_logs' console command */
        void logsCmdHandler(const char params[]);

        /** \brief Handle the 'ov_diag' console command */
        void diagCmdHandler(const char params[]);

        /** \brief Handle the 'ov_reset' console command */
        void resetCmdHandler(const char params[]);


        /** \brief Generic Nano-OS console command handler */
        static void cmdHandler(void* const user_data, const uint32_t command_id, const char params[]);



};

}

#endif // NANOOSCONSOLE_H
