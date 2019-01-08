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

/** \brief DMA channel for Rx */
const uint8_t Stm32l476Spi::RX_DMA_CHANNEL[] = {2u, 4u, 1u};

/** \brief DMA channel for Tx */
const uint8_t Stm32l476Spi::TX_DMA_CHANNEL[] = {3u, 5u, 2u};

/** \brief DMA request source */
const uint8_t Stm32l476Spi::DMA_REQ_SOURCE[] = {1u, 1u, 3u};



/** \brief Register definitions for the SPIs */
static SPI_TypeDef* const spi_regs[Stm32l476Spi::SPI_MAX] = {SPI1, SPI2, SPI3};

/** \brief SPI irq numbers */
static const IRQn_Type spi_irqs[Stm32l476Spi::SPI_MAX] = {SPI1_IRQn, SPI2_IRQn, SPI3_IRQn};



/** \brief Constructor */
Stm32l476Spi::Stm32l476Spi(const Stm32l476Cpu& cpu, const Spi spi, const uint32_t frequency, const Polarity polarity, const Phase phase, ISpiChipSelect& cs, Stm32l476Dma& dma)
: m_cpu(cpu)
, m_spi(spi)
, m_frequency(frequency)
, m_polarity(polarity)
, m_phase(phase)
, m_cs(cs)
, m_dma(dma)
, m_xfer(nullptr)
, m_end_of_xfer(0u, 1u)
, m_mutex()
, m_cs_active(false)
, m_last_cs(0u)
{}

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

        // Configure DMA
        Stm32l476Dma::ChannelConfig config;
        
        config.request = DMA_REQ_SOURCE[m_spi];
        config.mem_2_mem = false;
        config.priority = Stm32l476Dma::PRIO_HIGH;
        config.memory_size = Stm32l476Dma::MEM_8;
        config.periph_size = Stm32l476Dma::MEM_8;
        config.mem_inc = true;
        config.periph_inc = false;
        config.circular = false;
        config.dir = Stm32l476Dma::DIR_PERIPH_2_MEM;
        ret = m_dma.configureChannel(RX_DMA_CHANNEL[m_spi], config, *this);

        config.dir = Stm32l476Dma::DIR_MEM_2_PERIPH;
        ret = ret && m_dma.configureChannel(TX_DMA_CHANNEL[m_spi], config, *this);
    }

    return ret;
}

/** \brief Transfer data through the SPI */
bool Stm32l476Spi::xfer(const XFer& xfer)
{
    // Lock SPI bus
    m_mutex.lock();

    // Unlock the bus if the chip select has been maintained active between calls
    if (m_cs_active)
    {
        m_mutex.unlock();
    }

    // Transfer loop
    m_xfer = &xfer;
    do
    {
        // Enable peripheral selection
        m_cs.enable(m_xfer->cs);

        // Configure DMA
        SPI_TypeDef* const spi = spi_regs[m_spi];
        if (m_xfer->read_data != nullptr)
        {
            spi->CR2 |= (1u << 0u);
            m_dma.startXfer(RX_DMA_CHANNEL[m_spi], m_xfer->read_data, &spi->DR, m_xfer->size);
        }
        if (m_xfer->write_data != nullptr)
        {
            m_dma.startXfer(TX_DMA_CHANNEL[m_spi], m_xfer->write_data, &spi->DR, m_xfer->size);
        }        
        else
        {
            m_dma.startXfer(TX_DMA_CHANNEL[m_spi], m_xfer->read_data, &spi->DR, m_xfer->size);
        }
        spi->CR2 |= (1u << 1u);

        // Enable SPI to start transfer
        spi->CR1 |= (1u << 6u);

        // Wait for end of transfer
        (void)m_end_of_xfer.wait(IOs::getInstance().getInfiniteTimeoutValue());

        // Ensure that transfer is complete (BSY flag)
        while ((spi->SR & (1u << 7u)) != 0u)
        {}

        // Disable peripheral selection
        if (!m_xfer->keep_cs_active)
        {
            m_cs.disable(m_xfer->cs);
            m_cs_active = false;
        }
        else
        {
            m_cs_active = true;
            m_last_cs = m_xfer->cs;
        }

        // Disable SPI communication
        spi->CR1 &= ~(1u << 6u);
        spi->CR2 &= ~(1u << 0u);
        spi->CR2 &= ~(1u << 1u);
        while ((spi->SR & (1u << 0)) != 0u)
        {
            // Dummy read to flush rx FIFO
            const uint8_t dummy = spi->DR;
        }

        // Next transfer
        m_xfer = m_xfer->next;
    }
    while (m_xfer != nullptr);

    // Unlock SPI bus only if chip select is not active
    if (!m_cs_active)
    {
        m_mutex.unlock();
    }

    return true;
}

/** \brief Release chip select if it has been maintened active after a call to xfer() methode */
bool Stm32l476Spi::releaseCs()
{
    bool ret = false;

    // Unlock the bus if the chip select has been maintained active between calls
    if (m_cs_active)
    {
        m_cs.disable(m_last_cs);
        m_cs_active = false;
        ret = m_mutex.unlock();
    }

    return ret;
}

/** \brief Called when the DMA transfer is done on a channel */
void Stm32l476Spi::onDmaComplete(const uint8_t channel, const bool success)
{
    (void)success;

    // Check end of transfer:
    // - End of RX DMA request
    // or
    // - If write only, end of TX DMA request
    if (m_xfer->read_data == nullptr)
    {
        m_end_of_xfer.post(true);
    }
    else
    {
        if (channel == RX_DMA_CHANNEL[m_spi])
        {
            m_end_of_xfer.post(true);
        }
    }
}


}
