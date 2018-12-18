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

, m_dma1(Stm32l476Dma::DMA_1)

, m_crc32()

, m_rtc_driver()
, m_rtc(m_rtc_driver)

, m_activity_led_eval_pin(Stm32l476Gpio::PORT_A, 5u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)

, m_debug_uart_rx_pin(Stm32l476Gpio::PORT_A /* PORT_B */, 3u /* 7u */, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart_tx_pin(Stm32l476Gpio::PORT_A /* PORT_B */, 2u /* 6u */, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart(m_cpu, Stm32l476Usart::USART_2 /* USART_1 */, 115200u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)

, m_spi_1_sck_pin(Stm32l476Gpio::PORT_A, 4u /* 5u */, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_mosi_pin(Stm32l476Gpio::PORT_A, 7u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_miso_pin(Stm32l476Gpio::PORT_A, 6u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_cs0_pin(Stm32l476Gpio::PORT_A, 0u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_1_cs1_pin(Stm32l476Gpio::PORT_A, 1u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_1_cs2_pin(Stm32l476Gpio::PORT_A, 15u /* 2u */, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_cs_driver_1(Stm32l476Gpio::PORT_A)
, m_spi_1(m_cpu, Stm32l476Spi::SPI_1, 2000000u, ISpi::POL_HIGH, ISpi::PHA_FIRST, m_cs_driver_1, m_dma1)

, m_spi_2_sck_pin(Stm32l476Gpio::PORT_B, 13u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_2_mosi_pin(Stm32l476Gpio::PORT_B, 15u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_2_miso_pin(Stm32l476Gpio::PORT_B, 14u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_2_cs0_pin(Stm32l476Gpio::PORT_B, 0u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_2_cs1_pin(Stm32l476Gpio::PORT_B, 1u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_2_cs2_pin(Stm32l476Gpio::PORT_B, 2u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_cs_driver_2(Stm32l476Gpio::PORT_B)
, m_spi_2(m_cpu, Stm32l476Spi::SPI_2, 2000000u, ISpi::POL_HIGH, ISpi::PHA_FIRST, m_cs_driver_2, m_dma1)

, m_io_expander(m_spi_2, 4u)
, m_plus_button_pin(m_io_expander, 0u, true)
, m_minus_button_pin(m_io_expander, 1u, true)
, m_enter_button_pin(m_io_expander, 2u, true)
, m_activity_led_pin(m_io_expander, 6u, false)
, m_low_bat_led_pin(m_io_expander, 7u, false)

, m_activity_led(m_activity_led_eval_pin /* m_activity_led_pin */, IIoPin::HIGH)
, m_low_bat_led(m_low_bat_led_pin, IIoPin::HIGH)

, m_config_eeprom(m_spi_2, 1u, 32768u, 64u) // 32kB - 64B
, m_flight_data_flash(m_spi_2, 2u, 8388608u, 4096u, 256u) // 8MB - 4kB - 256B

, m_exp_uart_rx_pin(Stm32l476Gpio::PORT_C, 11u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_exp_uart_tx_pin(Stm32l476Gpio::PORT_C, 10u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_exp_uart(m_cpu, Stm32l476Usart::USART_3, 115200u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)

, m_baro_sensor(m_spi_1, 0u)
, m_alti_sensor(m_baro_sensor)

, m_gnss_uart_rx_pin(Stm32l476Gpio::PORT_C, 0u, Stm32l476Gpio::MODE_AF, 8u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_gnss_uart_tx_pin(Stm32l476Gpio::PORT_C, 1u, Stm32l476Gpio::MODE_AF, 8u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_gnss_uart(m_cpu, 9600u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)
, m_gnss_thread("GNSS Thread", 10u)
, m_gnss(m_gnss_uart, m_gnss_thread)
{}

/** \brief Configure the board peripherals */
bool OpenVarioBoard::configure()
{
    bool ret = true;

    // DMA
    ret = ret && m_dma1.configure();

    // RTC
    ret = ret && m_rtc.configure();

    // CRC-32
    ret = ret && m_crc32.configure();

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
    ret = ret && m_spi_1_cs2_pin.configure();
    ret = ret && m_cs_driver_1.configure();
    ret = ret && m_spi_1.configure();

    // SPI bus 2
    ret = ret && m_spi_2_sck_pin.configure();
    ret = ret && m_spi_2_mosi_pin.configure();
    ret = ret && m_spi_2_miso_pin.configure();
    ret = ret && m_spi_2_cs0_pin.configure();
    ret = ret && m_spi_2_cs1_pin.configure();
    ret = ret && m_spi_2_cs2_pin.configure();
    ret = ret && m_cs_driver_2.configure();
    ret = ret && m_spi_2.configure();

    // I/O expander
    ret = ret && m_io_expander.configure();
    ret = ret && m_plus_button_pin.configure();
    ret = ret && m_minus_button_pin.configure();
    ret = ret && m_enter_button_pin.configure();

    // LEDs
    ret = ret && m_activity_led.configure();
    ret = ret && m_low_bat_led.configure();

    // EEPROM
    ret = ret && m_config_eeprom.configure();

    // NOR flash
    ret = ret && m_flight_data_flash.configure();

    // Barometric altimeter sensor
    //ret = ret && m_alti_sensor.configure();

    // GNSS
    ret = ret && m_gnss_uart_rx_pin.configure();
    ret = ret && m_gnss_uart_tx_pin.configure();
    ret = ret && m_gnss_uart.configure();
    ret = ret && m_gnss.configure();
    
    return ret;
}


}
