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

#include "LogManager.h"

#include "ForEach.h"

using namespace nano_stl;

namespace open_vario
{


/** \brief Constructor */
LogManager::LogManager(const Log::Level log_level, IOs& os)
: m_log_level(log_level)
, m_os(os)
, m_listeners()
{}

/** \brief Log a message */
void LogManager::log(const Log::Level log_level, const char* const message, ...)
{
    // Check level
    if (log_level >= m_log_level)
    {
        // Create log
        va_list arg_list;
        va_start(arg_list, message);

        Log log;
        log.timestamp = m_os.getMsTimestamp();
        log.level = log_level;
        log.message.format(message, arg_list);

        va_end(arg_list);

        // Dispatch log
        foreach(listener, m_listeners, ILogListener*)
        {
            listener->onNewLog(log);
        }
    }
}


}
