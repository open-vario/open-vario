
#ifndef OV_DIR_H
#define OV_DIR_H

#include "lfs.h"

namespace ov
{

/** @brief Helper to list entries in a directory of the filesystem */
class dir
{
  public:
    /** @brief Type of directory entry */
    enum class entry_type : uint8_t
    {
        /** @brief File */
        file = LFS_TYPE_REG,
        /** @brief Directory */
        dir = LFS_TYPE_DIR
    };

    /** @brief Directory entry */
    struct entry
    {
        /** @brief Type of entry */
        entry_type type;
        /** @brief Size of the entry */
        uint32_t size;
        /** @brief Name of the entry as a null terminated string */
        const char* name;
    };

    /** @brief Constructor */
    dir(lfs_t* lfs, const char* path);
    /** @brief Copy constructor */
    dir(const dir& copy) = delete;
    /** @brief Move constructor */
    dir(dir&& move);

    /** @brief Destructor */
    ~dir();

    /** @brief Indicate if the directory is valid */
    bool is_open() { return (m_lfs != nullptr); }

    /** @brief Close the directory */
    bool close();

    /** @brief Read the next entry */
    bool read(entry& info);

  private:
    /** @brief Filesystem handle */
    lfs_t* m_lfs;
    /** @brief Directory handle */
    lfs_dir_t m_dir;
    /** @brief Directory entry */
    lfs_info m_entry;
};

} // namespace ov

#endif // OV_DIR_H
