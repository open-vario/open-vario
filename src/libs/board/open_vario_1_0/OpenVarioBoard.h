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

#include "TaskHelper.h"

#include "Stm32l476Cpu.h"
#include "Stm32l476Dma.h"
#include "Stm32l476Rtc.h"
#include "Stm32l476Gpio.h"
#include "Stm32l476Usart.h"
#include "Stm32l476LpUart.h"
#include "Stm32l476Spi.h"
#include "SpiChipSelectDriver.h"
#include "Stm32l476I2c.h"
#include "Stm32l476Crc32.h"
#include "Stm32l476LpTimPwm.h"
#include "Stm32l476UsbDeviceCdc.h"

#include "McuRtc.h"
#include "IoLed.h"
#include "Mcp23S17.h"
#include "IoExpanderPin.h"
#include "At25xxx.h"
#include "SSt26xxx.h"
#ifdef MS56XX_SPI
#include "Ms56xxSpi.h"
#else
#include "Ms56xxI2c.h"
#endif // MS56XX_SPI
#include "BarometricAltimeter.h"
#include "UBloxM8.h"
#include "BlueNrgMs.h"
#include "BlueNrgMsStack.h"
#include "PwmBuzzer.h"

namespace open_vario
{

class ConfigManager;

/** \brief Open vario v1.0 board */
class OpenVarioBoard : public IOpenVarioBoard
{
    public:

        /** \brief Constructor */
        OpenVarioBoard();


        /** \brief Configure the board low-level peripherals */
        virtual bool configure();

        /** \brief Start the board peripherals */
        virtual bool start() { return true; }

        /** \brief Get the board's name */
        virtual const char* name() const { return "Open Vario 1.0"; }

        /** \brief Get the board's CPU */
        virtual ICpu& cpu() { return m_cpu; }

        /** \brief Get the board's RTC */
        virtual IRtc& rtc() { return m_rtc; }

        /** \brief Get the board's activity LED */
        virtual ILed& activityLed() { return m_activity_led; }

        /** \brief Get the board's debug UART */
        virtual IUart& debugUart() { return m_debug_uart; }

        /** \brief Get the board's CRC-32 unit */
        virtual ICrc32& crc32() { return m_crc32; }

        /** \brief Get the board's configuration EEPROM */
        virtual IEeprom& config_eeprom() { return m_config_eeprom; }

        /** \brief Get the board's flight data NOR flash */
        virtual INorFlash& flight_data_norflash() { return m_flight_data_flash; }

        /** \brief Get the board's altimeter sensor */
        virtual IBarometricAltimeter& altimeter() { return m_alti_sensor; }

        /** \brief Get the board's GNSS */
        virtual IGnss& gnss() { return m_gnss; }

        /** \brief Get the board's Bluetooth Low Energy stack */
        virtual IBlePeripheralStack& ble_stack() { return m_bluenrgms_stack; }

        /** \brief Get the board's buzzer */
        virtual IBuzzer& buzzer() { return m_buzzer; }

        /** \brief Get the board's USB Device CDC */
        virtual IUsbDeviceCdc& usbd_cdc() { return m_usbd_cdc; }



        /** \brief Get the board's SPI1 driver */
        ISpi& spi1() { return m_spi_1; }

        /** \brief Get the board's SPI2 driver */
        ISpi& spi2() { return m_spi_2; }

        /** \brief Get the board's I2C1 driver */
        II2c& i2c1() { return m_i2c_1; }
        

    private:

        /** \brief CPU */
        Stm32l476Cpu m_cpu;

        /** \brief DMA controller 1 */
        Stm32l476Dma m_dma1;

        /** \brief CRC-32 unit */
        Stm32l476Crc32 m_crc32;


        /** \brief RTC driver */
        Stm32l476Rtc m_rtc_driver;

        /** \brief RTC peripheral */
        McuRtc m_rtc;


        /** \brief Debug UART Rx pin */
        Stm32l476Gpio m_debug_uart_rx_pin;

        /** \brief Debug UART Tx pin */
        Stm32l476Gpio m_debug_uart_tx_pin;

        /** \brief Debug UART */
        Stm32l476Usart m_debug_uart;



        /** \brief SPI bus 1 SCK pin */
        Stm32l476Gpio m_spi_1_sck_pin;

        /** \brief SPI bus 1 MOSI pin */
        Stm32l476Gpio m_spi_1_mosi_pin;

        /** \brief SPI bus 1 MISO pin */
        Stm32l476Gpio m_spi_1_miso_pin;

        /** \brief SPI bus 1 CS0 pin */
        Stm32l476Gpio m_spi_1_cs0_pin;

        /** \brief SPI bus 1 CS1 pin */
        Stm32l476Gpio m_spi_1_cs1_pin;

        /** \brief SPI bus 1 CS2 pin */
        Stm32l476Gpio m_spi_1_cs2_pin;

        /** \brief Chip select driver for SPI bus 1 */
        SpiChipSelectDriver m_cs_driver_1;

        /** \brief SPI bus 1 */
        Stm32l476Spi m_spi_1;


        /** \brief SPI bus 2 SCK pin */
        Stm32l476Gpio m_spi_2_sck_pin;

        /** \brief SPI bus 2 MOSI pin */
        Stm32l476Gpio m_spi_2_mosi_pin;

        /** \brief SPI bus 2 MISO pin */
        Stm32l476Gpio m_spi_2_miso_pin;

        /** \brief SPI bus 2 CS0 pin */
        Stm32l476Gpio m_spi_2_cs0_pin;

        /** \brief SPI bus 2 CS1 pin */
        Stm32l476Gpio m_spi_2_cs1_pin;

        /** \brief SPI bus 2 CS2 pin */
        Stm32l476Gpio m_spi_2_cs2_pin;

        /** \brief Chip select driver for SPI bus 2 */
        SpiChipSelectDriver m_cs_driver_2;

        /** \brief SPI bus 2 */
        Stm32l476Spi m_spi_2;


        /** \brief I2C bus 1 SCL pin */
        Stm32l476Gpio m_i2c_1_scl_pin;

        /** \brief I2C bus 1 SDA pin */
        Stm32l476Gpio m_i2c_1_sda_pin;

        /** \brief I2C bus 1 */
        Stm32l476I2c m_i2c_1;


        /** \brief I/O expander */
        Mcp23S17 m_io_expander;

        /** \brief Pin to read PLUS button state */
        IoExpanderPin m_plus_button_pin;

        /** \brief Pin to read MINUS button state */
        IoExpanderPin m_minus_button_pin;

        /** \brief Pin to read ENTER button state */
        IoExpanderPin m_enter_button_pin;

        /** \brief Pin to drive the low battery LED */
        IoExpanderPin m_low_bat_led_pin;

        /** \brief Pin to drive the activity LED */
        IoExpanderPin m_activity_led_pin;

        /** \brief Pin to drive the BLE chip reset */
        IoExpanderPin m_ble_reset_pin;

        /** \brief Activity LED */
        IoLed m_activity_led;

        /** \brief Low battery LED */
        IoLed m_low_bat_led;


        /** \brief Configuration EEPROM */
        At25xxx m_config_eeprom;

        /** \brief Flight data NOR flash */
        SSt26xxx m_flight_data_flash;


        /** \brief Expansion UART Rx pin */
        Stm32l476Gpio m_exp_uart_rx_pin;

        /** \brief Expansion UART Tx pin */
        Stm32l476Gpio m_exp_uart_tx_pin;

        /** \brief Expansion UART */
        Stm32l476Usart m_exp_uart;


        #ifdef MS56XX_SPI
        /** \brief Barometric pressure sensor */
        Ms56xxSpi m_baro_sensor;
        #else
        /** \brief Barometric pressure sensor */
        Ms56xxI2c m_baro_sensor;
        #endif // MS56XX_SPI

        /** \brief Barometric altimeter sensor */
        BarometricAltimeter m_alti_sensor;


        /** \brief GNSS UART Rx pin */
        Stm32l476Gpio m_gnss_uart_rx_pin;

        /** \brief GNSS UART Tx pin */
        Stm32l476Gpio m_gnss_uart_tx_pin;

        /** \brief GNSS UART */
        Stm32l476LpUart m_gnss_uart;

        /** \brief U-Blox M8 GNSS Rx task */
        TaskHelper<1024u> m_gnss_thread;

        /** \brief U-Blox M8 GNSS */
        UBloxM8 m_gnss;


        /** \brief BLE interrupt pin */
        Stm32l476Gpio m_ble_irq_pin;

        /** \brief BLE taskk */
        TaskHelper<2048u> m_ble_rx_task;

        /** \brief BLE chip */
        BlueNrgMs m_bluenrgms;

        /** \brief BLE stack */
        BlueNrgMsStack m_bluenrgms_stack;


        /** \brief Buzzer PWM pin */
        Stm32l476Gpio m_buzzer_pwm_pin;

        /** \brief Buzzer PWM */
        Stm32l476LpTimPwm m_buzzer_pwm;

        /** \brief Buzzer */
        PwmBuzzer m_buzzer;


        /** \brief USB DM pin */
        Stm32l476Gpio m_usb_dm_pin;

        /** \brief USB DP pin */
        Stm32l476Gpio m_usb_dp_pin;

        /** \brief USB VBUS pin */
        Stm32l476Gpio m_usb_vbus_pin;

        /** \brief USB CDC device */
        Stm32l476UsbDeviceCdc m_usbd_cdc;

};

}

#endif // OPENVARIOBOARD_H
