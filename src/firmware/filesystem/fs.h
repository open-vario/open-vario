
#ifndef OV_FS_H
#define OV_FS_H

#include "dir.h"
#include "file.h"
#include "i_storage_memory.h"

namespace ov
{
namespace fs
{

/** @brief File open flags */
enum open_flags
{
    o_rdonly = LFS_O_RDONLY,
    o_wronly = LFS_O_WRONLY,
    o_rdwr   = LFS_O_RDWR,
    o_creat  = LFS_O_CREAT,
    o_excl   = LFS_O_EXCL,
    o_trunc  = LFS_O_TRUNC,
    o_append = LFS_O_APPEND
};

/** @brief Initialize the filesystem */
bool init(bool& fs_reinitialized, i_storage_memory& storage_memory);

/** @brief Get filesystem info */
bool info(size_t& free_space, size_t& total_space);

/** @brief Create a directory */
bool mkdir(const char* dir_path);

/** @brief Remove a file or a directory (must be empty) */
bool remove(const char* path);

/** @brief Rename a file or a directory */
bool rename(const char* old_path, const char* new_path);

/** @brief Open or create a file */
file open(const char* path, int flags);

/** @brief Open a directory */
dir open_dir(const char* path);

} // namespace fs
} // namespace ov

#endif // OV_FS_H
