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

#ifndef OS_H
#define OS_H

#include "IOs.h"


namespace open_vario
{


/** \brief OS implementation for Windows */
class Os : public IOs
{
    public:

        /** \brief Constructor */
        Os();


        /** \brief Get the OS's name */
        virtual const char* name() const { return "Windows"; };

        /** \brief Initialize the operating system */
        virtual bool init();

        /** \brief Start the operating system (this method shall never return in case of success) */
        virtual bool start();

        /** \brief Get a millisecond timestamp */
        virtual uint32_t getMsTimestamp();

        /** \brief Put the current task into sleep for a specified time duration in milliseconds */
        virtual void waitMs(const uint32_t timeout);

        /** \brief Get the timeout value which corresponds to an infinite timeout */
        virtual uint32_t getInfiniteTimeoutValue() const { return 0xFFFFFFFFu; }


        /** \brief Get the unique instance of the operating system */
        static Os& getInstance() { return (*m_singleton); }


    private:

        /** \brief Singleton */
        static Os* m_singleton;
        
};

}

#endif // IOS_H
