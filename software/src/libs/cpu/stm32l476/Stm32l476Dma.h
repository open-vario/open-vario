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

#ifndef STM32L476DMA_H
#define STM32L476DMA_H


#include <cstdint>

namespace open_vario
{


/** \brief IRQ handler for DMA 1 Channel 1 */
extern "C" void DMA1_Channel1_IRQHandler(void);
/** \brief IRQ handler for DMA 1 Channel 2 */
extern "C" void DMA1_Channel2_IRQHandler(void);
/** \brief IRQ handler for DMA 1 Channel 3 */
extern "C" void DMA1_Channel3_IRQHandler(void);
/** \brief IRQ handler for DMA 1 Channel 4 */
extern "C" void DMA1_Channel4_IRQHandler(void);
/** \brief IRQ handler for DMA 1 Channel 5 */
extern "C" void DMA1_Channel5_IRQHandler(void);
/** \brief IRQ handler for DMA 1 Channel 6 */
extern "C" void DMA1_Channel6_IRQHandler(void);
/** \brief IRQ handler for DMA 1 Channel 7 */
extern "C" void DMA1_Channel7_IRQHandler(void);

/** \brief IRQ handler for DMA 2 Channel 1 */
extern "C" void DMA2_Channel1_IRQHandler(void);
/** \brief IRQ handler for DMA 2 Channel 2 */
extern "C" void DMA2_Channel2_IRQHandler(void);
/** \brief IRQ handler for DMA 2 Channel 3 */
extern "C" void DMA2_Channel3_IRQHandler(void);
/** \brief IRQ handler for DMA 2 Channel 4 */
extern "C" void DMA2_Channel4_IRQHandler(void);
/** \brief IRQ handler for DMA 2 Channel 5 */
extern "C" void DMA2_Channel5_IRQHandler(void);
/** \brief IRQ handler for DMA 2 Channel 6 */
extern "C" void DMA2_Channel6_IRQHandler(void);
/** \brief IRQ handler for DMA 2 Channel 7 */
extern "C" void DMA2_Channel7_IRQHandler(void);


/** \brief Interface for all the STM32L476 DMA listeners implementations */
class IStm32l476DmaListener
{
    public:

        /** \brief Called when the DMA transfer is done on a channel */
        virtual void onDmaComplete(const uint8_t channel, const bool success) = 0;
};



/** \brief STM32L476 DMA */
class Stm32l476Dma
{
    public:


        /** \brief DMA number */
        enum Dma
        {
            /** \brief DMA 1 */
            DMA_1 = 0u,
            /** \brief DMA 2 */
            DMA_2 = 1u,

            /** \brief Max DMA value (do not use) */
            DMA_MAX
        };

        /** \brief Maximum number of DMA channels */
        static const uint32_t MAX_DMA_CHANNEL_COUNT = 7u;


        /** \brief DMA priority level */
        enum Priority
        {
            /** \brief Low */
            PRIO_LOW = 0u,
            /** \brief Medium */
            PRIO_MEDIUM = 1u,
            /** \brief High */
            PRIO_HIGH = 2u,
            /** \brief Very high */
            PRIO_VERY_HIGH = 3u,

            /** \brief Max priority value (do not use) */
            PRIORITY_MAX
        };

        /** \brief DMA memory size */
        enum MemSize
        {
            /** \brief 8 bits */
            MEM_8 = 0u,
            /** \brief 16 bits */
            MEM_16 = 1u,
            /** \brief 32 bits */
            MEM_32 = 2u,

            /** \brief Max memory size value (do not use) */
            MEM_MAX
        };

        /** \brief DMA direction */
        enum Direction
        {
            /** \brief Peripheral to memory */
            DIR_PERIPH_2_MEM = 0u,
            /** \brief Memory to peripheral */
            DIR_MEM_2_PERIPH = 1u,
            
            /** \brief Max direction value (do not use) */
            DIR_MAX
        };

        /** \brief DMA channel configuration */
        struct ChannelConfig
        {
            /** \brief Request number */
            uint8_t request;
            /** \brief Memory to memory mode */
            bool mem_2_mem;
            /** \brief Priority level */
            Priority priority;
            /** \brief Memory size */
            MemSize memory_size;
            /** \brief Peripheral size */
            MemSize periph_size;
            /** \brief Memory increment mode */
            bool mem_inc;
            /** \brief Peripheral increment mode */
            bool periph_inc;
            /** \brief Circular mode */
            bool circular;
            /** \brief Read from memory direction */
            Direction dir;
        };




        /** \brief Constructor */
        Stm32l476Dma(const Dma dma);

        /** \brief Configure the DMA controller */
        bool configure();

        /** \brief Configure a DMA channel */
        bool configureChannel(const uint8_t channel, const ChannelConfig config, IStm32l476DmaListener& listener);

        /** \brief Start a DMA transfer on a channel */
        bool startXfer(const uint8_t channel, const volatile void* mem_address, const volatile void* periph_address, const uint16_t size);

        
    private:

        /** \brief DMA controller */
        const Dma m_dma;

        /** \brief Listeners */
        IStm32l476DmaListener* m_listeners[MAX_DMA_CHANNEL_COUNT];



        /** \brief IRQ handler */
        void irqHandler(const uint8_t channel);

        // To allow access to generic IRQ handler
        friend void DMA1_Channel1_IRQHandler(void);
        friend void DMA1_Channel2_IRQHandler(void);
        friend void DMA1_Channel3_IRQHandler(void);
        friend void DMA1_Channel4_IRQHandler(void);
        friend void DMA1_Channel5_IRQHandler(void);
        friend void DMA1_Channel6_IRQHandler(void);
        friend void DMA1_Channel7_IRQHandler(void);
        friend void DMA2_Channel1_IRQHandler(void);
        friend void DMA2_Channel2_IRQHandler(void);
        friend void DMA2_Channel3_IRQHandler(void);
        friend void DMA2_Channel4_IRQHandler(void);
        friend void DMA2_Channel5_IRQHandler(void);
        friend void DMA2_Channel6_IRQHandler(void);
        friend void DMA2_Channel7_IRQHandler(void);

};

}

#endif // STM32L476DMA_H
