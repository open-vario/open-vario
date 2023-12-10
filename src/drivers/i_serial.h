
#ifndef OV_I_SERIAL_H
#define OV_I_SERIAL_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Interface for serial port drivers implementations */
class i_serial
{
  public:
    /** @brief Destructor */
    virtual ~i_serial() { }

    /** @brief Read data from the serial port */
    virtual bool read(void* buffer, size_t size, uint32_t ms_timeout) = 0;

    /** @brief Write data to the serial port */
    virtual bool write(const void* buffer, size_t size) = 0;

    /** @brief Write a null-terminated string to the serial port */
    virtual bool write(const char* str) = 0;
};

} // namespace ov

#endif // OV_I_SERIAL_H
