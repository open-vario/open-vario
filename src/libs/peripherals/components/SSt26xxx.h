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

#ifndef SST26XXX_H
#define SST26XXX_H

#include "INorFlash.h"

namespace open_vario
{

class ISpi;


/** \brief Base class for SST26 NOR flash implementations */
class SSt26xxx : public INorFlash
{
    public:

        /** \brief Constructor */
        SSt26xxx(ISpi& spi, const uint8_t chip_select, const uint32_t size, const uint32_t sector_size, const uint32_t page_size);


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

        /** \brief Read the NOR flash ID */
        virtual bool readId(uint8_t& manufacturer_id, uint8_t& device_type, uint8_t& device_id);


    private:

        /** \brief SPI bus */
        ISpi& m_spi;

        /** \brief Chip select */
        const uint8_t m_chip_select;

        /** \brief NOR flash size in bytes */
        const uint32_t m_size;

        /** \brief Size of a sector of the NOR flash in bytes */
        const uint32_t m_sector_size;

        /** \brief Size of a programming page of the NOR flash in bytes */
        const uint32_t m_page_size;

        /** \brief Number of sectors in the NOR flash */
        const uint32_t m_sector_count;


        /** \brief Send a command to the NOR flash */
        bool sendCommand(const uint8_t cmd);

        /** \brief Send a command with parameters and completion to the NOR flash */
        bool sendCommand(const uint8_t cmd, const uint8_t params[], const uint32_t length);

        /** \brief Wait for the NOR flash to be ready */
        bool waitReady();


        /** \brief Reset enable command */
        static const uint8_t RSTEN = 0x66u;

        /** \brief Reset command */
        static const uint8_t RST = 0x99u;

        /** \brief Read command */
        static const uint8_t READ = 0x03u;

        /** \brief Page program command */
        static const uint8_t PP = 0x02u;

        /** \brief Write enable command */
        static const uint8_t WREN = 0x06u;

        /** \brief Sector erase command */
        static const uint8_t SE = 0x20u;

        /** \brief Chip erase command */
        static const uint8_t CE = 0xC7u;

        /** \brief Global block protection unlock ommand */
        static const uint8_t ULBPR = 0x98u;

        /** \brief JDEC-ID read command */
        static const uint8_t JEDEC_ID = 0x9Fu;

        /** \brief Read status register command */
        static const uint8_t RDSR = 0x05u;

        /** \brief Ready timeout in milliseconds */
        static const uint32_t READY_TIMEOUT = 100u;

};

}

#endif // SST26XXX_H
