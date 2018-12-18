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

#ifndef ICRC32_H
#define ICRC32_H

#include <cstdint>
#include <cstddef>

namespace open_vario
{

/** \brief Interface for all CRC-32 implementations */
class ICrc32
{
    public:

        /** \brief Configure the CRC-32 driver */
        virtual bool configure() = 0;

        /** \brief Reset the CRC-32 computation */
        virtual void reset() = 0;

        /** \brief Update the CRC-32 computation with a data buffer */
        virtual uint32_t update(const void* buffer, const size_t size) = 0;

        /** \brief Get the current value of the CRC-32 computation */
        virtual uint32_t value() const = 0;
};

}

#endif // ICRC32_H
