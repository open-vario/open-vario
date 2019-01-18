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

#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include <cstdint>

namespace open_vario
{


/** \brief Interface for all filesystems implementations */
class IFileSystem
{
    public:

        /** \brief File information */
        struct FileInfo
        {
            /** \brief Max filename length in bytes */
            static const uint8_t MAX_FILENAME_LEN = 63u;

            /** \brief File size */
            uint32_t size;

            /** \brief File name */
            char name[MAX_FILENAME_LEN + 1u];
        };


        /** \brief Get the file system version */
        virtual const char* getVersionString() const = 0;

        /** \brief Initialize the file system */
        virtual bool init() = 0;

        /** \brief Format the file system */
        virtual bool format() = 0;

        /** \brief Get the number of files in the file system */
        virtual uint32_t getFileCount() const = 0;


        /** \brief Create a file in the filesystem */
        virtual bool createFile(const char name[]) = 0;

        /** \brief Write data to the file */
        virtual bool writeToFile(const void* const data, const uint32_t size) = 0;

        /** \brief Close the written file */
        virtual bool closeWrittenFile() = 0;


        /** \brief Open a file in the filesystem */
        virtual bool openFile(const uint32_t file_number, FileInfo& file_info) = 0;

        /** \brief Read data from the file */
        virtual bool readFromFile(void* const data, const uint32_t size, uint32_t& read) = 0;

        /** \brief Close the read file */
        virtual bool closeReadFile() = 0;

};

}

#endif // IFILESYSTEM_H
