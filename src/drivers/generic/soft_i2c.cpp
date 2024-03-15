/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "soft_i2c.h"

namespace ov
{

/** @brief Timeout to wait for idle state before starting a transfer in number of calls to the delay function */
static const uint32_t IDLE_WAIT_TIMEOUT = 100u;

/** @brief Constructor */
soft_i2c::soft_i2c(i_io_pin& scl_pin, i_io_pin& sda_pin, const delay_func& delay) : m_scl_pin(scl_pin), m_sda_pin(sda_pin), m_delay(delay)
{
}

/** @brief Initialize the driver */
bool soft_i2c::init()
{
    // Set the lines in idle state
    bool ret = set_idle();
    return ret;
}

/** @brief Transfer data through the I2C */
bool soft_i2c::xfer(const uint8_t slave_address, const xfer_desc& xfer)
{
    bool ret = false;

    // Wait idle state
    uint32_t retries = 0;
    while (!set_idle() && (retries < IDLE_WAIT_TIMEOUT))
    {
        m_delay();
        retries++;
    }
    if (set_idle())
    {
        // Transfers loop
        const xfer_desc* current_xfer = &xfer;
        bool             started      = false;
        do
        {
            // Start condition
            ret     = start(started);
            started = true;

            // Slave address
            ret = ret && send_byte(slave_address | (static_cast<uint8_t>(current_xfer->read)));

            // Transfer data
            if (current_xfer->read)
            {
                // Read loop
                for (uint8_t i = 0; (i < current_xfer->size) && ret; i++)
                {
                    ret = ret && receive_byte(current_xfer->data[i], (i != (current_xfer->size - 1u)));
                }
            }
            else
            {
                // Write loop
                for (uint8_t i = 0; (i < current_xfer->size) && ret; i++)
                {
                    ret = ret && send_byte(current_xfer->data[i]);
                }
            }

            // Stop condition
            if (current_xfer->stop_cond)
            {
                ret = stop() && ret;
            }

            // Next transfer
            current_xfer = current_xfer->next;
        } while (ret && (current_xfer != nullptr));
    }

    // Release bus
    set_idle();

    return ret;
}

/** @brief Set the lines in idle state */
bool soft_i2c::set_idle()
{
    // SDA => HIGH
    // SCL => HIGH
    m_sda_pin.set_high();
    m_scl_pin.set_high();

    // Check if the state is applied
    m_delay();
    bool ret = (m_scl_pin.is_high() && m_sda_pin.is_high());
    return ret;
}

/** @brief Generate a start condition */
bool soft_i2c::start(bool restart)
{
    // Restart condition
    if (restart)
    {
        // Set lines idle again in 2 bits time
        m_sda_pin.set_high();
        m_delay();
        m_scl_pin.set_high();
        m_delay();
    }

    // SDA goes from HIGH to LOW while SCL is still HIGH
    // Then SCL goes LOW too
    m_sda_pin.set_low();
    m_delay();
    m_scl_pin.set_low();
    m_delay();

    return true;
}

/** @brief Generate a stop condition */
bool soft_i2c::stop()
{
    // SDA goes from LOW to HIGH while SCL is still HIGH
    m_sda_pin.set_low();
    m_scl_pin.set_high();
    m_delay();
    m_sda_pin.set_high();
    m_delay();

    return true;
}

/** @brief Send a byte */
bool soft_i2c::send_byte(uint8_t byte)
{
    bool ret = true;

    // Send MSB first
    for (int i = 0; i < 8; i++)
    {
        // Send bit
        ret = ret && send_bit((byte & 0x80u) != 0u);

        // Next bit
        byte = byte << 1;
    }
    if (ret)
    {
        // Check acknowledge
        bool not_acknowledged = false;
        ret                   = receive_bit(not_acknowledged);
        ret                   = ret && !not_acknowledged;

        // Release SDA
        m_sda_pin.set_high();
    }

    return ret;
}

/** @brief Receive a byte */
bool soft_i2c::receive_byte(uint8_t& byte, bool ack)
{
    bool ret = true;

    // Receive MSB first
    byte     = 0;
    bool bit = false;
    for (int i = 0; ((i < 8) && ret); i++)
    {
        // Next bit
        byte = byte << 1;

        // Receive bit
        ret = ret && receive_bit(bit);

        // Update byte value
        if (bit)
        {
            byte |= 1u;
        }
    }
    if (ret)
    {
        // Write acknowledge
        ret = send_bit(!ack);

        // Release SDA
        m_sda_pin.set_high();
    }

    return ret;
}

/** @brief Send a bit */
bool soft_i2c::send_bit(bool bit)
{
    // Set bit on SDA
    if (bit)
    {
        m_sda_pin.set_high();
    }
    else
    {
        m_sda_pin.set_low();
    }

    // Generate clock on SCL
    m_scl_pin.set_high();
    m_delay();
    m_scl_pin.set_low();
    m_delay();

    return true;
}

/** @brief Receive a bit */
bool soft_i2c::receive_bit(bool& bit)
{
    // Release SDA
    m_sda_pin.set_high();

    // Generate clock
    m_scl_pin.set_high();
    m_delay();

    // Read bit
    bit = m_sda_pin.is_high();

    // Generate clock
    m_scl_pin.set_low();
    m_delay();

    return true;
}

} // namespace ov
