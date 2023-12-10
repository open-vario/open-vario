
#include "fs.h"
#include "mutex.h"

#include "lfs.h"

namespace ov
{
namespace fs
{

/** @brief Configuration of the filesystem */
static struct lfs_config s_lfs_cfg;
/** @brief Filesystem handle*/
static lfs_t s_lfs;
/** @brief Mutex for filesystem accesses */
static mutex s_lfs_mutex;

/** @brief Read buffer */
static uint8_t s_read_buffer[FS_CACHE_SIZE];
/** @brief Prog buffer */
static uint8_t s_prog_buffer[FS_CACHE_SIZE];
/** @brief Lookahead buffer */
static uint8_t s_lookahead_buffer[FS_CACHE_SIZE];

/** @brief Read a region in a block */
static int fs_read(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size);
/** @brief Program a region in a block */
static int fs_prog(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size);
/** @brief Erase a block */
static int fs_erase(const struct lfs_config* c, lfs_block_t block);
/** @brief Sync the state of the underlying block device */
static int fs_sync(const struct lfs_config* c);
/** @brief Lock the underlying block device */
static int fs_lock(const struct lfs_config*);
/** @brief Unlock the underlying block device */
static int fs_unlock(const struct lfs_config*);

/** @brief Initialize the filesystem */
bool init(bool& fs_reinitialized, i_storage_memory& storage_memory)
{
    /** @brief Configuration of the filesystem */
    s_lfs_cfg = {
        // Context
        .context = &storage_memory,

        // Block device operations
        .read  = fs_read,
        .prog  = fs_prog,
        .erase = fs_erase,
        .sync  = fs_sync,

        // Lock functions
        .lock   = fs_lock,
        .unlock = fs_unlock,

        // Block device configuration
        .read_size      = FS_CACHE_SIZE,
        .prog_size      = FS_CACHE_SIZE,
        .block_size     = storage_memory.get_block_size(),
        .block_count    = storage_memory.get_size() / storage_memory.get_block_size(),
        .block_cycles   = 500u,
        .cache_size     = FS_CACHE_SIZE,
        .lookahead_size = FS_CACHE_SIZE,

        // Buffers
        .read_buffer      = s_read_buffer,
        .prog_buffer      = s_prog_buffer,
        .lookahead_buffer = s_lookahead_buffer,

        // Limits
        .name_max     = 64u,
        .file_max     = LFS_FILE_MAX,                   // Keep default value
        .attr_max     = LFS_ATTR_MAX,                   // Keep default value
        .metadata_max = storage_memory.get_block_size() // Keep default value
    };

    // Mount the filesystem
    int err = lfs_mount(&s_lfs, &s_lfs_cfg);
    if (err < 0)
    {
        // Format filesystem
        lfs_format(&s_lfs, &s_lfs_cfg);
        fs_reinitialized = true;

        // Try to remount
        err = lfs_mount(&s_lfs, &s_lfs_cfg);
    }
    else
    {
        fs_reinitialized = false;
    }

    return (err == 0);
}

/** @brief Get filesystem info */
bool info(size_t& free_space, size_t& total_space)
{
    bool       ret    = false;
    lfs_fsinfo fsinfo = {};
    if (lfs_fs_stat(&s_lfs, &fsinfo) == LFS_ERR_OK)
    {
        lfs_ssize_t allocated_blocks = lfs_fs_size(&s_lfs);
        if (allocated_blocks > 0)
        {
            total_space = fsinfo.block_count * fsinfo.block_size;
            free_space  = total_space - allocated_blocks * fsinfo.block_size;
            ret         = true;
        }
    }
    return ret;
}

/** @brief Create a directory */
bool mkdir(const char* dir_path)
{
    return (lfs_mkdir(&s_lfs, dir_path) == LFS_ERR_OK);
}

/** @brief Remove a file or a directory (must be empty) */
bool remove(const char* path)
{
    return (lfs_remove(&s_lfs, path) == LFS_ERR_OK);
}

/** @brief Rename a file or a directory */
bool rename(const char* old_path, const char* new_path)
{
    return (lfs_rename(&s_lfs, old_path, new_path) == LFS_ERR_OK);
}

/** @brief Open or create a file */
file open(const char* path, int flags)
{
    return file(&s_lfs, path, flags);
}

/** @brief Open a directory */
dir open_dir(const char* path)
{
    return dir(&s_lfs, path);
}

/** @brief Read a region in a block */
static int fs_read(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size)
{
    i_storage_memory* storage_memory = reinterpret_cast<i_storage_memory*>(c->context);
    size_t            address        = block * storage_memory->get_block_size() + off;
    bool              ret            = storage_memory->read(address, buffer, size);
    return (ret ? LFS_ERR_OK : LFS_ERR_IO);
}

/** @brief Program a region in a block */
static int fs_prog(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size)
{
    i_storage_memory* storage_memory = reinterpret_cast<i_storage_memory*>(c->context);
    size_t            address        = block * storage_memory->get_block_size() + off;
    bool              ret            = storage_memory->write(address, buffer, size);
    return (ret ? LFS_ERR_OK : LFS_ERR_IO);
}

/** @brief Erase a block */
static int fs_erase(const struct lfs_config* c, lfs_block_t block)
{
    i_storage_memory* storage_memory = reinterpret_cast<i_storage_memory*>(c->context);
    bool              ret            = storage_memory->erase(block);
    return (ret ? LFS_ERR_OK : LFS_ERR_IO);
}

/** @brief Sync the state of the underlying block device */
static int fs_sync(const struct lfs_config*)
{
    // Nothing to do, everything is alread synced
    return LFS_ERR_OK;
}

/** @brief Lock the underlying block device */
static int fs_lock(const struct lfs_config*)
{
    s_lfs_mutex.lock();
    return 0;
}

/** @brief Unlock the underlying block device */
static int fs_unlock(const struct lfs_config*)
{
    s_lfs_mutex.unlock();
    return 0;
}

} // namespace fs
} // namespace ov
