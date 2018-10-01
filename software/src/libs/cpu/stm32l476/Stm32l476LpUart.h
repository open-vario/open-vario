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

#ifndef STM32L476LPUART_H
#define STM32L476LPUART_H

#include "IUart.h"
#include "FlagSet.h"
#include "nano-stl.h"

namespace open_vario
{

class Stm32l476Cpu;

/** \brief IRQ handler for LPUART 1 */
extern "C" void LPUART1_IRQHandler(void);


/** \brief STM32L476 LPUART */
class Stm32l476LpUart : public IUart
{
    public:

        /** \brief Constructor */
        Stm32l476LpUart(const Stm32l476Cpu& cpu, const uint32_t baudrate, const Parity parity, const StopBits stop_bits, const FlowControl flow_control);

        /** \brief Configure the UART */
        virtual bool configure();

        /** \brief Write data through the UART */
        virtual bool write(const uint8_t* const data, const uint32_t size);

        /** \brief Read data through the UART */
        virtual bool read(uint8_t* const data, const uint32_t size, const uint32_t timeout);


    private:

        /** \brief CPU */
        const Stm32l476Cpu& m_cpu;

        /** \brief Baudrate */
        const uint32_t m_baudrate;
        
        /** \brief Parity */
        const Parity m_parity;
        
        /** \brief Stop bits */
        const StopBits m_stop_bits;

        /** \brief Flow control */
        const FlowControl m_flow_control;


        /** \brief Uart status flags */
        FlagSet m_status_flags;

        /** \brief Received data queue */
        nano_stl::StaticQueue<uint8_t, 32u> m_rx_queue;

        /** \brief Transmit data queue */
        nano_stl::StaticQueue<uint8_t, 32u> m_tx_queue;


        /** \brief IRQ handler */
        void irqHandler();

        // To allow access to generic IRQ handler
        friend void LPUART1_IRQHandler(void);
};

}

#endif // STM32L476LPUART_H
