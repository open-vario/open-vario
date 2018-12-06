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

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "ILogger.h"
#include "IOs.h"

#include "StaticVector.h"

namespace open_vario
{


/** \brief Log manager */
class LogManager : public ILogger
{
    public:

        /** \brief Constructor */
        LogManager(const Log::Level log_level, IOs& os);


        /** \brief Set the log level */
        virtual void setLevel(const Log::Level log_level) { m_log_level = log_level; }

        /** \brief Get the log level */
        virtual Log::Level getLevel() const { return m_log_level; }


        /** \brief Log a message */
        virtual void log(const Log::Level log_level, const char* const message, ...);

        /** \brief Add a listener */
        virtual bool registerListener(ILogListener& listener) { return m_listeners.pushBack(&listener); }


    private:


        /** \brief Log level */
        Log::Level m_log_level;

        /** \brief Operating system */
        IOs& m_os;

        /** \brief Listeners */
        nano_stl::StaticVector<ILogListener*, 5u> m_listeners;

};

}

#endif // LOGMANAGER_H
