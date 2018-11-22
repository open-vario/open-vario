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

#include "FlightFs.h"
#include "NorFlashPartition.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
FlightFs::FlightFs(NorFlashPartition& nor_flash)
: m_nor_flash(nor_flash)
, m_sector_size(m_nor_flash.getSectorSize())
{}

/** \brief Initialize the flight file system */
bool FlightFs::init()
{
    bool ret = true;

    // Check if the memory has been formated
    uint32_t sector_0_counter = 0u;
    bool sector_0_valid = isFileTableHeaderValid(0u, sector_0_counter);
    uint32_t sector_1_counter = 0u;
    bool sector_1_valid = isFileTableHeaderValid(1u, sector_1_counter);
    if (sector_0_valid && sector_1_valid)
    {
        if (sector_0_counter > sector_1_counter)
        {
            m_file_table_sector = 0u;
        }
        else
        {
            m_file_table_sector = 1u;
        }
    }
    else if (sector_0_valid)
    {
        m_file_table_sector = 0u;
    }
    else if (sector_1_valid)
    {
        m_file_table_sector = 1u;
    }
    else
    {
        // Format memory
        ret = formatMemory();
    }

    // Read the file table header
    readFileTableHeader(m_file_table_sector);
    
    // Get the oldest and newest entries
    uint32_t flight_file_count = 0u;
    FlightFileTablePointer oldest, newest;
    parseFileTableEntry((m_file_table_sector ^ 1u), m_oldest_flight_entry, newest, flight_file_count);
    m_flight_file_count += flight_file_count;
    parseFileTableEntry(m_file_table_sector, oldest, m_current_flight_entry, flight_file_count);
    if (m_oldest_flight_entry.entry == FlightFileTablePointer::INVALID_ENTRY)
    {
        m_oldest_flight_entry = oldest;
    }
    if (m_current_flight_entry.entry == FlightFileTablePointer::INVALID_ENTRY)
    {
        m_current_flight_entry = newest;
    }

    return ret;
}

/** \brief Create a new flight file entry */
bool FlightFs::createFlightFile(uint8_t timestamp[], uint16_t interval, uint8_t data_properties, uint8_t data_size)
{
    bool ret = false;

    // Prepare file table entry
    FlightFileTableEntry flight_entry;
    flight_entry.start_magic = FlightFileTableEntry::START_MAGIC;
    flight_entry.start_address = 0x0000;
    flight_entry.properties = FlightFileTableEntry::IN_PROGRESS;
    flight_entry.end_magic = FlightFileTableEntry::END_MAGIC;

    // Prepare flight file header
    FlightFileHeader flight_header;
    flight_header.start_magic = FlightFileHeader::START_MAGIC;
    //flight_header.timestamp = timestamp;
    flight_header.interval = interval;
    flight_header.data_properties = data_properties;
    flight_header.data_size = data_size;
    flight_header.end_magic = FlightFileHeader::END_MAGIC;

    return ret;
}

/** \brief Read the file table header of a given sector */
void FlightFs::readFileTableHeader(const uint8_t sector)
{
    // Read the file table header
    m_nor_flash.read(sector * m_sector_size, reinterpret_cast<uint8_t*>(&m_file_table_header), sizeof(FlightFileTableHeader));
}

/** \brief Check if the file table header is valid for a given sector */
bool FlightFs::isFileTableHeaderValid(const uint8_t sector, uint32_t& counter)
{
    bool is_valid = false;

    // Read the file table header
    readFileTableHeader(sector);

    // Check validity
    is_valid = ((m_file_table_header.start_magic == FlightFileTableHeader::START_MAGIC) &&
                (m_file_table_header.end_magic == FlightFileTableHeader::END_MAGIC) &&
                (m_file_table_header.file_entry_count == ((m_sector_size - sizeof(FlightFileTableHeader))/sizeof(FlightFileTableEntry))));
    counter = m_file_table_header.counter;

    return is_valid;
}

/** \brief Format the memory */
bool FlightFs::formatMemory()
{
    // Erase all memory
    bool ret = m_nor_flash.fullErase();
    if (ret)
    {
        // Write file table header on first sector
        m_file_table_sector = 0u;
        m_file_table_header.start_magic = FlightFileTableHeader::START_MAGIC;
        m_file_table_header.file_entry_count = ((m_sector_size - sizeof(FlightFileTableHeader))/sizeof(FlightFileTableEntry));
        m_file_table_header.counter = 0u;
        m_file_table_header.end_magic = FlightFileTableHeader::END_MAGIC;
        ret = m_nor_flash.write(0u, reinterpret_cast<uint8_t*>(&m_file_table_header), sizeof(FlightFileTableHeader));
    }

    return ret;
}

/** \brief Parse a file table entry to look for its oldest and newest entries */
bool FlightFs::parseFileTableEntry(const uint8_t sector, FlightFileTablePointer& oldest, FlightFileTablePointer& newest, uint32_t& flight_file_count)
{
    // Compute base address
    uint32_t address = sector * m_sector_size + sizeof(FlightFileTableHeader);

    // Initial pointers
    oldest.sector = sector;
    oldest.entry = FlightFileTablePointer::INVALID_ENTRY;
    newest.sector = sector;
    newest.entry = FlightFileTablePointer::INVALID_ENTRY;

    // Look for the first unused entry
    bool ret;
    bool loop = true;
    bool oldest_found = false;
    flight_file_count = 0;
    uint32_t entry_count = 0;
    do
    {
        // Compute number of entries to read
        uint8_t entries_to_read = FILE_TABLE_ENTRY_BUFFER_SIZE;
        if ((entry_count + FILE_TABLE_ENTRY_BUFFER_SIZE) > m_file_table_header.file_entry_count)
        {
            entries_to_read = m_file_table_header.file_entry_count - entry_count;
        }

        // Read entries
        ret = m_nor_flash.read(address, reinterpret_cast<uint8_t*>(m_file_table_entries), entries_to_read * sizeof(FlightFileTableEntry));
        if (ret)
        {
            // Look for an unused entry
            uint8_t i = 0;
            
            while ((i < entries_to_read) &&
                   (m_file_table_entries[i].START_MAGIC == FlightFileTableEntry::START_MAGIC) &&
                   (m_file_table_entries[i].END_MAGIC == FlightFileTableEntry::END_MAGIC))
            {
                // Check if the entry is valid
                if (m_file_table_entries[i].properties == FlightFileTableEntry::ENDED)
                {
                    flight_file_count++;
                    if (!oldest_found)
                    {
                        oldest.entry = entry_count + i;
                        oldest_found = true;
                    }
                }
                i++;
            }
            if ((m_file_table_entries[i].START_MAGIC != FlightFileTableEntry::START_MAGIC) ||
                (m_file_table_entries[i].END_MAGIC != FlightFileTableEntry::END_MAGIC))
            {
                loop = false;
                if (i != 0u)
                {
                    newest.entry = entry_count + (i - 1u);
                }
                else
                {
                    if (entry_count != 0u)
                    {              
                        newest.entry = entry_count - 1u;
                    }
                }
            }
        }

        // Next loop
        if (loop)
        {
            address += entries_to_read * sizeof(FlightFileTableEntry);
            entry_count += entries_to_read;
        }
    }
    while (loop && ret && (entry_count != m_file_table_header.file_entry_count));

    return ret;
}


}
