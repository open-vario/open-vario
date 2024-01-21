
#include "s25flxxxs.h"

namespace ov
{

/** @brief Constructor */
s25flxxxs::s25flxxxs(ref reference, i_qspi& qspi) : m_reference(reference), m_qspi(qspi) { }

/** @brief Get the memory size in bytes */
size_t s25flxxxs::get_size()
{
    // 16MB and 32MB
    // Skip the first 128kBytes which are only 4kBytes sectors
    static constexpr size_t sizes[] = {0x1000000u - MEMSKIP_OFFSET, 0x2000000u - MEMSKIP_OFFSET};
    return sizes[static_cast<int>(m_reference)];
}

/** @brief Get the erase block size in bytes */
size_t s25flxxxs::get_block_size()
{
    return 0x10000u; // 64kB
}

/** @brief Reset the memory */
bool s25flxxxs::reset()
{
    // Prepare command
    i_qspi::command cmd = {};

    cmd.cmd       = 0xF0u;
    cmd.addr_mode = i_qspi::line_mode::none;
    cmd.data_mode = i_qspi::line_mode::none;

    // Execute command
    bool ret = m_qspi.write(cmd, nullptr, 0u);
    if (ret)
    {
        // Wait for completion
        ret = wait_ready();
    }

    return ret;
}

/** @brief Read data from the memory */
bool s25flxxxs::read(size_t address, void* buffer, size_t size)
{
    bool ret = false;

    // Check memory address
    if ((address + size) < get_size())
    {
        // Prepare command
        i_qspi::command cmd = {};

        cmd.cmd          = 0x6Bu;
        cmd.address      = address + MEMSKIP_OFFSET;
        cmd.addr_mode    = i_qspi::line_mode::l_1;
        cmd.addr_size    = i_qspi::address_size::s_24bits;
        cmd.data_mode    = i_qspi::line_mode::l_4;
        cmd.dummy_cycles = 8u;

        // Execute command
        ret = m_qspi.read(cmd, buffer, size);
    }

    return ret;
}

/** @brief Write data to the memory */
bool s25flxxxs::write(size_t address, const void* buffer, size_t size)
{
    static constexpr size_t page_size = 0x100u; // 256B

    bool ret = false;

    // Check memory address
    if ((address + size) < get_size())
    {
        size_t         written         = 0;
        size_t         current_address = address + MEMSKIP_OFFSET;
        size_t         bytes_to_write  = page_size - (address % page_size);
        const uint8_t* u8_buffer       = reinterpret_cast<const uint8_t*>(buffer);
        ret                            = true;
        while (ret && (written != size))
        {
            // Check data size to write
            if (bytes_to_write > (size - written))
            {
                bytes_to_write = size - written;
            }

            // Prepare command
            i_qspi::command cmd = {};

            cmd.cmd       = 0x32u;
            cmd.address   = current_address;
            cmd.addr_mode = i_qspi::line_mode::l_1;
            cmd.addr_size = i_qspi::address_size::s_24bits;
            cmd.data_mode = i_qspi::line_mode::l_4;

            // Enable write operations
            ret = enable_write();
            if (ret)
            {
                // Execute command
                ret = m_qspi.write(cmd, u8_buffer, bytes_to_write);
                if (ret)
                {
                    // Wait for completion
                    ret = wait_ready();
                }
            }

            // Next page
            written += bytes_to_write;
            current_address += bytes_to_write;
            u8_buffer += bytes_to_write;
            bytes_to_write = page_size;
        }
    }

    return ret;
}

/** @brief Erase a block */
bool s25flxxxs::erase(size_t block)
{
    bool ret = false;

    // Compute memory address
    uint32_t address = block * get_block_size();
    if (address < get_size())
    {
        // Prepare command
        i_qspi::command cmd = {};

        cmd.cmd       = 0xD8u;
        cmd.address   = address + MEMSKIP_OFFSET;
        cmd.addr_mode = i_qspi::line_mode::l_1;
        cmd.addr_size = i_qspi::address_size::s_24bits;
        cmd.data_mode = i_qspi::line_mode::none;

        // Enable write operations
        ret = enable_write();
        if (ret)
        {
            // Execute command
            ret = m_qspi.write(cmd, nullptr, 0u);
            if (ret)
            {
                // Wait for completion
                ret = wait_ready();
            }
        }
    }

    return ret;
}

/** @brief Wait for the memory to be ready for another command */
bool s25flxxxs::wait_ready()
{
    // Wait for the WIP flag to be cleared in the Status Register-1
    return m_qspi.poll(0x05u, 0x01u, 0x00u, 1000u);
}

/** @brief Put the memory in write mode */
bool s25flxxxs::enable_write()
{
    // Prepare command
    i_qspi::command cmd = {};

    cmd.cmd       = 0x06u;
    cmd.addr_mode = i_qspi::line_mode::none;
    cmd.data_mode = i_qspi::line_mode::none;

    // Execute command
    bool ret = m_qspi.write(cmd, nullptr, 0u);
    if (ret)
    {
        // Wait for completion => WEL bit set in the Status Register-1
        ret = m_qspi.poll(0x05u, 0x02u, 0x02u, 1000u);
    }

    return ret;
}

} // namespace ov
