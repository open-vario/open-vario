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

#ifndef IOS_H
#define IOS_H

#include <cstdint>

namespace open_vario
{


/** \brief Interface for all OS implementations */
class IOs
{
    public:

        /** \brief Get the OS's name */
        virtual const char* name() const = 0;

        /** \brief Initialize the operating system */
        virtual bool init() = 0;

        /** \brief Start the operating system (this method shall never return in case of success) */
        virtual bool start() = 0;

        /** \brief Get a millisecond timestamp */
        virtual uint32_t getMsTimestamp() = 0;

        /** \brief Put the current task into sleep for a specified time duration in milliseconds */
        virtual void waitMs(const uint32_t timeout) = 0;
};


}

#endif // IOS_H
