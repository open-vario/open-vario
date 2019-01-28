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

#ifndef LOGHISTORY_H
#define LOGHISTORY_H


#include "ILogHistory.h"
#include "ILogger.h"

#include "StaticRingBuffer.h"

namespace open_vario
{


/** \brief Log history */
class LogHistory : public ILogHistory, public ILogListener
{
    public:

        /** \brief Constructor */
        LogHistory(ILogger& logger);


        ////// Implementation of ILogListener interface //////


        /** \brief Called when a new log is available */
        virtual void onNewLog(const Log& log);


        ////// Implementation of ILogHistory interface //////


        /** \brief Get the number of logs in the log history */
        virtual uint32_t getLogCount() const;

        /** \brief Clear the log history */
        virtual void clearLogs();

        /** \brief Read a log from the log history */
        virtual bool readLog(Log& log); 


    private:


        /** \brief Log history */
        nano_stl::StaticRingBuffer<Log, 30u> m_logs;

};

}

#endif // LOGHISTORY_H
