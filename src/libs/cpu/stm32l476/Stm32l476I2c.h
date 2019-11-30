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

#ifndef STM32L476I2C_H
#define STM32L476I2C_H

#include "II2C.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "Stm32l476Dma.h"

namespace open_vario
{

class Stm32l476Cpu;

/** \brief IRQ handler for I2C 1 */
extern "C" void I2C1_EV_IRQHandler(void);
/** \brief IRQ handler for I2C 2 */
extern "C" void I2C2_EV_IRQHandler(void);
/** \brief IRQ handler for I2C 3 */
extern "C" void I2C3_EV_IRQHandler(void);
/** \brief IRQ handler for I2C 1 */
extern "C" void I2C1_ER_IRQHandler(void);
/** \brief IRQ handler for I2C 2 */
extern "C" void I2C2_ER_IRQHandler(void);
/** \brief IRQ handler for I2C 3 */
extern "C" void I2C3_ER_IRQHandler(void);


/** \brief STM32L476 I2C */
class Stm32l476I2c : public II2c, public IStm32l476DmaListener
{
    public:


        /** \brief I2C number */
        enum I2c
        {
            /** \brief I2C 1 */
            I2C_1 = 0u,
            /** \brief I2C 2 */
            I2C_2 = 1u,
            /** \brief I2C 3 */
            I2C_3 = 2u,

            /** \brief Max I2C value (do not use) */
            I2C_MAX
        };

        /** \brief Constructor */
        Stm32l476I2c(const Stm32l476Cpu& cpu, const I2c i2c, const uint32_t frequency, Stm32l476Dma& dma);


        /** \brief Configure the I2C */
        virtual bool configure();

        /** \brief Transfer data through the I2C */
        virtual bool xfer(const uint8_t slave_address, const XFer& xfer);

        /** \brief Transfer data through the I2C */
        virtual bool xfer(const uint8_t slave_address, const XFer& xfer, I2cError& error);

        /** \brief Called when the DMA transfer is done on a channel */
        virtual void onDmaComplete(const uint8_t channel, const bool success);


    private:

        /** \brief CPU */
        const Stm32l476Cpu& m_cpu;

        /** \brief I2C */
        const I2c m_i2c;
        
        /** \brief Operating freuency in Hz */
        const uint32_t m_frequency; 
        

        /** \brief DMA controller */
        Stm32l476Dma& m_dma;


        /** \brief Current transfer */
        const XFer* m_xfer;

        /** \brief Current error status */
        volatile I2cError m_error;

        /** \brief End of transfer semaphore */
        Semaphore m_end_of_xfer;

        /** \brief Mutex to protect concurrent bus access */
        Mutex m_mutex;


        /** \brief DMA channel for Rx */
        static const uint8_t RX_DMA_CHANNEL[];

        /** \brief DMA channel for Tx */
        static const uint8_t TX_DMA_CHANNEL[];

        /** \brief DMA request source */
        static const uint8_t DMA_REQ_SOURCE[];



        /** \brief IRQ handler */
        void irqHandler();

        // To allow access to generic IRQ handler
        friend void I2C1_EV_IRQHandler(void);
        friend void I2C2_EV_IRQHandler(void);
        friend void I2C3_EV_IRQHandler(void);
        friend void I2C1_ER_IRQHandler(void);
        friend void I2C2_ER_IRQHandler(void);
        friend void I2C3_ER_IRQHandler(void);

};

}

#endif // STM32L476I2C_H
