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

#ifndef NORFLASHFS_H
#define NORFLASHFS_H

#include <cstdint>

namespace open_vario
{

class NorFlashPartition;




/** \brief NOR flash file system */
class NorFlashFs
{
    public:

        /** \brief Constructor */
        NorFlashFs(NorFlashPartition& nor_flash);


        /** \brief Get the file system version */
        virtual uint32_t getVersion() const { return NORFLASHFS_VERSION; }

        /** \brief Initialize the file system */
        virtual bool init();

        /** \brief Format the file system */
        virtual bool format();

        /** \brief Get the number of files in the file system */
        virtual uint32_t getFileCount() const { return m_file_count; }

        /** \brief Create a file in the filesystem */
        virtual bool createFile(const char* name);

        /** \brief Write data to the file */
        virtual bool writeToFile(const void* const data, const uint32_t size);

        /** \brief Close the written file */
        virtual bool closeWrittenFile();


    private:


        /** \brief NOR flash sector header */
        struct NorFlashSectorHeader
        {
            /** \brief Start magic */
            uint8_t start_magic;
            /** \brief Sector type */
            uint8_t sector_type;
            /** \brief Reserved */
            uint8_t reserved;
            /** \brief End magic */
            uint8_t end_magic;

            /** \brief Start magic */
            static const uint8_t START_MAGIC = 0xAAu;
            /** \brief End magic */
            static const uint8_t END_MAGIC = 0x55u;

            /** \brief Sector type : start of file system */
            static const uint8_t START_OF_FS = 0x00u;
            /** \brief Sector type : start of file */
            static const uint8_t START_OF_FILE = 0xAAu;
            /** \brief Sector type : file data */
            static const uint8_t FILE_DATA = 0x55u;

            /** \brief Reserved value */
            static const uint8_t RESERVED = 0xFFu;
        };

        /** \brief Start of file system header */
        struct StartFsHeader
        {
            /** \brief File system version */
            uint32_t version;
            /** \brief Sector type */
            uint8_t sector_type;
            /** \brief Reserved */
            uint8_t reserved_1;
            /** \brief Reserved */
            uint8_t reserved_2;
            /** \brief Reserved */
            uint8_t reserved_3;
        };

        /** \brief File header */
        struct FileHeader
        {
            /** \brief Max filename length in bytes */
            static const uint8_t MAX_FILENAME_LEN = 63u;
            /** \brief No file size */
            static const uint32_t NO_SIZE = 0xFFFFFFFFu;
            /** \brief No file sector */
            static const uint32_t NO_SECTOR = 0xFFFFFFFFu;

            /** \brief File id */
            uint32_t id;
            /** \brief File size */
            uint32_t size;
            /** \brief Next file sector */
            uint32_t next_file_sector;
            /** \brief Delete flag */
            uint8_t reserved_1;
            /** \brief Reserved */
            uint8_t reserved_2;
            /** \brief Reserved */
            uint8_t reserved_3;
            /** \brief File name length */
            uint8_t name_length;
            /** \brief File name */
            char name[MAX_FILENAME_LEN + 1u];
        };

        /** \brief File data header */
        struct FileDataHeader
        {
            /** \brief File id */
            uint32_t id;
            /** \brief File start sector */
            uint32_t file_start_sector;
            /** \brief Data size */
            uint32_t data_size;
            /** \brief Next data sector */
            uint32_t next_data_sector;
        };


        /** \brief File open context */
        struct FileContext
        {
            /** \brief File sector */
            uint32_t file_sector;
            /** \brief File header */
            FileHeader file_header;
            /** \brief File data header */
            FileDataHeader file_data_header;
            /** \brief Current sector */
            uint32_t current_sector;
            /** \brief Current address */
            uint32_t current_address;
            /** \brief Indicate if the context is in use */
            bool is_in_use;
        };


        /** \brief NOR flash partition */
        NorFlashPartition& m_nor_flash;

        /** \brief Number of files */
        uint32_t m_file_count;

        /** \brief Current file id */
        uint32_t m_current_file_id;


        /** \brief Newest file sector */
        uint32_t m_newest_file_sector;

        /** \brief Oldest file sector */
        uint32_t m_oldest_file_sector;

        



        /** \brief File context for read */
        FileContext m_read_context;

        /** \brief File context for write */
        FileContext m_write_context;



        /** \brief Convert a sector number to a flash address */
        uint32_t toAddress(const uint32_t sector) const;

        /** \brief Write a NOR flash sector header */
        bool writeSectorHeader(const uint32_t sector, const uint8_t sector_type);

        /** \brief Read a NOR flash sector header */
        bool readSectorHeader(const uint32_t sector, NorFlashSectorHeader& header, bool& valid);

        /** \brief Walk through the whole file system to initialize it */
        bool walkThroughInit(const uint32_t file_sector, bool& format_needed);

        /** \brief Go to the end of the current file by going through the data sectors */
        bool goToEndOfFileByDataSector(const uint32_t current_sector, uint32_t& end_sector);

        /** \brief Close the sector currently being written */
        bool closeWrittenSector();

        /** \brief Open the sector to write into it */
        bool openSectorToWrite(const uint8_t open_sector_type);





        /** \brief NOR flash file system version : 1.0.0 */
        static const uint32_t NORFLASHFS_VERSION = 0x00100000u;
};

}

#endif // NORFLASHFS_H
