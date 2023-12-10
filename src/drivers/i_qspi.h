
#ifndef OV_I_QSPI_H
#define OV_I_QSPI_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Interface for QSPI drivers implementations */
class i_qspi
{
  public:
    /** @brief Destructor */
    virtual ~i_qspi() { }

    /** @brief Address size */
    enum class address_size
    {
        /** @brief 8-bits */
        s_8bits,
        /** @brief 16-bits */
        s_16bits,
        /** @brief 24-bits */
        s_24bits,
        /** @brief 32-bits */
        s_32bits
    };

    /** @brief Line mode */
    enum class line_mode
    {
        /** @brief No transmission */
        none,
        /** @brief 1 line */
        l_1,
        /** @brief 2 lines */
        l_2,
        /** @brief 4 lines */
        l_4
    };

    /** @brief Command */
    struct command
    {
        /** @brief Command to send */
        uint8_t cmd;
        /** @brief Address to send */
        uint32_t address;
        /** @brief Address size */
        address_size addr_size;
        /** @brief Dummy cycles count */
        uint8_t dummy_cycles;
        /** @brief Address mode */
        line_mode addr_mode;
        /** @brief Data mode */
        line_mode data_mode;
    };

    /** @brief Execute a QSPI read command */
    virtual bool read(const command& cmd, void* buffer, size_t size) = 0;

    /** @brief Execute a QSPI write command */
    virtual bool write(const command& cmd, const void* buffer, size_t size) = 0;

    /** @brief Poll a register state */
    virtual bool poll(uint8_t cmd, uint8_t mask, uint8_t value, uint32_t ms_timeout) = 0;
};

} // namespace ov

#endif // OV_I_QSPI_H
