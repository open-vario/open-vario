
#ifndef OV_SOFT_I2C_H
#define OV_SOFT_I2C_H

#include "delegate.h"
#include "i_i2c.h"
#include "i_io_pin.h"

namespace ov
{

/** @brief Software I2C driver */
class soft_i2c : public i_i2c
{
  public:
    /** @brief Delay function => bit time */
    using delay_func = delegate<void>;

    /** @brief Constructor */
    soft_i2c(i_io_pin& scl_pin, i_io_pin& sda_pin, const delay_func& delay);

    /** @brief Initialize the driver */
    bool init();

    /** @brief Transfer data through the I2C */
    bool xfer(const uint8_t slave_address, const xfer_desc& xfer) override;

  private:
    /** @brief SCL pin */
    i_io_pin& m_scl_pin;
    /** @brief SDA pin */
    i_io_pin& m_sda_pin;
    /** @brief Delay function */
    delay_func m_delay;

    /** @brief Set the lines in idle state */
    bool set_idle();
    /** @brief Generate a start condition */
    bool start(bool restart);
    /** @brief Generate a stop condition */
    bool stop();
    /** @brief Send a byte */
    bool send_byte(uint8_t byte);
    /** @brief Receive a byte */
    bool receive_byte(uint8_t& byte, bool ack);
    /** @brief Send a bit */
    bool send_bit(bool bit);
    /** @brief Receive a bit */
    bool receive_bit(bool& bit);
};

} // namespace ov

#endif // OV_SOFT_I2C_H
