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

#include "File.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

namespace open_vario
{


/** \brief Constructor */
File::File()
: m_file(-1)
{}

/** \brief Open the file */
bool File::open(const char* file_path, const FileAccess file_access, const FileMode file_mode)
{
    bool ret = true;

    // Prepare parameters
    if (file_path != NULL)
    {
        int flags = 0;
        switch (file_access)
        {
            case FA_RONLY:
            {
                flags |= O_RDONLY;
                break;
            }
            case FA_WONLY:
            {
                flags |= O_WRONLY;
                break;
            }
            case FA_RW:
            {
                flags |= O_RDWR;
                break;
            }
            default:
            {
                // Invalid value
                ret = false;
                break;
            }
        }

        if (((file_mode) & FM_CREATE) != 0)
        {
            flags |= O_CREAT;
        }
        if (((file_mode) & FM_TRUNCATE) != 0)
        {
            flags |= O_TRUNC;
        }
        if (ret)
        {
            // Open the file
            m_file = ::open(file_path, flags);
            ret = (m_file >= 0);
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

/** \brief Close the file */
bool File::close()
{
    const bool ret = (::close(m_file) == 0);
    return ret;
}

/** \brief Set the size of the file */
bool File::setFileSize(const size_t new_size)
{
    // Set the file size
    const bool ret = (ftruncate(m_file, static_cast<off_t>(new_size)) == 0);
    return ret;
}

/** \brief Write data to the file */
bool File::write(const void* data, const size_t size)
{
    const ssize_t n = ::write(m_file, data, size);
    const bool ret = (n == static_cast<ssize_t>(size));
    return ret;
}

/** \brief Read data from the file */
bool File::read(void* data, const size_t size, size_t& read)
{
    const ssize_t n = ::read(m_file, data, size);
    const bool ret = (n >= 0);
    if (ret)
    {
        read = static_cast<size_t>(n);
    }
    return ret;
}

/** \brief Move to a specific offset in the file */
bool File::seek(const SeekOffset offset, const int32_t position, int32_t& new_position)
{
    bool ret = true;

    // Prepare parameters
    int whence = 0;
    switch (offset)
    {
        case SO_BEGIN:
        {
            whence = SEEK_SET;
            break;
        }
        case SO_CURR:
        {
            whence = SEEK_CUR;
            break;
        }
        case SO_END:
        {
            whence = SEEK_END;
            break;
        }
        default:
        {
            // Invalid value
            ret = false;
        }
    }
    if (ret)
    {
        // Move file pointer
        off_t pos = lseek(m_file, static_cast<off_t>(position), whence);
        ret = (pos != static_cast<off_t>(-1));
        if (ret)
        {
            new_position = static_cast<int32_t>(pos);
        }
    }

    return ret;
}


}
