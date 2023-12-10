
#ifndef OV_FS_CONFIG_H
#define OV_FS_CONFIG_H

#include <cstddef>

namespace ov
{
namespace fs
{

/** @brief Filesystem buffers size */
static constexpr size_t FS_BUFFERS_SIZE = 128u;

/** @brief File buffer size */
static constexpr size_t FILE_BUFFER_SIZE = 64u;

} // namespace fs
} // namespace ov

#endif // OV_FS_CONFIG_H
