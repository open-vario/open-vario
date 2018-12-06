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

#ifndef WINDOWSCPU_H
#define WINDOWSCPU_H

#include "ICpu.h"

namespace open_vario
{



/** \brief Windows CPU */
class WindowsCpu : public ICpu
{
    public:

        /** \brief Constructor */
        WindowsCpu();

        /** \brief Get the CPU's name */
        virtual const char* name() const { return "Windows"; }

        /** \brief Get the CPU's frequency in Hz */
        virtual uint32_t getFrequency() const { return 1000000000u; }

        /** \brief Get the CPU's RAM size in bytes */
        virtual uint32_t ramSize() const { return 0xFFFFFFFFu; }

        /** \brief Get the CPU's ROM size in bytes */
        virtual uint32_t romSize() const { return 0xFFFFFFFFu; }


        /** \brief Reset the CPU */
        virtual bool reset();

};

}

#endif // WINDOWSCPU_H
