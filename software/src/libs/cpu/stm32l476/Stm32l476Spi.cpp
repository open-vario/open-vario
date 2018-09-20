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

#include "Stm32l476Spi.h"
#include "Stm32l476Cpu.h"
#include "IOs.h"

#include "stm32l476xx.h"

#include <cstddef>

namespace open_vario
{


/** \brief Register definitions for the SPIs */
static SPI_TypeDef* const spi_regs[Stm32l476Spi::SPI_MAX] = {SPI1, SPI2, SPI3};

/** \brief SPI irq numbers */
static const IRQn_Type spi_irqs[Stm32l476Spi::SPI_MAX] = {SPI1_IRQn, SPI2_IRQn, SPI3_IRQn};

/** \brief SPI instances */
static Stm32l476Spi* spis[Stm32l476Spi::SPI_MAX] = {NULL, NULL, NULL};



/** \brief Constructor */
Stm32l476Spi::Stm32l476Spi(const Stm32l476Cpu& cpu, const Spi spi, const uint32_t frequency, const Polarity polarity, const Phase phase, ISpiChipSelect& cs)
: m_cpu(cpu)
, m_spi(spi)
, m_frequency(frequency)
, m_polarity(polarity)
, m_phase(phase)
, m_cs(cs)
, m_xfer(NULL)
, m_xfer_tx_index(0u)
, m_xfer_rx_index(0u)
, m_end_of_xfer(0u, 1u)
, m_mutex()
{
    // Register instance
    spis[spi] = this;
}

/** \brief Configure the SPI */
bool Stm32l476Spi::configure()
{
    bool ret = true;

    // Enable clock
    uint32_t input_clock_frequency = 0u;
    switch (m_spi)
    {
        case SPI_1:
        {
            RCC->APB2ENR |= (1u << 12u);
            input_clock_frequency = m_cpu.getPclk2Frequency();
            break;
        }

        case SPI_2:
        {
            RCC->APB1ENR1 |= (1u << 14u);
            input_clock_frequency = m_cpu.getPclk1Frequency();
            break;
        }

        case SPI_3:
        {
            RCC->APB1ENR1 |= (1u << 15u);
            input_clock_frequency = m_cpu.getPclk1Frequency();
            break;
        }

        default:
        {
            // Invalid SPI
            ret = false;
            break;
        }
    }
    if (ret)
    {
        // Configure the chip select driver
        ret = m_cs.configure();
    }
    if (ret)
    {
        // Get SPI registers
        SPI_TypeDef* const spi = spi_regs[m_spi];

        // Disable SPI
        spi->CR1 = 0u;
        spi->CR2 = 0u;

        // Configure frequency
        uint8_t br = 0;
        input_clock_frequency =  input_clock_frequency / 2;
        while (input_clock_frequency > m_frequency)
        {
            input_clock_frequency =  input_clock_frequency / 2;
            br++;
        }
        if (br > 7u)
        {
            ret = false;
        }
        spi->CR1 |= (br << 3u);

        // Configure polarity and phase
        if (m_polarity == POL_HIGH)
        {
            spi->CR1 |= (1u << 1u);
        }
        if (m_phase == PHA_SECOND)
        {
            spi->CR1 |= (1u << 0u);
        }

        // Configure transfer : master, 8 bits, msb first, motorola mode, no NSS pulse management
        spi->CR1 |= (1u << 2u) | (1u << 8u) | (1u << 9u);
        spi->CR2 |= (7u << 8u);

        // Configure FIFO threshold to one byte
        spi->CR2 |= (1u << 12u);

        // Enable Tx/Rx interrupt
        NVIC_EnableIRQ(spi_irqs[m_spi]);

        // Enable SPI
        spi->CR1 |= (1u << 6u);

    }

    return ret;
}

/** \brief Transfer data through the SPI */
bool Stm32l476Spi::xfer(const XFer& xfer)
{
    // Lock SPI bus
    m_mutex.lock();

    // Transfer loop
    m_xfer = &xfer;
    do
    {
        // Reset transfer informations
        m_xfer_tx_index = 0u;
        m_xfer_rx_index = 0u;

        // Enable peripheral selection
        m_cs.enable(m_xfer->cs);

        // Enable interrupt to start transfer
        SPI_TypeDef* const spi = spi_regs[m_spi];
        spi->CR2 |= (1u << 6u) | (1u << 7u);

        // Wait for end of transfer
        (void)m_end_of_xfer.wait(IOs::getInstance().getInfiniteTimeoutValue());

        // Disable peripheral selection
        if ((m_xfer->next == NULL) ||
            (!m_xfer->keep_cs_active))
        {
            m_cs.disable(m_xfer->cs);
        }

        // Next transfer
        m_xfer = m_xfer->next;
    }
    while (m_xfer != NULL);

    // Lock SPI bus
    m_mutex.unlock();

    return true;
}

/** \brief IRQ handler */
void Stm32l476Spi::irqHandler()
{
    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Enter();
    #endif // OS_NANO_OS

    SPI_TypeDef* const spi = spi_regs[m_spi];

    // Transmit data
    while ((m_xfer_tx_index < m_xfer->size) &&
            ((spi->SR & (1u << 1u)) != 0u))
    {
        if (m_xfer->write_data != NULL)
        {
            spi->DR = m_xfer->write_data[m_xfer_tx_index];
        }
        else
        {
            // Dummy write
            spi->DR = 0xAA;
        }
        m_xfer_tx_index++;
    }
    if (m_xfer_tx_index == m_xfer->size)
    {
        // Disable Tx interrupt
        spi->CR2 &= ~(1u << 7u);
    }

    // Receive data
    while ((m_xfer_rx_index < m_xfer->size) &&
            ((spi->SR & (1u << 0u)) != 0u))
    {
        if (m_xfer->read_data != NULL)
        {
            m_xfer->read_data[m_xfer_rx_index] = spi->DR;
        }
        else
        {
            // Dummy read
            spi->CRCPR = spi->DR;
        }
        m_xfer_rx_index++;
    }
    if (m_xfer_rx_index == m_xfer->size)
    {
        // Disable Rx interrupt
        spi->CR2 &= ~(1u << 6u);

        // End of transfer
        m_end_of_xfer.post(true);
    }

    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Exit();
    #endif // OS_NANO_OS
}


/** \brief IRQ handler for SPI 1 */
extern "C" void SPI1_IRQHandler(void)
{
    spis[Stm32l476Spi::SPI_1]->irqHandler();
}

/** \brief IRQ handler for SPI 2 */
extern "C" void SPI2_IRQHandler(void)
{
    spis[Stm32l476Spi::SPI_2]->irqHandler();
}

/** \brief IRQ handler for SPI 3 */
extern "C" void SPI3_IRQHandler(void)
{
    spis[Stm32l476Spi::SPI_3]->irqHandler();
}

}
