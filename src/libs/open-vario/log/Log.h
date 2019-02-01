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

#ifndef LOG_H
#define LOG_H

#include "StaticString.h"

namespace open_vario
{


/** \brief Log */
struct Log
{
    /** \brief Maximum log message size */
    static const uint32_t MAX_LOG_MESSAGE_SIZE = 128u;

    /** \brief Log levels */
    enum Level
    {
        /** \brief Logging is disabled */
        LL_DISABLED = 0u,
        /** \brief Debug */
        LL_DEBUG = 1u,
        /** \brief Info */
        LL_INFO = 2u,
        /** \brief Error */
        LL_ERROR = 3u
    };

    /** \brief Timestamp */
    uint32_t timestamp;

    /** \brief Level */
    Level level;

    /** \brief Message */
    nano_stl::StaticString<MAX_LOG_MESSAGE_SIZE> message;


    /** \brief Default constructor */
    Log();

    /** \brief Initializing constructor */
    Log(const uint32_t timestamp, const Level level, const char message[]);

    /** \brief Copy constructor */
    Log(const Log& copy);

    /** \brief Copy operator */
    Log& operator = (const Log& copy);

    /** \brief Comparison operator */
    bool operator == (const Log& log) const;

    /** \brief Get the string corresponding to the log level */
    const char* getLevelString() const;

    private:

    /** \brief Log level strings */
    static const char* const log_level_str[];
};

}

#endif // LOG_H
