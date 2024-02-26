/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "file.h"

namespace ov
{

/** @brief Constructor */
file::file(lfs_t* lfs, const char* path, int flags) : m_lfs(lfs), m_file{}, m_config{}, m_buffer{}
{
    // Open the file
    m_config.buffer = m_buffer;
    int err         = lfs_file_opencfg(m_lfs, &m_file, path, flags, &m_config);
    if (err != LFS_ERR_OK)
    {
        m_lfs = nullptr;
    }
}

/** @brief Move constructor */
file::file(file&& move) : m_lfs(move.m_lfs), m_file(move.m_file), m_config(move.m_config), m_buffer()
{
    move.m_lfs = nullptr;
}

/** @brief Destructor */
file::~file()
{
    close();
}

/** @brief Close the file */
bool file::close()
{
    bool ret = false;

    if (is_open())
    {
        if (lfs_file_close(m_lfs, &m_file) == LFS_ERR_OK)
        {
            m_lfs = nullptr;
            ret   = true;
        }
    }

    return ret;
}

/** @brief Read data from the file */
bool file::read(void* buffer, size_t size, size_t& read_count)
{
    bool ret = false;

    if (is_open())
    {
        auto count = lfs_file_read(m_lfs, &m_file, buffer, size);
        if (count >= 0)
        {
            read_count = static_cast<size_t>(count);
            ret        = true;
        }
    }

    return ret;
}

/** @brief Write data to the file */
bool file::write(const void* buffer, size_t size, size_t& write_count)
{
    bool ret = false;

    if (is_open())
    {
        auto count = lfs_file_write(m_lfs, &m_file, buffer, size);
        if (count >= 0)
        {
            write_count = static_cast<size_t>(count);
            ret         = true;
        }
    }

    return ret;
}

/** @brief Seek to a specific location in the file */
bool file::seek(int32_t offset, seek_whence whence, int32_t& new_offset)
{
    bool ret = false;

    if (is_open())
    {
        new_offset = lfs_file_seek(m_lfs, &m_file, offset, static_cast<int>(whence));
        ret        = (new_offset >= 0);
    }

    return ret;
}

/** @brief Force data synchornization on the storage medium */
bool file::sync()
{
    bool ret = false;

    if (is_open())
    {
        ret = (lfs_file_sync(m_lfs, &m_file) == LFS_ERR_OK);
    }

    return ret;
}

} // namespace ov
