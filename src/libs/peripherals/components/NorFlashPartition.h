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

#ifndef NORFLASHPARTITION_H
#define NORFLASHPARTITION_H

#include "INorFlash.h"

namespace open_vario
{



/** \brief NOR flash partition */
class NorFlashPartition
{
    public:

        /** \brief Constructor */
        NorFlashPartition(INorFlash& nor_flash, const uint32_t first_sector, const uint32_t sector_count);


        /** \brief Get the size of the NOR flash partition in bytes */
        virtual uint32_t getSize() const { return m_size; }

        /** \brief Get the size of a sector of the NOR flash in bytes */
        virtual uint32_t getSectorSize() const { return m_nor_flash.getSectorSize(); }

        /** \brief Configure the NOR flash partition */
        virtual bool configure();

        /** \brief Read data from the NOR flash partition */
        virtual bool read(const uint32_t address, uint8_t data[], const uint32_t size);

        /** \brief Write data to the NOR flash partition */
        virtual bool write(const uint32_t address, const uint8_t data[], const uint32_t size);

        /** \brief Erase a sector of the NOR flash partition */
        virtual bool sectorErase(const uint32_t address);

        /** \brief Erase the whole the NOR flash partition */
        virtual bool fullErase();


    private:

        /** \brief NOR flash */
        INorFlash& m_nor_flash;

        /** \brief First sector to use on the NOR flash */
        const uint32_t m_first_sector;
        
        /** \brief Number of sectors to use on the NOR flash */
        const uint32_t m_sector_count;

        /** \brief Size of the partition in bytes */
        const uint32_t m_size;

        /** \brief Start address of the partition in the NOR flash */
        const uint32_t m_start_address;
};

}

#endif // NORFLASHPARTITION_H
