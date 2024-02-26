/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "flight_file.h"
#include "fs.h"

namespace ov
{

/** @brief Constructor to open the file for read operations */
flight_file::flight_file(const char* path) : m_header{}, m_file(ov::fs::open(path, ov::fs::o_rdonly))
{
    // Read header
    if (m_file.is_open())
    {
        uint32_t magic    = 0;
        bool     is_valid = m_file.read(magic);
        if (is_valid && (magic == header::MAGIC_NUMBER))
        {
            is_valid = m_file.read(m_header);
        }
        if (!is_valid)
        {
            m_file.close();
        }
    }
}

/** @brief Constructor to open the file for write operations */
flight_file::flight_file(const char* path, header& header)
    : m_header(header), m_file(ov::fs::open(path, ov::fs::o_creat | ov::fs::o_trunc | ov::fs::o_wronly))
{
    // Write header
    if (m_file.is_open())
    {
        uint32_t magic    = header::MAGIC_NUMBER;
        bool     is_valid = m_file.write(magic);
        is_valid          = is_valid && m_file.write(m_header);
        if (!is_valid)
        {
            m_file.close();
        }
    }
}

/** @brief Close the file */
bool flight_file::close()
{
    return m_file.close();
}

/** @brief Write a flight entry to the file */
bool flight_file::write(const entry& e)
{
    bool ret = false;

    if (m_file.is_open())
    {
        ret = m_file.write(e);
    }

    return ret;
}

/** @brief Read a flight entry from the file */
bool flight_file::read(entry& e)
{
    bool ret = false;

    if (m_file.is_open())
    {
        ret = m_file.read(e);
    }

    return ret;
}

} // namespace ov
