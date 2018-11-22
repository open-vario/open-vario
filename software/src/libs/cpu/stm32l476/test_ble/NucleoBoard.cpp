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

#include "NucleoBoard.h"
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
NucleoBoard::NucleoBoard()
: m_cpu()

, m_rtc_driver()
, m_rtc(m_rtc_driver)

, m_debug_uart_rx_pin(Stm32l476Gpio::PORT_A, 3u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart_tx_pin(Stm32l476Gpio::PORT_A, 2u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart(m_cpu, Stm32l476Usart::USART_2, 115200u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)

, m_spi_1_sck_pin(Stm32l476Gpio::PORT_B, 3u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_mosi_pin(Stm32l476Gpio::PORT_A, 7u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_miso_pin(Stm32l476Gpio::PORT_A, 6u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_cs0_pin(Stm32l476Gpio::PORT_A, 1u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_1_cs1_pin(Stm32l476Gpio::PORT_A, 9u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_cs_driver_1(m_spi_1_cs0_pin, m_spi_1_cs1_pin)
, m_spi_1(m_cpu, Stm32l476Spi::SPI_1, 100000u, ISpi::POL_LOW, ISpi::PHA_FIRST, m_cs_driver_1)

, m_activity_led_pin(Stm32l476Gpio::PORT_A, 5u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)
, m_activity_led(m_activity_led_pin, IIoPin::HIGH)

, m_eeprom(m_spi_1, 1u, 8096u, 32u) // 8kB - 32B

, m_ble_reset_pin(Stm32l476Gpio::PORT_A, 8u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)
, m_ble_irq_pin(Stm32l476Gpio::PORT_C, 7u, Stm32l476Gpio::MODE_INPUT, 0u, Stm32l476Gpio::IT_RAISING, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)
, m_ble_rx_task("BlueNrgMs Rx task", 15u)
, m_ble_module(m_spi_1, 0u, m_ble_irq_pin, m_ble_rx_task)
{}

/** \brief Configure the board peripherals */
bool NucleoBoard::configure()
{
    bool ret = true;

    // RTC
    ret = ret && m_rtc.configure();

    // Debug UART
    ret = ret && m_debug_uart_rx_pin.configure();
    ret = ret && m_debug_uart_tx_pin.configure();
    ret = ret && m_debug_uart.configure();

    // SPI bus 1
    ret = ret && m_spi_1_sck_pin.configure();
    ret = ret && m_spi_1_mosi_pin.configure();
    ret = ret && m_spi_1_miso_pin.configure();
    ret = ret && m_spi_1_cs0_pin.configure();
    ret = ret && m_spi_1_cs1_pin.configure();
    ret = ret && m_cs_driver_1.configure();
    ret = ret && m_spi_1.configure();

    // LEDs
    ret = ret && m_activity_led.configure();

    // EEPROM
    ret = ret && m_eeprom.configure();

    // BLE
    ret = ret && m_ble_reset_pin.configure();
    ret = ret && m_ble_irq_pin.configure();
    m_ble_reset_pin.setLow();
    IOs::getInstance().waitMs(100u);
    m_ble_reset_pin.setHigh();
    ret = ret && m_ble_module.configure();
    
    return ret;
}


}
