
#include "stm32hal_i2c.h"
#include "os.h"
#include "thread.h"

namespace ov
{

/** @brief HAL I2C handles */
static I2C_HandleTypeDef* s_i2cs[2u];

/** @brief Constructor */
stm32hal_i2c::stm32hal_i2c(I2C_TypeDef* instance) : m_i2c{}, m_xfer(nullptr), m_error(i_i2c::error::success)
{
    // Save parameters
    m_i2c.Instance = instance;
    m_i2c.user     = this;
    if (m_i2c.Instance == I2C1)
    {
        s_i2cs[0u] = &m_i2c;
    }
    else
    {
        s_i2cs[1u] = &m_i2c;
    }
}

/** @brief Initialize the driver */
bool stm32hal_i2c::init()
{
    bool ret = false;

    // Enable clock
    if (m_i2c.Instance == I2C1)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
    else
    {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }

    // Configure device
    m_i2c.Init.Timing           = 0x60702729u; // Fixed rate for now => 100kbit/s
    m_i2c.Init.OwnAddress1      = 0;
    m_i2c.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    m_i2c.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    m_i2c.Init.OwnAddress2      = 0;
    m_i2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    m_i2c.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    m_i2c.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&m_i2c) == HAL_OK)
    {
        // Enable interrupts
        if (m_i2c.Instance == I2C1)
        {
            HAL_NVIC_SetPriority(I2C1_EV_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
            HAL_NVIC_SetPriority(I2C1_ER_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
        }
        else
        {
            HAL_NVIC_SetPriority(I2C3_EV_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
            HAL_NVIC_SetPriority(I2C3_ER_IRQn, 15u, 0u);
            HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
        }

        ret = true;
    }

    return ret;
}

/** @brief Transfer data through the I2C */
bool stm32hal_i2c::xfer(const uint8_t slave_address, const xfer_desc& xfer)
{
    I2C_TypeDef* const i2c = m_i2c.Instance;

    // Wait bus free
    while ((i2c->ISR & (1u << 15u)) != 0)
    {
    }

    // Transfer loop
    m_xfer = &xfer;
    do
    {
        // Reset error flag
        m_error  = i_i2c::error::success;
        i2c->ICR = 0xFFFFFFFFu;

        // Configure transfer
        uint32_t cr2_reg;
        cr2_reg = (slave_address << 0u) | (m_xfer->size << 16u);
        if (m_xfer->read)
        {
            cr2_reg |= (1u << 10u);
        }
        if (m_xfer->stop_cond && (m_xfer->size != 0))
        {
            cr2_reg |= (1u << 25u);
        }
        else
        {
            cr2_reg |= (1u << 24u);
        }

        // Start condition
        cr2_reg |= (1u << 13u);
        i2c->CR2 = cr2_reg;

        // Enable error interrupts
        i2c->CR1 |= ((1u << 4u) | (1u << 7u));

        // Wait for end of transfer
        uint8_t current_byte = 0;
        if (m_xfer->read)
        {
            do
            {
                // Wait rx ready condition
                while ((m_error == i_i2c::error::success) && ((i2c->ISR & (1u << 2u)) == 0))
                {
                }
                if (m_error == i_i2c::error::success)
                {
                    m_xfer->data[current_byte] = i2c->RXDR;
                    current_byte++;
                }
            } while ((m_error == i_i2c::error::success) && (current_byte != m_xfer->size));

            // Wait end of transfer
            while ((i2c->ISR & (1u << 6u)) != 0)
            {
            }
        }
        else
        {
            do
            {
                // Wait tx ready condition
                while ((m_error == i_i2c::error::success) && ((i2c->ISR & (1u << 1u)) == 0))
                {
                }
                if (m_error == i_i2c::error::success)
                {
                    i2c->TXDR = m_xfer->data[current_byte];
                    current_byte++;
                }
            } while ((m_error == i_i2c::error::success) && (current_byte != m_xfer->size));
        }

        // Wait stop condition
        if (m_xfer->stop_cond)
        {
            const uint32_t start_time = os::now();
            while (((i2c->ISR & (1u << 5u)) == 0) && ((os::now() - start_time) <= 100u))
            {
            }
        }

        // Disable error interrupts
        i2c->CR1 &= ~((1u << 4u) | (1u << 7u));

        // Reset error flag
        i2c->ICR = 0xFFFFFFFFu;

        // Next transfer
        m_xfer = m_xfer->next;
    } while ((m_xfer != nullptr) && (m_error == i_i2c::error::success));

    // Clear control register
    //i2c->CR2 = 0;

    return (m_error == i_i2c::error::success);
}

/** @brief IRQ handler */
void stm32hal_i2c::irq_handler()
{
    // Check error
    I2C_TypeDef* const i2c = m_i2c.Instance;
    if ((i2c->ISR & (1u << 9u)) != 0)
    {
        m_error = i_i2c::error::arbitration_lost;
    }
    else if ((i2c->ISR & (1u << 8u)) != 0)
    {
        m_error = i_i2c::error::bus_error;
    }
    else if ((i2c->ISR & (1u << 4u)) != 0)
    {
        m_error = i_i2c::error::nack;
    }
    else
    {
        m_error = i_i2c::error::other;
    }

    // Abort DMA
    i2c->CR1 &= ~((1u << 14u) | (1u << 15u));

    // Reset error flag
    i2c->ICR = 0xFFFFFFFFu;

    // Stop condition
    i2c->CR2 |= (1u << 14u);
}

} // namespace ov

/** @brief IRQ handler for I2C 1 */
extern "C" void I2C1_EV_IRQHandler(void)
{
    ov::stm32hal_i2c* i2c_drv = reinterpret_cast<ov::stm32hal_i2c*>(ov::s_i2cs[0u]->user);
    i2c_drv->irq_handler();
}

/** @brief IRQ handler for I2C 3 */
extern "C" void I2C3_EV_IRQHandler(void)
{
    ov::stm32hal_i2c* i2c_drv = reinterpret_cast<ov::stm32hal_i2c*>(ov::s_i2cs[1u]->user);
    i2c_drv->irq_handler();
}

/** @brief IRQ handler for I2C 1 */
extern "C" void I2C1_ER_IRQHandler(void)
{
    ov::stm32hal_i2c* i2c_drv = reinterpret_cast<ov::stm32hal_i2c*>(ov::s_i2cs[0u]->user);
    i2c_drv->irq_handler();
}

/** @brief IRQ handler for I2C 3 */
extern "C" void I2C3_ER_IRQHandler(void)
{
    ov::stm32hal_i2c* i2c_drv = reinterpret_cast<ov::stm32hal_i2c*>(ov::s_i2cs[1u]->user);
    i2c_drv->irq_handler();
}
