/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Stm32l476I2c.h"
#include "Stm32l476Cpu.h"
#include "IOs.h"

#include "stm32l476xx.h"

#include <cstddef>

namespace open_vario
{

/** \brief DMA channel for Rx */
const uint8_t Stm32l476I2c::RX_DMA_CHANNEL[] = {7u, 5u, 3u};

/** \brief DMA channel for Tx */
const uint8_t Stm32l476I2c::TX_DMA_CHANNEL[] = {6u, 4u, 2u};

/** \brief DMA request source */
const uint8_t Stm32l476I2c::DMA_REQ_SOURCE[] = {3u, 3u, 3u};



/** \brief Register definitions for the I2Cs */
static I2C_TypeDef* const i2c_regs[Stm32l476I2c::I2C_MAX] = {I2C1, I2C2, I2C3};

/** \brief I2C event irq numbers */
static const IRQn_Type i2c_evt_irqs[Stm32l476I2c::I2C_MAX] = {I2C1_EV_IRQn, I2C2_EV_IRQn, I2C3_EV_IRQn};

/** \brief I2C error irq numbers */
static const IRQn_Type i2c_err_irqs[Stm32l476I2c::I2C_MAX] = {I2C1_ER_IRQn, I2C2_ER_IRQn, I2C3_ER_IRQn};

/** \brief I2C instances */
static Stm32l476I2c* i2cs[Stm32l476I2c::I2C_MAX] = {nullptr, nullptr, nullptr};



/** \brief Constructor */
Stm32l476I2c::Stm32l476I2c(const Stm32l476Cpu& cpu, const I2c i2c, const uint32_t frequency, Stm32l476Dma& dma)
: m_cpu(cpu)
, m_i2c(i2c)
, m_frequency(frequency)
, m_dma(dma)
, m_xfer(nullptr)
, m_end_of_xfer(0u, 1u)
, m_mutex()
{
    // Register instance
    i2cs[m_i2c] = this;
}

/** \brief Configure the I2C */
bool Stm32l476I2c::configure()
{
    bool ret = true;

    // Enable clock
    switch (m_i2c)
    {
        case I2C_1:
        /* Intended fallthrough */
        case I2C_2:
        /* Intended fallthrough */
        case I2C_3:
        {
            // Select SYSCLK has clock source
            RCC->CCIPR &= ~(3u << (12u + 2u * static_cast<uint8_t>(m_i2c)));
            RCC->CCIPR |= (1u << (12u + 2u * static_cast<uint8_t>(m_i2c)));

            // Enable clock
            RCC->APB1ENR1 |= (1u << (21u + static_cast<uint8_t>(m_i2c)));
            break;
        }

        default:
        {
            // Invalid I2C
            ret = false;
            break;
        }
    }
    if (ret)
    {
        // Get I2C registers
        I2C_TypeDef* const i2c = i2c_regs[m_i2c];

        // Disable I2C
        i2c->CR1 = 0;
        i2c->CR2 = 0;

        // Configure frequency (based on a input clock of 48MHz)
        if (m_frequency == 100000u)
        {
            // Standard mode - 100kHz
            i2c->TIMINGR = 0xB0420F13;
        }
        else if (m_frequency == 400000u)
        {
            // Fast mode - 400kHz
            i2c->TIMINGR = 0x50330309;
        }
        else if (m_frequency == 1000000u)
        {
            // Fast mode Plus - 1000kHz
            i2c->TIMINGR = 0x50100103;
        }
        else
        {
            // Standard mode - 10kHz
            i2c->TIMINGR = 0xB042C3C7;
        }
        i2c->TIMEOUTR = 0;

        // Disable analog filter
        i2c->CR1 |= (1u << 12u);

        // Configure DMA
        Stm32l476Dma::ChannelConfig config;
        
        config.request = DMA_REQ_SOURCE[m_i2c];
        config.mem_2_mem = false;
        config.priority = Stm32l476Dma::PRIO_HIGH;
        config.memory_size = Stm32l476Dma::MEM_8;
        config.periph_size = Stm32l476Dma::MEM_8;
        config.mem_inc = true;
        config.periph_inc = false;
        config.circular = false;
        config.dir = Stm32l476Dma::DIR_PERIPH_2_MEM;
        ret = m_dma.configureChannel(RX_DMA_CHANNEL[m_i2c], config, *this);

        config.dir = Stm32l476Dma::DIR_MEM_2_PERIPH;
        ret = ret && m_dma.configureChannel(TX_DMA_CHANNEL[m_i2c], config, *this);
        if (ret)
        {
            // Enable I2C
            i2c->CR1 |= (1u << 0u);

            // Enable I2C error interrupts
            NVIC_EnableIRQ(i2c_err_irqs[m_i2c]);
            NVIC_EnableIRQ(i2c_evt_irqs[m_i2c]);
        }
    }

    return ret;
}

/** \brief Transfer data through the I2C */
bool Stm32l476I2c::xfer(const uint8_t slave_address, const XFer& xfer)
{
    I2cError error = II2c::I2C_SUCCESS;
    return this->xfer(slave_address, xfer, error);
}

/** \brief Transfer data through the I2C */
bool Stm32l476I2c::xfer(const uint8_t slave_address, const XFer& xfer, I2cError& error)
{
    // Lock I2C bus
    m_mutex.lock();

    I2C_TypeDef* const i2c = i2c_regs[m_i2c];

    // Wait bus free
    while ((i2c->ISR & (1u << 15u)) != 0)
    {}

    // Transfer loop
    m_xfer = &xfer;
    do
    {
        // Reset error flag
        m_error = II2c::I2C_SUCCESS;
        i2c->ICR = 0xFFFFFFFFu;

        // Configure DMA
        i2c->CR1 &= ~((1u << 14u) | (1u << 15u));
        /*
        if (m_xfer->size != 0)
        {
            if (m_xfer->read)
            {
                m_dma.startXfer(RX_DMA_CHANNEL[m_i2c], m_xfer->data, &i2c->RXDR, m_xfer->size);
            }
            else
            {
                m_dma.startXfer(TX_DMA_CHANNEL[m_i2c], m_xfer->data, &i2c->TXDR, m_xfer->size);
            }
        }
        */
        

        // Configure transfer
        uint32_t cr2_reg;
        cr2_reg = ((slave_address & 0x7Fu) << 1u) | (m_xfer->size << 16u);
        if (m_xfer->read)
        {
            cr2_reg |= (1u << 10u);
        }
        if (m_xfer->stop_cond && (m_xfer->size != 0))
        {
            cr2_reg |= (1u << 25u);
        }

        // Start condition
        cr2_reg |= (1u << 13u);
        i2c->CR2 = cr2_reg;

        // Enable DMA
        /*
        if (m_xfer->size != 0)
        {
            if (m_xfer->read)
            {
                i2c->CR1 |= (1u << 15u);
            }
            else
            {
                i2c->CR1 |= (1u << 14u);
            }
        }
        */

        // Enable error interrupts
        i2c->CR1 |= ((1u << 4u) | (1u << 7u));

        // Wait for end of transfer
        /*
        if (m_xfer->size != 0)
        {
            (void)m_end_of_xfer.wait(IOs::getInstance().getInfiniteTimeoutValue());
        }
        else
        */
        {
            uint8_t current_byte = 0;

            if (m_xfer->read)
            {
                do
                {
                    // Wait rx ready condition
                    while ((m_error == II2c::I2C_SUCCESS) &&
                        ((i2c->ISR & (1u << 2u)) == 0))
                    {}
                    if (m_error == II2c::I2C_SUCCESS)
                    {
                        m_xfer->data[current_byte] = i2c->RXDR;
                        current_byte++;
                    }
                }
                while ((m_error == II2c::I2C_SUCCESS) && (current_byte != m_xfer->size));

                // Wait end of transfer
                while ((i2c->ISR & (1u << 6u)) != 0)
                {}
            }
            else
            {
                do
                {
                    // Wait tx ready condition
                    while ((m_error == II2c::I2C_SUCCESS) &&
                        ((i2c->ISR & (1u << 1u)) == 0))
                    {}
                    if (m_error == II2c::I2C_SUCCESS)
                    {
                        i2c->TXDR = m_xfer->data[current_byte];
                        current_byte++;
                    }
                }
                while ((m_error == II2c::I2C_SUCCESS) && (current_byte != m_xfer->size));
            } 
        }

        // Wait stop condition
        if (m_xfer->stop_cond)
        {
            // Wait end of transfer
            while ((i2c->ISR & (1u << 5u)) == 0)
            {}
        }

        // Disable error interrupts
        i2c->CR1 &= ~((1u << 4u) | (1u << 7u));

        // Reset error flag
        i2c->ICR = 0xFFFFFFFFu;

        // Next transfer
        m_xfer = m_xfer->next;
    }
    while ((m_xfer != nullptr) && (m_error == II2c::I2C_SUCCESS));

    // Clear control register
    i2c->CR2 = 0;

    // Save error state
    error = m_error;

    // Unlock I2C bus
    m_mutex.unlock();

    return (error == II2c::I2C_SUCCESS);
}

/** \brief Called when the DMA transfer is done on a channel */
void Stm32l476I2c::onDmaComplete(const uint8_t channel, const bool success)
{
    if (!success)
    {
        m_error = II2c::I2C_ERR_OTHER;
    }
    m_end_of_xfer.post(true);
}


/** \brief IRQ handler */
void Stm32l476I2c::irqHandler()
{
    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Enter();
    #endif // OS_NANO_OS

    // Check error
    I2C_TypeDef* const i2c = i2c_regs[m_i2c];
    if ((i2c->ISR & (1u << 9u)) != 0)
    {
        m_error = II2c::I2C_ERR_ARB_LOST;
    }
    else if ((i2c->ISR & (1u << 8u)) != 0)
    {
        m_error = II2c::I2C_ERR_BUS_ERROR;
    }
    else if ((i2c->ISR & (1u << 4u)) != 0)
    {
        m_error = II2c::I2C_ERR_NACK;
    }
    else
    {
        m_error = II2c::I2C_ERR_OTHER;
    }

    // Abort DMA
    i2c->CR1 &= ~((1u << 14u) | (1u << 15u));

    // Reset error flag
    i2c->ICR = 0xFFFFFFFFu;

    // Stop condition
    i2c->CR2 |= (1u << 14u);

    // End of transfer
    m_end_of_xfer.post(true);

    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Exit();
    #endif // OS_NANO_OS
}


/** \brief IRQ handler for I2C 1 */
extern "C" void I2C1_EV_IRQHandler(void)
{
    i2cs[Stm32l476I2c::I2C_1]->irqHandler();
}

/** \brief IRQ handler for I2C 2 */
extern "C" void I2C2_EV_IRQHandler(void)
{
    i2cs[Stm32l476I2c::I2C_2]->irqHandler();
}

/** \brief IRQ handler for I2C 3 */
extern "C" void I2C3_EV_IRQHandler(void)
{
    i2cs[Stm32l476I2c::I2C_3]->irqHandler();
}

/** \brief IRQ handler for I2C 1 */
extern "C" void I2C1_ER_IRQHandler(void)
{
    i2cs[Stm32l476I2c::I2C_1]->irqHandler();
}

/** \brief IRQ handler for I2C 2 */
extern "C" void I2C2_ER_IRQHandler(void)
{
    i2cs[Stm32l476I2c::I2C_2]->irqHandler();
}

/** \brief IRQ handler for I2C 3 */
extern "C" void I2C3_ER_IRQHandler(void)
{
    i2cs[Stm32l476I2c::I2C_3]->irqHandler();
}

}
