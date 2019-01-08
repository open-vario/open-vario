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

#include "Stm32l476Dma.h"

#include "stm32l476xx.h"

#ifdef OS_NANO_OS
#include "nano_os_api.h"
#endif // OS_NANO_OS


#include <cstddef>

namespace open_vario
{


/** \brief Register definitions for the DMAs */
static DMA_TypeDef* const dma_regs[Stm32l476Dma::DMA_MAX] = {DMA1, DMA2};

/** \brief Register definitions for the DMA channels */
static DMA_Channel_TypeDef* const dma_channel_regs[Stm32l476Dma::DMA_MAX][Stm32l476Dma::MAX_DMA_CHANNEL_COUNT] = 
                                                            {
                                                                {
                                                                    DMA1_Channel1,
                                                                    DMA1_Channel2,
                                                                    DMA1_Channel3,
                                                                    DMA1_Channel4,
                                                                    DMA1_Channel5,
                                                                    DMA1_Channel6,
                                                                    DMA1_Channel7
                                                                },
                                                                {
                                                                    DMA2_Channel1,
                                                                    DMA2_Channel2,
                                                                    DMA2_Channel3,
                                                                    DMA2_Channel4,
                                                                    DMA2_Channel5,
                                                                    DMA2_Channel6,
                                                                    DMA2_Channel7
                                                                }
                                                            };

/** \brief Register definitions for the DMA requests */
static DMA_Request_TypeDef* const dma_req_regs[Stm32l476Dma::DMA_MAX] = {DMA1_CSELR, DMA2_CSELR};

/** \brief DMA irq numbers */
static const IRQn_Type dma_irqs[Stm32l476Dma::DMA_MAX][Stm32l476Dma::MAX_DMA_CHANNEL_COUNT] = 
                                                            {
                                                                {
                                                                    DMA1_Channel1_IRQn,
                                                                    DMA1_Channel2_IRQn,
                                                                    DMA1_Channel3_IRQn,
                                                                    DMA1_Channel4_IRQn,
                                                                    DMA1_Channel5_IRQn,
                                                                    DMA1_Channel6_IRQn,
                                                                    DMA1_Channel7_IRQn
                                                                },
                                                                {
                                                                    DMA2_Channel1_IRQn,
                                                                    DMA2_Channel2_IRQn,
                                                                    DMA2_Channel3_IRQn,
                                                                    DMA2_Channel4_IRQn,
                                                                    DMA2_Channel5_IRQn,
                                                                    DMA2_Channel6_IRQn,
                                                                    DMA2_Channel7_IRQn
                                                                }
                                                            };

/** \brief DMA instances */
static Stm32l476Dma* dmas[Stm32l476Dma::DMA_MAX] = {nullptr, nullptr};



/** \brief Constructor */
Stm32l476Dma::Stm32l476Dma(const Dma dma)
: m_dma(dma)
, m_listeners()
{
    // Register instance
    dmas[dma] = this;
}

/** \brief Configure the DMA */
bool Stm32l476Dma::configure()
{
    bool ret = true;

    // Enable clock
    switch (m_dma)
    {
        case DMA_1:
        {
            RCC->AHB1ENR |= (1u << 0u);
            break;
        }

        case DMA_2:
        {
            RCC->AHB1ENR |= (1u << 1u);
            break;
        }

        default:
        {
            // Invalid DMA
            ret = false;
            break;
        }
    }

    return ret;
}

/** \brief Configure a DMA channel */
bool Stm32l476Dma::configureChannel(const uint8_t channel, const ChannelConfig config, IStm32l476DmaListener& listener)
{
    bool ret = false;

    // Check channel number
    if ((channel > 0u) && (channel <= MAX_DMA_CHANNEL_COUNT))
    {
        // Configure request
        DMA_Request_TypeDef* const dma_req = dma_req_regs[m_dma];
        dma_req->CSELR |= ((config.request & 0x07u) << ((channel - 1u) * 4u));


        // Configure channel
        DMA_Channel_TypeDef* const dma_channel = dma_channel_regs[m_dma][channel - 1u];
        dma_channel->CCR = 0u;
        if (config.mem_2_mem)
        {
            dma_channel->CCR |= (1u << 14u);
        }
        dma_channel->CCR |= ((config.priority & 0x03u) << 12u);
        dma_channel->CCR |= ((config.memory_size & 0x03u) << 10u);
        dma_channel->CCR |= ((config.periph_size & 0x03u) << 8u);
        if (config.mem_inc)
        {
            dma_channel->CCR |= (1u << 7u);
        }
        if (config.periph_inc)
        {
            dma_channel->CCR |= (1u << 6u);
        }
        if (config.circular)
        {
            dma_channel->CCR |= (1u << 5u);
        }
        dma_channel->CCR |= ((config.dir & 0x01u) << 4u);

        // Enable transfer complete and error interrupts
        dma_channel->CCR |= (1u << 3u) | (1u << 1u);
        NVIC_EnableIRQ(dma_irqs[m_dma][channel - 1u]);

        // Save listener
        m_listeners[channel - 1u] = &listener;

        ret = true;
    }
 
    return ret;
}

/** \brief Start a DMA transfer on a channel */
bool Stm32l476Dma::startXfer(const uint8_t channel, const volatile void* mem_address, const volatile void* periph_address, const uint16_t size)
{
    bool ret = false;

    // Check channel number
    if ((channel > 0u) && (channel <= MAX_DMA_CHANNEL_COUNT))
    {
        DMA_TypeDef* const dma = dma_regs[m_dma]; 
        DMA_Channel_TypeDef* const dma_channel = dma_channel_regs[m_dma][channel - 1u];

        // Configure transfer
        dma_channel->CCR &= ~(1u << 0u);
        dma_channel->CMAR = reinterpret_cast<uint32_t>(mem_address);
        dma_channel->CPAR = reinterpret_cast<uint32_t>(periph_address);
        dma_channel->CNDTR = size;

        // Clear interrupt flags
        dma->IFCR = (0x0F << ((channel - 1u) * 4u));

        // Start transfer
        dma_channel->CCR |= (1u << 0u);

        ret = true;
    }

    return ret;
}

/** \brief IRQ handler */
void Stm32l476Dma::irqHandler(const uint8_t channel)
{
    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Enter();
    #endif // OS_NANO_OS

    DMA_TypeDef* const dma = dma_regs[m_dma];

    // Call listener
    m_listeners[channel - 1u]->onDmaComplete(channel, ((0x08 << ((channel - 1u) * 4u)) == 0u));

    // Clear interrupt flags
    dma->IFCR = (0x0F << ((channel - 1u) * 4u));

    #ifdef OS_NANO_OS
    NANO_OS_INTERRUPT_Exit();
    #endif // OS_NANO_OS
}


/** \brief IRQ handler for DMA 1 Channel 1 */
extern "C" void DMA1_Channel1_IRQHandler(void)
{
    dmas[0u]->irqHandler(1u);
}

/** \brief IRQ handler for DMA 1 Channel 2 */
extern "C" void DMA1_Channel2_IRQHandler(void)
{
    dmas[0u]->irqHandler(2u);
}

/** \brief IRQ handler for DMA 1 Channel 3 */
extern "C" void DMA1_Channel3_IRQHandler(void)
{
    dmas[0u]->irqHandler(3u);
}

/** \brief IRQ handler for DMA 1 Channel 4 */
extern "C" void DMA1_Channel4_IRQHandler(void)
{
    dmas[0u]->irqHandler(4u);
}

/** \brief IRQ handler for DMA 1 Channel 5 */
extern "C" void DMA1_Channel5_IRQHandler(void)
{
    dmas[0u]->irqHandler(5u);
}

/** \brief IRQ handler for DMA 1 Channel 6 */
extern "C" void DMA1_Channel6_IRQHandler(void)
{
    dmas[0u]->irqHandler(6u);
}

/** \brief IRQ handler for DMA 1 Channel 7 */
extern "C" void DMA1_Channel7_IRQHandler(void)
{
    dmas[0u]->irqHandler(7u);
}

/** \brief IRQ handler for DMA 2 Channel 1 */
extern "C" void DMA2_Channel1_IRQHandler(void)
{
    dmas[1u]->irqHandler(1u);
}

/** \brief IRQ handler for DMA 2 Channel 2 */
extern "C" void DMA2_Channel2_IRQHandler(void)
{
    dmas[1u]->irqHandler(2u);
}

/** \brief IRQ handler for DMA 2 Channel 3 */
extern "C" void DMA2_Channel3_IRQHandler(void)
{
    dmas[1u]->irqHandler(3u);
}

/** \brief IRQ handler for DMA 2 Channel 4 */
extern "C" void DMA2_Channel4_IRQHandler(void)
{
    dmas[1u]->irqHandler(4u);
}

/** \brief IRQ handler for DMA 2 Channel 5 */
extern "C" void DMA2_Channel5_IRQHandler(void)
{
    dmas[1u]->irqHandler(5u);
}

/** \brief IRQ handler for DMA 2 Channel 6 */
extern "C" void DMA2_Channel6_IRQHandler(void)
{
    dmas[1u]->irqHandler(6u);
}

/** \brief IRQ handler for DMA 2 Channel 7 */
extern "C" void DMA2_Channel7_IRQHandler(void)
{
    dmas[1u]->irqHandler(7u);
}

}
