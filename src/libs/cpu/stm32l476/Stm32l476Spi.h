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
#include "Stm32l476Dma.h"

namespace open_vario
{

class Stm32l476Cpu;

/** \brief STM32L476 SPI */
class Stm32l476Spi : public ISpi, public IStm32l476DmaListener
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
        Stm32l476Spi(const Stm32l476Cpu& cpu, const Spi spi, const uint32_t frequency, const Polarity polarity, const Phase phase, ISpiChipSelect& cs, Stm32l476Dma& dma);


        /** \brief Configure the SPI */
        virtual bool configure();

        /** \brief Transfer data through the SPI */
        virtual bool xfer(const XFer& xfer);

        /** \brief Release chip select if it has been maintened active after a call to xfer() methode */
        virtual bool releaseCs();

        /** \brief Called when the DMA transfer is done on a channel */
        virtual void onDmaComplete(const uint8_t channel, const bool success);


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

        /** \brief DMA controller */
        Stm32l476Dma& m_dma;


        /** \brief Current transfer */
        const XFer* m_xfer;

        /** \brief End of transfer semaphore */
        Semaphore m_end_of_xfer;

        /** \brief Mutex to protect concurrent bus access */
        Mutex m_mutex;

        /** \brief Indicate if the chip select is still active after the transfer */
        bool m_cs_active;

        /** \brief Last chip select */
        uint8_t m_last_cs;


        /** \brief DMA channel for Rx */
        static const uint8_t RX_DMA_CHANNEL[];

        /** \brief DMA channel for Tx */
        static const uint8_t TX_DMA_CHANNEL[];

        /** \brief DMA request source */
        static const uint8_t DMA_REQ_SOURCE[];

};

}

#endif // STM32L476SPI_H
