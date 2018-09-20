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

#ifndef STM32L476SPI_H
#define STM32L476SPI_H

#include "ISpi.h"
#include "Mutex.h"
#include "Semaphore.h"

namespace open_vario
{

class Stm32l476Cpu;

/** \brief IRQ handler for SPI 1 */
extern "C" void SPI1_IRQHandler(void);
/** \brief IRQ handler for SPI 2 */
extern "C" void SPI2_IRQHandler(void);
/** \brief IRQ handler for SPI 3 */
extern "C" void SPI3_IRQHandler(void);


/** \brief STM32L476 SPI */
class Stm32l476Spi : public ISpi
{
    public:


        /** \brief SPI number */
        enum Spi
        {
            /** \brief SPI 1 */
            SPI_1 = 0u,
            /** \brief SPI 2 */
            SPI_2 = 1u,
            /** \brief SPI 3 */
            SPI_3 = 2u,

            /** \brief Max SPI value (do not use) */
            SPI_MAX
        };

        /** \brief Constructor */
        Stm32l476Spi(const Stm32l476Cpu& cpu, const Spi spi, const uint32_t frequency, const Polarity polarity, const Phase phase, ISpiChipSelect& cs);


        /** \brief Configure the SPI */
        virtual bool configure();

        /** \brief Transfer data through the SPI */
        virtual bool xfer(const XFer& xfer);


    private:

        /** \brief CPU */
        const Stm32l476Cpu& m_cpu;

        /** \brief SPI */
        const Spi m_spi;
        
        /** \brief Operating freuency in Hz */
        const uint32_t m_frequency; 
        
        /** \brief Clock polarity */
        const Polarity m_polarity;
        
        /** \brief Clock phase */
        const Phase m_phase;

        /** \brief Chip select driver */
        ISpiChipSelect& m_cs;


        /** \brief Current transfer */
        const XFer* m_xfer;

        /** \brief Index of the data currently being transfered */
        uint32_t m_xfer_tx_index;

        /** \brief Index of the data currently being received */
        uint32_t m_xfer_rx_index;

        /** \brief End of transfer semaphore */
        Semaphore m_end_of_xfer;

        /** \brief Mutex to protect concurrent bus access */
        Mutex m_mutex;
        

        /** \brief IRQ handler */
        void irqHandler();

        // To allow access to generic IRQ handler
        friend void SPI1_IRQHandler(void);
        friend void SPI2_IRQHandler(void);
        friend void SPI3_IRQHandler(void);
};

}

#endif // STM32L476SPI_H
