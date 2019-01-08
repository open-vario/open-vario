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

namespace open_vario
{


/** \brief Constructor */
File::File()
: m_file(INVALID_HANDLE_VALUE)
{}

/** \brief Open the file */
bool File::open(const char* file_path, const FileAccess file_access, const FileMode file_mode)
{
    bool ret = true;

    // Prepare parameters
    if (file_path != nullptr)
    {
        DWORD access = 0;
        switch (file_access)
        {
            case FA_RONLY:
            {
                access = GENERIC_READ;
                break;
            }
            case FA_WONLY:
            {
                access = GENERIC_WRITE;
                break;
            }
            case FA_RW:
            {
                access = (GENERIC_READ | GENERIC_WRITE);
                break;
            }
            default:
            {
                // Invalid value
                ret = false;
                break;
            }
        }

        DWORD mode = 0;
        if (((file_mode) & FM_CREATE) != 0)
        {
            mode |= OPEN_ALWAYS;
        }
        else
        {
            mode |= OPEN_EXISTING;
        }
        if (((file_mode) & FM_TRUNCATE) != 0)
        {
            mode |= TRUNCATE_EXISTING;
        }
        if (ret)
        {
            // Open the file
            m_file = CreateFileA(file_path, access, FILE_SHARE_READ, nullptr, mode, FILE_ATTRIBUTE_NORMAL, nullptr);
            ret = (m_file != INVALID_HANDLE_VALUE);
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
    const bool ret = (CloseHandle(m_file) != 0);
    return ret;
}

/** \brief Set the size of the file */
bool File::setFileSize(const size_t new_size)
{
    bool ret;

    // Move the file pointer to the desired size
    ret = (SetFilePointer(m_file, static_cast<DWORD>(new_size), nullptr, FILE_BEGIN) != INVALID_SET_FILE_POINTER);
    if (ret)
    {
        // Set the file size
        ret = (SetEndOfFile(m_file) != 0);
    }
    
    return ret;
}

/** \brief Write data to the file */
bool File::write(const void* data, const size_t size)
{
    DWORD written = 0;
    const bool ret = (WriteFile(m_file, data, static_cast<DWORD>(size), &written, nullptr) != 0);
    return ret;
}

/** \brief Read data from the file */
bool File::read(void* data, const size_t size, size_t& read)
{
    const bool ret = (ReadFile(m_file, data, static_cast<DWORD>(size), reinterpret_cast<DWORD*>(&read), nullptr) != 0);
    return ret;
}

/** \brief Move to a specific offset in the file */
bool File::seek(const SeekOffset offset, const int32_t position, int32_t& new_position)
{
    bool ret = true;

    // Prepare parameters
    DWORD move_method = 0;
    switch (offset)
    {
        case SO_BEGIN:
        {
            move_method = FILE_BEGIN;
            break;
        }
        case SO_CURR:
        {
            move_method = FILE_CURRENT;
            break;
        }
        case SO_END:
        {
            move_method = FILE_END;
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
        DWORD pos = SetFilePointer(m_file, position, nullptr, move_method);
        ret = (pos != INVALID_SET_FILE_POINTER);
        if (ret)
        {
            new_position = static_cast<int32_t>(pos);
        }
    }

    return ret;
}


}
