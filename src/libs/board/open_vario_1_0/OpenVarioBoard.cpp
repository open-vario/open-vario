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
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
OpenVarioBoard::OpenVarioBoard()
: m_cpu()

, m_dma1(Stm32l476Dma::DMA_1)

, m_crc32()

, m_rtc_driver()
, m_rtc(m_rtc_driver)

, m_debug_uart_rx_pin(Stm32l476Gpio::PORT_B, 7u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart_tx_pin(Stm32l476Gpio::PORT_B, 6u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_debug_uart(m_cpu, Stm32l476Usart::USART_1, 115200u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)

, m_spi_1_sck_pin(Stm32l476Gpio::PORT_A, 5u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_mosi_pin(Stm32l476Gpio::PORT_A, 7u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_miso_pin(Stm32l476Gpio::PORT_A, 6u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_1_cs0_pin(Stm32l476Gpio::PORT_A, 0u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_1_cs1_pin(Stm32l476Gpio::PORT_A, 1u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_1_cs2_pin(Stm32l476Gpio::PORT_A, 2u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_cs_driver_1(Stm32l476Gpio::PORT_A)
, m_spi_1(m_cpu, Stm32l476Spi::SPI_1, 200000u, ISpi::POL_LOW, ISpi::PHA_FIRST, m_cs_driver_1, m_dma1)

, m_spi_2_sck_pin(Stm32l476Gpio::PORT_B, 13u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_2_mosi_pin(Stm32l476Gpio::PORT_B, 15u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_2_miso_pin(Stm32l476Gpio::PORT_B, 14u, Stm32l476Gpio::MODE_AF, 5u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_spi_2_cs0_pin(Stm32l476Gpio::PORT_B, 0u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_2_cs1_pin(Stm32l476Gpio::PORT_B, 1u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_spi_2_cs2_pin(Stm32l476Gpio::PORT_B, 2u, Stm32l476Gpio::MODE_OUTPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_cs_driver_2(Stm32l476Gpio::PORT_B)
, m_spi_2(m_cpu, Stm32l476Spi::SPI_2, 200000u, ISpi::POL_LOW, ISpi::PHA_FIRST, m_cs_driver_2, m_dma1)

, m_i2c_1_scl_pin(Stm32l476Gpio::PORT_B, 8u, Stm32l476Gpio::MODE_AF, 4u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_i2c_1_sda_pin(Stm32l476Gpio::PORT_B, 9u, Stm32l476Gpio::MODE_AF, 4u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_i2c_1(m_cpu, Stm32l476I2c::I2C_1, 100000u, m_dma1)

, m_io_expander(m_spi_2, 4u)
, m_plus_button_pin(m_io_expander, 0u, true)
, m_minus_button_pin(m_io_expander, 1u, true)
, m_enter_button_pin(m_io_expander, 2u, true)
, m_low_bat_led_pin(m_io_expander, 6u, false)
, m_activity_led_pin(m_io_expander, 7u, false)
, m_ble_reset_pin(m_io_expander, 8u, false)

, m_activity_led(m_activity_led_pin, IIoPin::HIGH)
, m_low_bat_led(m_low_bat_led_pin, IIoPin::HIGH)

, m_config_eeprom(m_spi_1, 4u, 32768u, 64u) // 32kB - 64B
, m_flight_data_flash(m_spi_2, 2u, 8388608u, 4096u, 256u) // 8MB - 4kB - 256B

, m_exp_uart_rx_pin(Stm32l476Gpio::PORT_C, 11u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_exp_uart_tx_pin(Stm32l476Gpio::PORT_C, 10u, Stm32l476Gpio::MODE_AF, 7u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_exp_uart(m_cpu, Stm32l476Usart::USART_3, 115200u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)

#ifdef MS56XX_SPI
, m_baro_sensor(m_spi_1, 1u)
#else
, m_baro_sensor(m_i2c_1, 0x76u)
#endif // MS56XX_SPI
, m_alti_sensor(m_baro_sensor)

, m_gnss_uart_rx_pin(Stm32l476Gpio::PORT_C, 0u, Stm32l476Gpio::MODE_AF, 8u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_gnss_uart_tx_pin(Stm32l476Gpio::PORT_C, 1u, Stm32l476Gpio::MODE_AF, 8u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_gnss_uart(m_cpu, 9600u, IUart::PARITY_NONE, IUart::STOPBITS_ONE, IUart::FLOWCONTROL_NONE)
, m_gnss_thread("GNSS Thread", 10u)
, m_gnss(m_gnss_uart, m_gnss_thread)

, m_ble_irq_pin(Stm32l476Gpio::PORT_C, 8u, Stm32l476Gpio::MODE_INPUT, 0u, Stm32l476Gpio::IT_RAISING, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)
, m_ble_rx_task("BlueNrgMs Rx task", 11u)
, m_bluenrgms(m_spi_1, 3u, m_ble_reset_pin, m_ble_irq_pin, m_ble_rx_task)
, m_bluenrgms_stack(m_bluenrgms)

, m_buzzer_pwm_pin(Stm32l476Gpio::PORT_A, 4u, Stm32l476Gpio::MODE_AF, 14u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_HIGH)
, m_buzzer_pwm(m_cpu, Stm32l476LpTim::LPTIM_2)
, m_buzzer(m_buzzer_pwm)

, m_usb_dm_pin(Stm32l476Gpio::PORT_A, 11u, Stm32l476Gpio::MODE_AF, 10u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_usb_dp_pin(Stm32l476Gpio::PORT_A, 12u, Stm32l476Gpio::MODE_AF, 10u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_VERY_HIGH)
, m_usb_vbus_pin(Stm32l476Gpio::PORT_A, 9u, Stm32l476Gpio::MODE_INPUT, 0u, Stm32l476Gpio::IT_NONE, Stm32l476Gpio::CONFIG_NONE, Stm32l476Gpio::SPEED_MEDIUM)
, m_usbd_cdc(m_cpu)
{
}

/** \brief Configure the board low-level peripherals */
bool OpenVarioBoard::configure()
{
    bool ret = true;

    // DMA
    ret = ret && m_dma1.configure();

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

    // I2C bus 1
    ret = ret && m_i2c_1_scl_pin.configure();
    ret = ret && m_i2c_1_sda_pin.configure();
    ret = ret && m_i2c_1.configure();

    // I/O expander
    ret = ret && m_io_expander.configure();
    ret = ret && m_plus_button_pin.configure();
    ret = ret && m_minus_button_pin.configure();
    ret = ret && m_enter_button_pin.configure();
    ret = ret && m_activity_led_pin.configure();
    ret = ret && m_low_bat_led_pin.configure();
    ret = ret && m_ble_reset_pin.configure();

    // LEDs
    ret = ret && m_activity_led.configure();
    ret = ret && m_low_bat_led.configure();

    // GNSS UART
    ret = ret && m_gnss_uart_rx_pin.configure();
    ret = ret && m_gnss_uart_tx_pin.configure();
    ret = ret && m_gnss_uart.configure();

    // BLE pins
    ret = ret && m_ble_reset_pin.configure();
    ret = ret && m_ble_irq_pin.configure();

    // Buzzer PWM
    ret = ret && m_buzzer_pwm_pin.configure();
    ret = ret && m_buzzer_pwm.configure(800u, 50u);

    // USB
    ret = ret && m_usb_dm_pin.configure();
    ret = ret && m_usb_dp_pin.configure();
    ret = ret && m_usb_vbus_pin.configure();
    ret = ret && m_usbd_cdc.configure();

    return ret;
}


}
