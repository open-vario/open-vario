
#include "dir.h"

namespace ov
{

/** @brief Constructor */
dir::dir(lfs_t* lfs, const char* path) : m_lfs(lfs), m_dir{}, m_entry{}
{
    // Open the directory
    int err = lfs_dir_open(m_lfs, &m_dir, path);
    if (err != LFS_ERR_OK)
    {
        m_lfs = nullptr;
    }
}

/** @brief Move constructor */
dir::dir(dir&& move) : m_lfs(move.m_lfs), m_dir(move.m_dir), m_entry(move.m_entry)
{
    move.m_lfs = nullptr;
}

/** @brief Destructor */
dir::~dir()
{
    close();
}

/** @brief Close the directory */
bool dir::close()
{
    bool ret = false;

    if (is_open())
    {
        if (lfs_dir_close(m_lfs, &m_dir) == LFS_ERR_OK)
        {
            m_lfs = nullptr;
            ret   = true;
        }
    }

    return ret;
}

/** @brief Read the next entry */
bool dir::read(entry& info)
{
    bool ret = false;

    if (is_open())
    {
        if (lfs_dir_read(m_lfs, &m_dir, &m_entry) > 0)
        {
            info.type = static_cast<entry_type>(m_entry.type);
            info.size = m_entry.size;
            info.name = m_entry.name;
            ret       = true;
        }
    }

    return ret;
}

} // namespace ov
