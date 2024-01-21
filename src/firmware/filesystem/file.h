
#ifndef OV_FILE_H
#define OV_FILE_H

#include "fs_config.h"
#include "lfs.h"

namespace ov
{

/** @brief Helper to read/write on a file of the filesystem */
class file
{
  public:
    /** @brief Whence flags for a seek operation */
    enum seek_whence
    {
        /** @brief Seek relative to an absolute position */
        seek_set = LFS_SEEK_SET,
        /** @brief Seek relative to the current file position */
        seek_cur = LFS_SEEK_CUR,
        /** @brief Seek relative to the end of the file */
        seek_end = LFS_SEEK_END
    };

    /** @brief Constructor */
    file(lfs_t* lfs, const char* path, int flags);
    /** @brief Copy constructor */
    file(const file& copy) = delete;
    /** @brief Move constructor */
    file(file&& move);

    /** @brief Destructor */
    ~file();

    /** @brief Indicate if the file is valid */
    bool is_open() const { return (m_lfs != nullptr); }

    /** @brief Indicate if the file is valid */
    operator bool() const { return is_open(); }

    /** @brief Close the file */
    bool close();

    /** @brief Read data from the file */
    bool read(void* buffer, size_t size, size_t& read_count);

    /** @brief Write data to the file */
    bool write(const void* buffer, size_t size, size_t& write_count);

    /** @brief Read data from the file with size check */
    template <typename T>
    bool read(T& data)
    {
        size_t read_count = 0;
        bool   ret        = read(&data, sizeof(data), read_count);
        return (ret && (read_count == sizeof(data)));
    }

    /** @brief Write data to the file with size check */
    template <typename T>
    bool write(const T& data)
    {
        size_t write_count = 0;
        bool   ret         = write(&data, sizeof(data), write_count);
        return (ret && (write_count == sizeof(data)));
    }

    /** @brief Seek to a specific location in the file */
    bool seek(int32_t offset, seek_whence whence, int32_t& new_offset);

    /** @brief Force data synchornization on the storage medium */
    bool sync();

  private:
    /** @brief Filesystem handle */
    lfs_t* m_lfs;
    /** @brief File handle */
    lfs_file_t m_file;
    /** @brief File configuration */
    lfs_file_config m_config;
    /** @brief File buffer */
    uint8_t m_buffer[fs::FS_CACHE_SIZE];
};

} // namespace ov

#endif // OV_FILE_H
