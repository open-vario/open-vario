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

#ifndef OPENVARIOBOARD_H
#define OPENVARIOBOARD_H

#include "IOpenVarioBoard.h"

#include "Stm32l476Cpu.h"
#include "Stm32l476Gpio.h"
#include "Stm32l476Usart.h"

#include "IoLed.h"

namespace open_vario
{



/** \brief Open vario v1.0 board */
class OpenVarioBoard : public IOpenVarioBoard
{
    public:

        /** \brief Constructor */
        OpenVarioBoard();


        /** \brief Configure the board peripherals */
        virtual bool configure();

        /** \brief Get the board's name */
        virtual const char* name() const { return "Open Vario 1.0"; }

        /** \brief Get the board's CPU */
        virtual ICpu& cpu() { return m_cpu; }

        /** \brief Get the board's activity LED */
        virtual ILed& activityLed() { return m_activity_led; }

        /** \brief Get the board's debug UART */
        virtual IUart& debugUart() { return m_debug_uart; }


    private:

        /** \brief CPU */
        Stm32l476Cpu m_cpu;


        /** \brief Pin to drive the activity LED */
        Stm32l476Gpio m_activity_led_pin;

        /** \brief Activity LED */
        IoLed m_activity_led;


        /** \brief Debug UART Rx pin */
        Stm32l476Gpio m_debug_uart_rx_pin;

        /** \brief Debug UART Tx pin */
        Stm32l476Gpio m_debug_uart_tx_pin;

        /** \brief Debug UART */
        Stm32l476Usart m_debug_uart;
};

}

#endif // OPENVARIOBOARD_H
