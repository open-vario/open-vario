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

#include "Log.h"

namespace open_vario
{


/** \brief Log level strings */
const char* const Log::log_level_str[] = { "DISABLED", "DEBUG", "INFO", "ERROR" };


/** \brief Default constructor */
Log::Log()
: timestamp(0u)
, level(LL_DISABLED)
, message("")
{}

/** \brief Initializing constructor */
Log::Log(const uint32_t _timestamp, const Level _level, const char _message[])
: timestamp(_timestamp)
, level(_level)
, message(_message)
{}

/** \brief Copy constructor */
Log::Log(const Log& copy)
: timestamp(copy.timestamp)
, level(copy.level)
, message(copy.message)
{}

/** \brief Copy operator */
Log& Log::operator = (const Log& copy)
{
    timestamp = copy.timestamp;
    level = copy.level;
    message = copy.message;

    return (*this);
}

/** \brief Comparison operator */
bool Log::operator == (const Log& log) const
{
    return ((log.level == level) && 
            (log.timestamp == timestamp) &&
            (log.message == message));
}

/** \brief Get the string corresponding to the log level */
const char* Log::getLevelString() const
{
    const char* ret;
    uint32_t index = static_cast<uint32_t>(level);
    if (index <= LL_ERROR)
    {
        ret = log_level_str[index];
    }
    else
    {
        ret = "INVALID_LEVEL";
    }

    return ret;
}


}
