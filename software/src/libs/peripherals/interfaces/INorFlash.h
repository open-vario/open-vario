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

#ifndef INORFLASH_H
#define INORFLASH_H

#include <cstdint>

namespace open_vario
{



/** \brief Interface for all NOR flash implementations */
class INorFlash
{
    public:

        /** \brief Get the size of the NOR flash in bytes */
        virtual uint32_t getSize() const = 0;

        /** \brief Get the size of a sector of the NOR flash in bytes */
        virtual uint32_t getSectorSize() const = 0;

        /** \brief Configure the NOR flash */
        virtual bool configure() = 0;

        /** \brief Read data from the NOR flash */
        virtual bool read(const uint32_t address, uint8_t data[], const uint32_t size) = 0;

        /** \brief Write data to the NOR flash */
        virtual bool write(const uint32_t address, const uint8_t data[], const uint32_t size) = 0;

        /** \brief Erase a sector of the NOR flash */
        virtual bool sectorErase(const uint32_t address) = 0;

        /** \brief Erase the whole the NOR flash */
        virtual bool chipErase(const uint32_t address) = 0;
};

}

#endif // INORFLASH_H
