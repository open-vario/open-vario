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

#include "OpenVarioBoard.h"

namespace open_vario
{


/** \brief Constructor */
OpenVarioBoard::OpenVarioBoard()
: m_cpu()

, m_activity_led_pin(Stm32l476Gpio::PORT_A, 5u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)
, m_activity_led(m_activity_led_pin, ILed::OFF, IIoPin::HIGH)

, m_debug_uart_rx_pin(Stm32l476Gpio::PORT_A, 3u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart_tx_pin(Stm32l476Gpio::PORT_A, 2u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart(m_cpu, Stm32l476Usart::USART_2, 115200u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)
{

}

/** \brief Configure the board peripherals */
bool OpenVarioBoard::configure()
{
    bool ret = true;
    
    ret = ret && m_activity_led.configure();

    ret = ret && m_debug_uart_rx_pin.configure();
    ret = ret && m_debug_uart_tx_pin.configure();
    ret = ret && m_debug_uart.configure();

    return ret;
}


}
