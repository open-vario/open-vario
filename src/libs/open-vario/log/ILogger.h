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

#ifndef ILOGGER_H
#define ILOGGER_H

#include "Log.h"

namespace open_vario
{


/** \brief Interface for objects which want to listen to incoming logs */
class ILogListener
{
    public:

        /** \brief Called when a new log is available */
        virtual void onNewLog(const Log& log) = 0;
};

/** \brief Interface for all loggers implementations */
class ILogger
{
    public:

        /** \brief Set the log level */
        virtual void setLevel(const Log::Level log_level) = 0;

        /** \brief Get the log level */
        virtual Log::Level getLevel() const  = 0;


        /** \brief Log a message */
        virtual void log(const Log::Level log_level, const char message[], ...) = 0;

        /** \brief Add a listener */
        virtual bool registerListener(ILogListener& listener)  = 0;

};

}

#endif // ILOGGER_H
