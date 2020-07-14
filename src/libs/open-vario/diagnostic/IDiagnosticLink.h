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

#ifndef IDIAGNOSTICLINK_H
#define IDIAGNOSTICLINK_H

#include "nano-stl.h"

namespace open_vario
{

/** \brief Interface for listeners to diagnostic link */
class IDiagnosticLinkListener
{
    public:

        /** \brief Called when the link is available */
        virtual void onLinkAvailable() = 0;

        /** \brief Called when the link is not available */
        virtual void onLinkNotAvailable() = 0;

};


/** \brief Interface for all diagnostic links implementations */
class IDiagnosticLink
{
    public:


        /** \brief Register a listener */
        virtual void registerListener(IDiagnosticLinkListener& listener) = 0;

        /** \brief Initialize the link */
        virtual bool init() = 0;

        /** \brief Check if the link is available */
        virtual bool isAvailable() = 0;

        /** \brief Write data through the diagnostic link */
        virtual bool write(const uint8_t* const data, const uint32_t size) = 0;

        /** \brief Read data from the diagnostic link */
        virtual bool read(uint8_t* const data, const uint32_t size, const uint32_t timeout) = 0;

};

}

#endif // IDIAGNOSTICLINK_H
