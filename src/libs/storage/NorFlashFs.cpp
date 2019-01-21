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

#include "NorFlashFs.h"
#include "NorFlashPartition.h"
#include "nano-stl-conf.h"

namespace open_vario
{


/** \brief Constructor */
NorFlashFs::NorFlashFs(NorFlashPartition& nor_flash)
: m_nor_flash(nor_flash)
, m_mutex()
, m_file_count(0u)
, m_current_file_id(0u)
, m_newest_file_sector(FileHeader::NO_SECTOR)
, m_oldest_file_sector(FileHeader::NO_SECTOR)
, m_read_context()
, m_write_context()
{}

/** \brief Initialize the file system */
bool NorFlashFs::init()
{
    bool ret;

    // Read first sector header
    bool sector_valid = false;
    NorFlashSectorHeader sector_header;
    ret = readSectorHeader(0, sector_header, sector_valid);
    if (ret)
    {
        // Check validity
        bool format_needed = true;
        if (sector_valid)
        {
            // Check sector type
            if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
            {
                // Read file system header
                StartFsHeader start_fs_header;
                ret = m_nor_flash.read(toAddress(0) + sizeof(NorFlashSectorHeader), &start_fs_header, sizeof(StartFsHeader));
                if (ret)
                {
                    // Check version
                    if (start_fs_header.version == NORFLASHFS_VERSION)
                    {
                        // Check sector type
                        switch (start_fs_header.sector_type)
                        {
                            case NorFlashSectorHeader::RESERVED:
                            {
                                // Empty file system

                                // Initialize file count and contexts
                                m_file_count = 0u;
                                m_current_file_id = 1u;
                                m_oldest_file_sector = 0u;
                                NANO_STL_MEMSET(&m_write_context, 0, sizeof(m_write_context));
                                NANO_STL_MEMSET(&m_read_context, 0, sizeof(m_read_context));

                                format_needed = false;

                                break;
                            }

                            case NorFlashSectorHeader::START_OF_FILE:
                            {
                                // Start of file

                                // Initialize file system by walk through
                                ret = walkThroughInit(0, format_needed);

                                break;
                            }

                            case NorFlashSectorHeader::FILE_DATA:
                            {
                                // File data

                                // Look for the start of the file
                                FileDataHeader file_data_header;
                                ret = m_nor_flash.read(toAddress(0) + sizeof(NorFlashSectorHeader) + sizeof(StartFsHeader), &file_data_header, sizeof(FileDataHeader));
                                if (ret)
                                {
                                    // Check file sector number
                                    if (file_data_header.file_start_sector < m_nor_flash.getSectorCount())
                                    {
                                        // Initialize file system by walk through
                                        ret = walkThroughInit(file_data_header.file_start_sector, format_needed);
                                    }
                                }
                                break;
                            }

                            default:
                            {
                                // Invalid value => format
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                format_needed = true;
            }
        }

        // Format file system if needed
        if (ret && format_needed)
        {
            ret = format();
        }
    }

    return ret;
}

/** \brief Format the file system */
bool NorFlashFs::format()
{
    bool ret;

    // Lock filesystem
    m_mutex.lock();

    // Erase all the NOR flash partition
    ret = m_nor_flash.fullErase();
    if (ret)
    {
        // Write the first sector header
        ret = writeSectorHeader(0u, NorFlashSectorHeader::START_OF_FS);
        if (ret)
        {
            // Write the file system header
            StartFsHeader start_fs_header;
            start_fs_header.version = NORFLASHFS_VERSION;
            start_fs_header.sector_type = NorFlashSectorHeader::RESERVED;
            start_fs_header.reserved_1 = NorFlashSectorHeader::RESERVED;
            start_fs_header.reserved_2 = NorFlashSectorHeader::RESERVED;
            start_fs_header.reserved_3 = NorFlashSectorHeader::RESERVED;
            ret = m_nor_flash.write(sizeof(NorFlashSectorHeader), &start_fs_header, sizeof(StartFsHeader));
            if (ret)
            {
                // Initialize file count and contexts
                m_file_count = 0u;
                m_current_file_id = 1u;
                m_oldest_file_sector = 0u;
                NANO_STL_MEMSET(&m_write_context, 0, sizeof(m_write_context));
                NANO_STL_MEMSET(&m_read_context, 0, sizeof(m_read_context));
            }
        }
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}


/** \brief Create a file in the filesystem */
bool NorFlashFs::createFile(const char name[])
{
    bool ret = false;

    // Lock filesystem
    m_mutex.lock();

    // Check if the write context is available
    if (!m_write_context.is_in_use)
    {
        // Open sector to write file
        m_write_context.current_sector = m_write_context.file_sector;
        ret = openSectorToWrite(NorFlashSectorHeader::START_OF_FILE);
        if (ret)
        {
            // Create the file header
            FileHeader& file_header = m_write_context.file_header;
            file_header.id = m_current_file_id;
            file_header.size = FileHeader::NO_SIZE;
            file_header.next_file_sector = FileHeader::NO_SECTOR;
            file_header.name_length = static_cast<uint8_t>(NANO_STL_STRNLEN(name, FileHeader::MAX_FILENAME_LEN));
            NANO_STL_STRNCPY(file_header.name, name, file_header.name_length);
            file_header.reserved_1 = NorFlashSectorHeader::RESERVED;
            file_header.reserved_2 = NorFlashSectorHeader::RESERVED;
            file_header.reserved_3 = NorFlashSectorHeader::RESERVED;

            // Write the file header
            ret = m_nor_flash.write(m_write_context.current_address, &file_header, sizeof(FileHeader));
            if (ret)
            {
                // Update file header and context
                file_header.size = 0;
                m_write_context.current_sector = m_write_context.file_sector;
                m_write_context.current_address += sizeof(FileHeader);

                // Write the file data header
                FileDataHeader& file_data_header = m_write_context.file_data_header;
                file_data_header.id = file_header.id;
                file_data_header.file_start_sector = m_write_context.file_sector;
                file_data_header.data_size = FileHeader::NO_SIZE;
                file_data_header.next_data_sector = FileHeader::NO_SECTOR;
                ret = m_nor_flash.write(m_write_context.current_address, &file_data_header, sizeof(FileDataHeader));
                if (ret)
                {
                    m_current_file_id++;
                    file_data_header.data_size = 0u;
                    m_write_context.is_in_use = true;
                    m_write_context.current_address += sizeof(FileDataHeader);
                }
            }
        }
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}

/** \brief Write data to the file */
bool NorFlashFs::writeToFile(const void* const data, const uint32_t size)
{
    bool ret = false;

    // Lock filesystem
    m_mutex.lock();

    // Check if the write context is available
    if (m_write_context.is_in_use)
    {
        // Check if data fits into current sector
        const uint32_t end_of_sector_address = toAddress(m_write_context.current_sector) + m_nor_flash.getSectorSize();
        if (m_write_context.current_address + size < end_of_sector_address)
        {
            // Write data
            ret = m_nor_flash.write(m_write_context.current_address, data, size);
            if (ret)
            {
                // Update file header and context
                m_write_context.current_address += size;
                m_write_context.file_header.size += size;
                m_write_context.file_data_header.data_size += size;
            }
        }
        else
        {
            // Write data fitting into current sector
            const uint32_t fit_size = end_of_sector_address - m_write_context.current_address;
            const uint32_t left_size = size - fit_size;
            const uint8_t* u8_data = reinterpret_cast<const uint8_t*>(data); 
            ret = m_nor_flash.write(m_write_context.current_address, u8_data, fit_size);
            if (ret)
            {
                // Update file header and context
                u8_data += fit_size;
                m_write_context.file_header.size += fit_size;
                m_write_context.file_data_header.data_size += fit_size;

                // Compute next sector number
                m_write_context.file_data_header.next_data_sector = m_write_context.current_sector + 1u;
                if (m_write_context.file_data_header.next_data_sector == m_nor_flash.getSectorCount())
                {
                    m_write_context.file_data_header.next_data_sector = 0;
                }

                // Close current sector
                ret = closeWrittenSector();
                if (ret)
                {
                    // Open next sector
                    m_write_context.current_sector = m_write_context.file_data_header.next_data_sector;
                    ret = openSectorToWrite(NorFlashSectorHeader::FILE_DATA);
                    if (ret)
                    {
                        // Write data left into current sector
                        ret = m_nor_flash.write(m_write_context.current_address, u8_data, left_size);
                        if (ret)
                        {
                            // Update file header and context
                            m_write_context.current_address += left_size;
                            m_write_context.file_header.size += left_size;
                            m_write_context.file_data_header.data_size += left_size;
                        }
                    }
                } 
            }           
        }
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}

/** \brief Close the written file */
bool NorFlashFs::closeWrittenFile()
{
    bool ret = false;

    // Lock filesystem
    m_mutex.lock();

    // Check if the write context is available
    if (m_write_context.is_in_use)
    {
        // Compute next sector number
        m_write_context.file_header.next_file_sector = m_write_context.current_sector + 1u;
        if (m_write_context.file_header.next_file_sector == m_nor_flash.getSectorCount())
        {
            m_write_context.file_header.next_file_sector = 0;
        }

        // Close current sector
        ret = closeWrittenSector();
        if (ret)
        {
            // Read file header sector
            bool sector_valid = false;
            NorFlashSectorHeader sector_header;
            uint32_t current_address = toAddress(m_write_context.file_sector);
            ret = readSectorHeader(m_write_context.file_sector, sector_header, sector_valid);
            ret = ret && sector_valid;
            if (ret)
            {
                // Update address
                current_address += sizeof(NorFlashSectorHeader);

                // Check sector type
                uint8_t sector_type = sector_header.sector_type; 
                if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
                {
                    // Read file system header
                    StartFsHeader start_fs_header;
                    ret = m_nor_flash.read(current_address, &start_fs_header, sizeof(StartFsHeader));
                    if (ret)
                    {
                        // Update address
                        current_address += sizeof(StartFsHeader);

                        // Save sector type
                        sector_type = start_fs_header.sector_type;
                    }
                }
                if (ret)
                {
                    if (sector_type == NorFlashSectorHeader::START_OF_FILE)
                    {
                        // Update the file header
                        ret = m_nor_flash.write(current_address + sizeof(FileDataHeader::id), 
                                                &m_write_context.file_header.size, 
                                                sizeof(FileHeader::size) + sizeof(FileHeader::next_file_sector));
                        if (ret)
                        {
                            // File closed
                            m_write_context.is_in_use = false;
                            m_write_context.file_sector = m_write_context.file_header.next_file_sector;
                            m_file_count++;
                        }
                    }
                    else
                    {
                        // Not a valid sector type => corrupted sector
                        ret = false;
                    }
                }
            }        
        }
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}

/** \brief Open a file in the filesystem */
bool NorFlashFs::openFile(const uint32_t file_number, FileInfo& file_info)
{
    bool ret = false;

    // Lock filesystem
    m_mutex.lock();

    // Check if the read context is available
    // and if the file_number is valid
    if (!m_read_context.is_in_use && 
        (file_number < m_file_count))
    {
        // Walk through the file system
        bool found = false;
        bool valid = false;
        uint32_t current_file_number = 0u;
        uint32_t sector = m_oldest_file_sector;
        NorFlashSectorHeader sector_header;
        do
        {
            // Read sector header
            ret = readSectorHeader(sector, sector_header, valid);
            if (ret && valid)
            {
                // Check sector type
                uint8_t sector_type = sector_header.sector_type; 
                m_read_context.current_address = toAddress(sector) + sizeof(NorFlashSectorHeader);
                if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
                {
                    // Read file system header
                    StartFsHeader start_fs_header;
                    ret = m_nor_flash.read(m_read_context.current_address, &start_fs_header, sizeof(StartFsHeader));
                    if (ret)
                    {
                        sector_type = start_fs_header.sector_type;
                        m_read_context.current_address += sizeof(StartFsHeader);
                    }
                }
                if (ret)
                {
                    if (sector_type == NorFlashSectorHeader::START_OF_FILE)
                    {
                        // Start of file, read header
                        ret = m_nor_flash.read(m_read_context.current_address, &m_read_context.file_header, sizeof(FileHeader));
                        if (ret)
                        {
                            // Jump to next file
                            if (current_file_number != file_number)
                            {
                                current_file_number++;
                                sector = m_read_context.file_header.next_file_sector;
                            }
                            else
                            {
                                found = true;
                            }
                        }
                    }
                }
            }
        }
        while (ret && valid && !found);
        if (ret && valid)
        {
            // Read data header
            m_read_context.current_address += sizeof(FileHeader);
            ret = m_nor_flash.read(m_read_context.current_address, &m_read_context.file_data_header, sizeof(FileDataHeader));
            if (ret)
            {
                // Initialize read context 
                m_read_context.is_in_use = true;
                m_read_context.current_address += sizeof(FileDataHeader);
                m_read_context.file_sector = sector;
                m_read_context.current_sector = sector;

                // Extract file info
                file_info.size = m_read_context.file_header.size;
                NANO_STL_STRNCPY(file_info.name, m_read_context.file_header.name, FileInfo::MAX_FILENAME_LEN);
            }
        }
        else
        {
            ret = false;
        }
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}

/** \brief Read data from the file */
bool NorFlashFs::readFromFile(void* const data, const uint32_t size, uint32_t& read)
{
    bool ret = false;

    // Lock filesystem
    m_mutex.lock();

    // Check if the read context is available
    if (m_read_context.is_in_use)
    {
        // Check if data fits into current sector
        if (size <= m_read_context.file_data_header.data_size)
        {
            // Read data
            ret = m_nor_flash.read(m_read_context.current_address, data, size);
            if (ret)
            {
                // Update file header and context
                m_read_context.current_address += size;
                m_read_context.file_header.size -= size;
                m_read_context.file_data_header.data_size -= size;
                read = size;
            }
        }
        else
        {
            // Read data fitting into current sector
            const uint32_t fit_size = m_read_context.file_data_header.data_size;
            const uint32_t left_size = size - fit_size;
            uint8_t* u8_data = reinterpret_cast<uint8_t*>(data); 
            ret = m_nor_flash.read(m_read_context.current_address, u8_data, fit_size);
            if (ret)
            {
                // Update file header and context
                u8_data += fit_size;
                m_read_context.file_header.size -= fit_size;
                m_read_context.file_data_header.data_size -= fit_size;
                read = fit_size;

                // Check if there is data left into the file
                if (m_read_context.file_header.size != 0u)
                {
                    // Open next sector
                    bool valid = false;
                    NorFlashSectorHeader sector_header;
                    m_read_context.current_sector = m_read_context.file_data_header.next_data_sector;
                    ret = readSectorHeader(m_read_context.current_sector, sector_header, valid);
                    ret = ret && valid;
                    if (ret)
                    {
                        // Check sector type
                        uint8_t sector_type = sector_header.sector_type; 
                        m_read_context.current_address = toAddress(m_read_context.current_sector) + sizeof(NorFlashSectorHeader);
                        if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
                        {
                            // Read file system header
                            StartFsHeader start_fs_header;
                            ret = m_nor_flash.read(m_read_context.current_address, &start_fs_header, sizeof(StartFsHeader));
                            if (ret)
                            {
                                sector_type = start_fs_header.sector_type;
                                m_read_context.current_address += sizeof(StartFsHeader);
                            }
                        }
                        if (ret)
                        {
                            if (sector_type == NorFlashSectorHeader::FILE_DATA)
                            {
                                // Read file data header
                                ret = m_nor_flash.read(m_read_context.current_address, &m_read_context.file_data_header, sizeof(FileDataHeader));
                                if (ret)
                                {
                                    m_read_context.current_address += sizeof(FileDataHeader);
                                }
                            }
                            else
                            {
                                ret = false;
                            }
                        }
                    }
                    if (ret)
                    {
                        // Read data left from current sector
                        uint32_t size_to_read = left_size;
                        if (size_to_read > m_read_context.file_data_header.data_size)
                        {
                            size_to_read = m_read_context.file_data_header.data_size;
                        }
                        ret = m_nor_flash.read(m_read_context.current_address, u8_data, size_to_read);
                        if (ret)
                        {
                            // Update file header and context
                            m_read_context.current_address += size_to_read;
                            m_read_context.file_header.size -= size_to_read;
                            m_read_context.file_data_header.data_size -= size_to_read;
                            read += size_to_read;
                        }
                    }
                }
            }
        }
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}

/** \brief Close the read file */
bool NorFlashFs::closeReadFile()
{
    bool ret = false;

    // Lock filesystem
    m_mutex.lock();

    // Check if the read context is available
    if (m_read_context.is_in_use)
    {
        // Read context is now free
        m_read_context.is_in_use = false;
    }

    // Unlock filesystem
    m_mutex.unlock();

    return ret;
}


/** \brief Convert a sector number to a flash address */
uint32_t NorFlashFs::toAddress(const uint32_t sector) const
{
    return (sector * m_nor_flash.getSectorSize());
}

/** \brief Write a NOR flash sector header */
bool NorFlashFs::writeSectorHeader(const uint32_t sector, const uint8_t sector_type)
{
    // Initialize sector header data
    NorFlashSectorHeader header;
    header.start_magic = NorFlashSectorHeader::START_MAGIC;
    header.sector_type = sector_type;
    header.reserved = NorFlashSectorHeader::RESERVED;
    header.end_magic = NorFlashSectorHeader::END_MAGIC;

    // Read sector header
    const bool ret = m_nor_flash.write(toAddress(sector), &header, sizeof(NorFlashSectorHeader));
    return ret;
}

/** \brief Read a NOR flash sector header */
bool NorFlashFs::readSectorHeader(const uint32_t sector, NorFlashSectorHeader& header, bool& valid)
{
    // Read sector header
    valid = false;
    const bool ret = m_nor_flash.read(toAddress(sector), &header, sizeof(NorFlashSectorHeader));
    if (ret)
    {
        // Check sector start and end magic
        if ((header.start_magic == NorFlashSectorHeader::START_MAGIC) && 
            (header.end_magic == NorFlashSectorHeader::END_MAGIC))
        {
            // Check sector type
            if ((header.sector_type == NorFlashSectorHeader::START_OF_FS) || 
                (header.sector_type == NorFlashSectorHeader::START_OF_FILE) || 
                (header.sector_type == NorFlashSectorHeader::FILE_DATA))
            {
                valid = true;
            }
        }
    }

    return ret;
}

/** \brief Walk through the whole file system to initialize it */
bool NorFlashFs::walkThroughInit(const uint32_t file_sector, bool& format_needed)
{
    bool ret = false;

    NorFlashSectorHeader sector_header;

    uint32_t first_file_id = 0u;
    uint32_t oldest_file_id = 0xFFFFFFFFu;
    m_current_file_id = 0u;    
    m_oldest_file_sector = FileHeader::NO_SECTOR;
    m_newest_file_sector = FileHeader::NO_SECTOR;
    m_file_count = 0u;

    // Walk through the file system
    uint32_t sector = file_sector;
    bool last_file_reached = false;
    format_needed = false;
    m_write_context.is_in_use = true;
    do
    {
        // Read sector header
        bool valid = false;
        ret = readSectorHeader(sector, sector_header, valid);
        if (ret)
        {
            if (valid)
            {
                // Check sector type
                uint8_t sector_type = sector_header.sector_type; 
                uint32_t read_address = toAddress(sector) + sizeof(NorFlashSectorHeader);
                if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
                {
                    // Read file system header
                    StartFsHeader start_fs_header;
                    ret = m_nor_flash.read(read_address, &start_fs_header, sizeof(StartFsHeader));
                    if (ret)
                    {
                        sector_type = start_fs_header.sector_type;
                        read_address += sizeof(StartFsHeader);
                    }
                }
                if (ret)
                {
                    if (sector_type == NorFlashSectorHeader::START_OF_FILE)
                    {
                        // Start of file, read header
                        m_write_context.file_sector = sector;
                        m_write_context.current_sector = sector;
                        ret = m_nor_flash.read(read_address, &m_write_context.file_header, sizeof(FileHeader));
                        if (ret)
                        {
                            // Check if the file as already been processed
                            if (m_write_context.file_header.id == first_file_id)
                            {
                                // Last file reached
                                last_file_reached = true;
                            }
                            else
                            {
                                // Save first file id
                                if (first_file_id == 0u)
                                {
                                    first_file_id = m_write_context.file_header.id;
                                }

                                // Update file ids and count
                                if (m_write_context.file_header.id > m_current_file_id)
                                {
                                    m_current_file_id = m_write_context.file_header.id;
                                    m_newest_file_sector = sector;
                                }
                                if (m_write_context.file_header.id < oldest_file_id)
                                {
                                    oldest_file_id = m_write_context.file_header.id;
                                    m_oldest_file_sector = sector;
                                }

                                // Jump to next file
                                if (m_write_context.file_header.next_file_sector == FileHeader::NO_SECTOR)
                                {
                                    // Invalid file header => File file has not been closed properly
                                    
                                    // Try to find the end of the file
                                    uint32_t end_sector = 0u;
                                    uint8_t end_sector_type = 0u;
                                    m_write_context.file_header.size = 0u;
                                    ret = goToEndOfFileByDataSector(sector, end_sector, end_sector_type);
                                    if (ret)
                                    {
                                        // Close file properly
                                        m_write_context.current_sector = end_sector;
                                        closeWrittenFile();
                                    }
                                }
                                else
                                {
                                    m_file_count++;
                                }

                                // Next sector
                                if (sector == m_newest_file_sector)
                                {
                                    m_write_context.file_sector = m_write_context.file_header.next_file_sector;
                                }
                                sector = m_write_context.file_header.next_file_sector;
                            }
                        }
                    }
                    else
                    {
                        // Not a start of file sector
                        if (m_file_count == 0)
                        {
                            // File system corrupted
                            format_needed = true;
                        }
                        else
                        {
                            // Go to the end of the file
                            uint32_t end_sector = 0;
                            ret = goToEndOfFileByDataSector(sector, end_sector, sector_type);
                            if (ret)
                            {
                                sector = end_sector + 1u;
                                if (sector == m_nor_flash.getSectorCount())
                                {
                                    sector = 0;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                // Last file reached
                last_file_reached = true;
            }
        }
    }
    while (ret && !format_needed && !last_file_reached);  

    if (!format_needed)
    {
        if (m_file_count == 0u)
        {
            format_needed = true;
        }
        else
        {
            m_current_file_id++;
        }
    }
    m_write_context.is_in_use = false;

    return ret;
}

/** \brief Go to the end of the current file */
bool NorFlashFs::goToEndOfFileByDataSector(const uint32_t current_sector, uint32_t& end_sector, uint8_t& end_sector_type)
{
    bool ret = true;

    bool eof = false;
    uint32_t sector = current_sector;
    end_sector = sector;
    do
    {
        // Read sector header
        bool valid = false;
        NorFlashSectorHeader sector_header;
        ret = readSectorHeader(sector, sector_header, valid);
        if (ret)
        {
            // Check sector type
            uint8_t sector_type = sector_header.sector_type; 
            m_write_context.current_address = toAddress(sector) + sizeof(NorFlashSectorHeader);
            if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
            {
                // Read file system header
                StartFsHeader start_fs_header;
                ret = m_nor_flash.read(m_write_context.current_address, &start_fs_header, sizeof(StartFsHeader));
                if (ret)
                {
                    sector_type = start_fs_header.sector_type;
                    m_write_context.current_address += sizeof(StartFsHeader);
                }
            }
            if (ret)
            {
                if ((sector_type == NorFlashSectorHeader::FILE_DATA) ||
                    ((sector == current_sector) && (sector_type == NorFlashSectorHeader::START_OF_FILE)))
                {
                    // Data sector or sart of file sector => still in current file
                    if (sector_type == NorFlashSectorHeader::START_OF_FILE)
                    {
                        m_write_context.current_address += sizeof(FileHeader);
                    }
                    ret = m_nor_flash.read(m_write_context.current_address, &m_write_context.file_data_header, sizeof(FileDataHeader));
                    if (ret)
                    {
                        m_write_context.current_address += sizeof(FileDataHeader);
                        if (m_write_context.file_data_header.data_size == FileHeader::NO_SIZE)
                        {
                            // Try to compute written data size
                            uint32_t file_data = 0u;
                            const uint32_t max_data_size = m_nor_flash.getSectorSize() - m_write_context.current_address;
                            m_write_context.file_data_header.data_size = 0u;
                            do
                            {
                                ret = m_nor_flash.read(m_write_context.current_address, &file_data, sizeof(file_data));
                                if (ret && (file_data != FileHeader::NO_SIZE))
                                {
                                    m_write_context.file_data_header.data_size += sizeof(file_data);
                                    m_write_context.current_address += sizeof(file_data);
                                }
                            }
                            while (ret &&
                                   (m_write_context.file_data_header.data_size != max_data_size) && 
                                   (file_data != FileHeader::NO_SIZE));
                        }
                        m_write_context.file_header.size += m_write_context.file_data_header.data_size;
                        end_sector = sector;
                        sector++;
                    }
                }
                else
                {
                    // Not a data sector => end of file
                    eof = true;
                    end_sector_type = sector_type;
                }
            }
        }
    }
    while (ret && !eof);

    return ret;
}

/** \brief Close the sector currently being written */
bool NorFlashFs::closeWrittenSector()
{
    bool ret;

    // Read sector header
    bool sector_valid = false;
    NorFlashSectorHeader sector_header;
    uint32_t current_address = toAddress(m_write_context.current_sector);
    ret = readSectorHeader(m_write_context.current_sector, sector_header, sector_valid);
    ret = ret && sector_valid;
    if (ret)
    {
        // Update address
        current_address += sizeof(NorFlashSectorHeader);

        // Check sector type
        uint8_t sector_type = sector_header.sector_type; 
        if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
        {
            // Read file system header
            StartFsHeader start_fs_header;
            ret = m_nor_flash.read(current_address, &start_fs_header, sizeof(StartFsHeader));
            if (ret)
            {
                // Update address
                current_address += sizeof(StartFsHeader);

                // Save sector type
                sector_type = start_fs_header.sector_type;
            }
        }
        if (ret)
        {
            if (sector_type == NorFlashSectorHeader::START_OF_FILE)
            {
                // Start of file sector => skip file header to jump to file data header
                current_address += sizeof(FileHeader);                
            }
            else if (sector_type == NorFlashSectorHeader::FILE_DATA)
            {
                // File data sector => already at the good address
            }
            else
            {
                // Not a valid sector type => corrupted sector
                ret = false;
            }
            if (ret)
            {
                // Update the file data header
                ret = m_nor_flash.write(current_address + sizeof(FileDataHeader::id) + sizeof(FileDataHeader::file_start_sector), 
                                        &m_write_context.file_data_header.data_size, 
                                        sizeof(FileDataHeader::data_size) + sizeof(FileDataHeader::next_data_sector));
            }
        }
    }

    return ret;
}

/** \brief Open the sector to write into it */
bool NorFlashFs::openSectorToWrite(const uint8_t open_sector_type)
{
    bool ret;

    // Read sector header
    bool sector_valid = false;
    NorFlashSectorHeader sector_header;
    m_write_context.current_address = toAddress(m_write_context.current_sector);
    ret = readSectorHeader(m_write_context.current_sector, sector_header, sector_valid);
    if (ret)
    {
        // Check sector's validity
        bool start_of_fs = false;
        if (sector_valid)
        {
            // Update read address
            uint32_t read_address = m_write_context.current_address + sizeof(NorFlashSectorHeader);

            uint8_t sector_type = sector_header.sector_type; 
            if (sector_header.sector_type == NorFlashSectorHeader::START_OF_FS)
            {
                // Read file system header
                StartFsHeader start_fs_header;
                ret = m_nor_flash.read(read_address, &start_fs_header, sizeof(StartFsHeader));
                if (ret)
                {
                    // Save sector type
                    sector_type = start_fs_header.sector_type;
                    start_of_fs = true;

                    // Update read address
                    read_address += sizeof(StartFsHeader);
                }
            }
            if (ret)
            {
                if (sector_type == NorFlashSectorHeader::START_OF_FILE)
                {
                    // Start of file sector => update oldest sector
                    if (m_write_context.current_sector == m_oldest_file_sector)
                    {
                        // Get next file sector
                        ret = m_nor_flash.read(read_address + sizeof(FileHeader::id) + sizeof(FileHeader::size), &m_oldest_file_sector, sizeof(FileHeader::next_file_sector));
                        if (ret)
                        {
                            // Update file count
                            if (m_file_count != 0u)
                            {
                                m_file_count--;
                            }
                        }
                    }
                    
                }
            }
        }
        if (ret)
        {
            // Erase sector
            ret = m_nor_flash.sectorErase(toAddress(m_write_context.current_sector));
            if (ret)
            {
                // Write sector header
                if (start_of_fs)
                {
                    ret = writeSectorHeader(m_write_context.current_sector, NorFlashSectorHeader::START_OF_FS);
                    if (ret)
                    {
                        // Update write address
                        m_write_context.current_address += sizeof(NorFlashSectorHeader);

                        // Write the file system header
                        StartFsHeader start_fs_header;
                        start_fs_header.version = NORFLASHFS_VERSION;
                        start_fs_header.sector_type = open_sector_type;
                        start_fs_header.reserved_1 = NorFlashSectorHeader::RESERVED;
                        start_fs_header.reserved_2 = NorFlashSectorHeader::RESERVED;
                        start_fs_header.reserved_3 = NorFlashSectorHeader::RESERVED; 
                        ret = m_nor_flash.write(m_write_context.current_address, &start_fs_header, sizeof(StartFsHeader));
                        if (ret)
                        {
                            // Update write address
                            m_write_context.current_address += sizeof(StartFsHeader);
                        }
                    }
                }
                else
                {
                    ret = writeSectorHeader(m_write_context.current_sector, open_sector_type);
                    if (ret)
                    {
                        // Update write address
                        m_write_context.current_address += sizeof(NorFlashSectorHeader);
                    }
                }
                if (ret)
                {
                    if (open_sector_type == NorFlashSectorHeader::FILE_DATA)
                    {
                        // Initialize file data header
                        FileDataHeader& file_data_header = m_write_context.file_data_header;
                        file_data_header.id = m_write_context.file_header.id;
                        file_data_header.file_start_sector = m_write_context.file_sector;
                        file_data_header.data_size = FileHeader::NO_SIZE;
                        file_data_header.next_data_sector = FileHeader::NO_SECTOR;
                        ret = m_nor_flash.write(m_write_context.current_address, &file_data_header, sizeof(FileDataHeader));
                        if (ret)
                        {
                            // Update file data header and write address
                            file_data_header.data_size = 0;
                            m_write_context.current_address += sizeof(FileDataHeader);
                        }
                    }
                }
            }
        }
    }

    return ret;
}


}
