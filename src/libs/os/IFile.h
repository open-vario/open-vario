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

#ifndef IFILE_H
#define IFILE_H

#include <cstdint>
#include <cstdlib>

namespace open_vario
{



/** \brief Interface for all file accessors implementations */
class IFile
{
    public:

        /** \brief File access */
        enum FileAccess
        {
            /** \brief Read only */
            FA_RONLY = 1u,
            /** \brief Write only */
            FA_WONLY = 2u,
            /** \brief Read-Write */
            FA_RW = 3u
        };

        /** \brief File open mode */
        enum FileMode
        {
            /** \brief Open existing file (fails if its doesn't exists) */
            FM_EXISTING = 0u,
            /** \brief Create file if it doesn't exists */
            FM_CREATE = 1u,
            /** \brief Truncate file if it exists */
            FM_TRUNCATE = 2u
        };

        /** \brief Seek offset */
        enum SeekOffset
        {
            /** \brief Start of file */
            SO_BEGIN = 1u,
            /** \brief Current position */
            SO_CURR = 2u,
            /** \brief End of file */
            SO_END = 3u
        };


        /** \brief Open the file */
        virtual bool open(const char* file_path, const FileAccess file_access, const FileMode file_mode) = 0;

        /** \brief Close the file */
        virtual bool close() = 0;

        /** \brief Set the size of the file */
        virtual bool setFileSize(const size_t new_size) = 0;

        /** \brief Write data to the file */
        virtual bool write(const void* data, const size_t size) = 0;

        /** \brief Read data from the file */
        virtual bool read(void* data, const size_t size, size_t& read) = 0;

        /** \brief Move to a specific offset in the file */
        virtual bool seek(const SeekOffset offset, const int32_t position, int32_t& new_position) = 0;

};

}

#endif // IFILE_H
