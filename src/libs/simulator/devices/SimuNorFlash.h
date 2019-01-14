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

#ifndef SIMUNORFLASH_H
#define SIMUNORFLASH_H

#include "INorFlash.h"
#include "File.h"

#include <string>

namespace open_vario
{


/** \brief Simulated NOR flash */
class SimuNorFlash : public INorFlash
{
    public:

        /** \brief Contructor */
        SimuNorFlash(const std::string& binfile_path, const uint32_t size, const uint32_t sector_size);


        /** \brief Get the size of the NOR flash in bytes */
        virtual uint32_t getSize() const { return m_size; }

        /** \brief Get the size of a sector of the NOR flash in bytes */
        virtual uint32_t getSectorSize() const { return m_sector_size; }

        /** \brief Get the number of sectors in the NOR flash */
        virtual uint32_t getSectorCount() const { return m_sector_count; }

        /** \brief Configure the NOR flash */
        virtual bool configure();

        /** \brief Read data from the NOR flash */
        virtual bool read(const uint32_t address, void* const data, const uint32_t size);

        /** \brief Write data to the NOR flash */
        virtual bool write(const uint32_t address, const void* const data, const uint32_t size);

        /** \brief Erase a sector of the NOR flash */
        virtual bool sectorErase(const uint32_t address);

        /** \brief Erase a range of sectors of the NOR flash */
        virtual bool sectorErase(const uint32_t first_sector, const uint32_t sector_count);

        /** \brief Erase the whole the NOR flash */
        virtual bool chipErase();


    private:

        /** \brief NOR flash's binary file path */
        const std::string m_binfile_path;

        /** \brief NOR flash's size */
        const uint32_t m_size;

        /** \brief NOR flash's sector size */
        const uint32_t m_sector_size;

        /** \brief NOR flash's numbere of sectors */
        const uint32_t m_sector_count;

        /** \brief NOR flash's binary file */
        File m_file;

        /** \brief Erase buffer */
        uint8_t m_erase_buffer[65536u];
        
};

}

#endif // SIMUNORFLASH_H
