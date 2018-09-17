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

#ifndef ILOGHISTORY_H
#define ILOGHISTORY_H


#include "Log.h"

namespace open_vario
{


/** \brief Interface for log history implementation */
class ILogHistory
{
    public:


        /** \brief Get the number of logs in the log history */
        virtual uint32_t getLogCount() const = 0;

        /** \brief Clear the log history */
        virtual void clearLogs() = 0;

        /** \brief Read a log from the log history */
        virtual bool readLog(Log& log) = 0; 

};

}

#endif // ILOGHISTORY_H
