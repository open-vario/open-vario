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

#ifndef FILE_H
#define FILE_H

#include "IFile.h"

#include <Windows.h>


namespace open_vario
{


/** \brief File implementation for Windows */
class File : public IFile
{
    public:

        /** \brief Constructor */
        File();

        /** \brief Open the file */
        virtual bool open(const char* file_path, const FileAccess file_access, const FileMode file_mode);

        /** \brief Close the file */
        virtual bool close();

        /** \brief Set the size of the file */
        virtual bool setFileSize(const size_t new_size);

        /** \brief Write data to the file */
        virtual bool write(const void* data, const size_t size);

        /** \brief Read data from the file */
        virtual bool read(void* data, const size_t size, size_t& read);

        /** \brief Move to a specific offset in the file */
        virtual bool seek(const SeekOffset offset, const int32_t position, int32_t& new_position);


    private:

        /** \brief Underlying file handle */
        HANDLE m_file;

};

}

#endif // FILE_H
