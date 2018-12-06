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

#ifndef FLIGHTFS_H
#define FLIGHTFS_H

#include <cstdint>

namespace open_vario
{

class NorFlashPartition;


/** \brief Flight file system */
class FlightFs
{
    public:

        /** \brief Constructor */
        FlightFs(NorFlashPartition& nor_flash);

        /** \brief Initialize the flight file system */
        virtual bool init();

        /** \brief Create a new flight file entry */
        virtual bool createFlightFile(uint8_t timestamp[], uint16_t interval, uint8_t data_properties, uint8_t data_size);


    private:

        /** \brief Flight file table header */
        struct FlightFileTableHeader
        {
            /** \brief Start magic */
            uint32_t start_magic;
            /** \brief File table entry count */
            uint32_t file_entry_count;
            /** \brief Free running counter */
            uint32_t counter;
            /** \brief End magic */
            uint32_t end_magic;

            /** \brief Start magic */
            static const uint32_t START_MAGIC = 0x8BADF00Du;
            /** \brief End magic */
            static const uint32_t END_MAGIC = 0xDEADBABEu;
        };

        /** \brief Flight file table entry */
        struct FlightFileTableEntry
        {
            /** \brief Start magic */
            uint8_t start_magic;
            /** \brief File start address */
            uint32_t start_address;
            /** \brief File properties */
            uint16_t properties;
            /** \brief End magic */
            uint8_t end_magic;

        
            /** \brief Start magic */
            static const uint8_t START_MAGIC = 0xAAu;
            /** \brief End magic */
            static const uint8_t END_MAGIC = 0x55u;
            /** \brief In progress flag */
            static const uint16_t IN_PROGRESS = 0xFFFFu;
            /** \brief Ended flag */
            static const uint16_t ENDED = 0xFF00u;
            /** \brief Deleted flag */
            static const uint16_t DELETED = 0x0000u;
        };

        /** \brief Flight file header */
        struct FlightFileHeader
        {
            /** \brief Start magic */
            uint32_t start_magic;
            /** \brief Timestamp */
            uint8_t timestamp[8u];
            /** \brief Data interval in milliseconds */
            uint16_t interval;
            /** \brief Data properties */
            uint8_t data_properties;
            /** \brief Data size in bytes */
            uint8_t data_size;
            /** \brief End magic */
            uint32_t end_magic;

            /** \brief Start magic */
            static const uint32_t START_MAGIC = 0xC0FEF00Du;
            /** \brief End magic */
            static const uint32_t END_MAGIC = 0xBAD00BADu;
        };


        /** \brief Flight file table pointer */
        struct FlightFileTablePointer
        {
            /** \brief Sector */
            uint8_t sector;
            /** \brief Entry */
            uint16_t entry;

            /** \brief Invalid entry */
            static const uint16_t INVALID_ENTRY = 0xFFFFu;
        };


        /** \brief NOR flash partition */
        NorFlashPartition& m_nor_flash;

        /** \brief NOR flash sector size */
        const uint32_t m_sector_size;



        /** \brief Flight file table header */
        FlightFileTableHeader m_file_table_header;

        /** \brief Flight file table sector */
        uint8_t m_file_table_sector;

        /** \brief Number of entries in the file table entry buffer */
        static const uint8_t FILE_TABLE_ENTRY_BUFFER_SIZE = 32u;

        /** \brief File table entry buffer */
        FlightFileTableEntry m_file_table_entries[FILE_TABLE_ENTRY_BUFFER_SIZE];

        /** \brief Flight file count */
        uint32_t m_flight_file_count;

        /** \brief Oldest flight file entry */
        FlightFileTablePointer m_oldest_flight_entry;

        /** \brief Current flight file entry */
        FlightFileTablePointer m_current_flight_entry;


        
        


        /** \brief Read the file table header of a given sector */
        void readFileTableHeader(const uint8_t sector);

        /** \brief Check if the file table header is valid for a given sector */
        bool isFileTableHeaderValid(const uint8_t sector, uint32_t& counter);

        /** \brief Format the memory */
        bool formatMemory();

        /** \brief Look for the current file table entry */
        bool parseFileTableEntry(const uint8_t sector, FlightFileTablePointer& oldest, FlightFileTablePointer& newest, uint32_t& flight_file_count);

        
};

}

#endif // FLIGHTFS_H
