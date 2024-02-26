/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_STORAGE_MEMORY_H
#define OV_I_STORAGE_MEMORY_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Interface for block storage memories implementations */
class i_storage_memory
{
  public:
    /** @brief Destructor */
    virtual ~i_storage_memory() { }

    /** @brief Get the memory size in bytes */
    virtual size_t get_size() = 0;

    /** @brief Get the erase block size in bytes */
    virtual size_t get_block_size() = 0;

    /** @brief Reset the memory */
    virtual bool reset() = 0;

    /** @brief Read data from the memory */
    virtual bool read(size_t address, void* buffer, size_t size) = 0;

    /** @brief Write data to the memory */
    virtual bool write(size_t address, const void* buffer, size_t size) = 0;

    /** @brief Erase a block */
    virtual bool erase(size_t block) = 0;
};

} // namespace ov

#endif // OV_I_STORAGE_MEMORY_H
