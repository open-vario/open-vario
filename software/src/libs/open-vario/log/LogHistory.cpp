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

#include "LogHistory.h"


namespace open_vario
{


/** \brief Constructor */
LogHistory::LogHistory(ILogger& logger)
: m_logs()
{
    logger.registerListener(*this);
}


////// Implementation of ILogListener interface //////


/** \brief Called when a new log is available */
void LogHistory::onNewLog(const Log& log)
{
    m_logs.write(log);
}


////// Implementation of LogHistory methods //////


/** \brief Get the number of logs in the log history */
uint32_t LogHistory::getLogCount() const
{
    return static_cast<uint32_t>(m_logs.getCount());
}

/** \brief Clear the log history */
void LogHistory::clearLogs()
{
    m_logs.clear();
}

/** \brief Read a log from the log history */
bool LogHistory::readLog(Log& log)
{
    return m_logs.read(log);
}


}
