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

#ifndef NUCLEOBOARD_H
#define NUCLEOBOARD_H

#include "IOpenVarioBoard.h"

#include "TaskHelper.h"

#include "Stm32l476Cpu.h"
#include "Stm32l476Dma.h"
#include "Stm32l476Rtc.h"
#include "Stm32l476Gpio.h"
#include "Stm32l476Usart.h"
#include "Stm32l476Spi.h"

#include "McuRtc.h"
#include "IoLed.h"
#include "At25xxx.h"
#include "BlueNrgMs.h"

namespace open_vario
{


/** \brief Chip select driver */
class SpiChipSelectDriver : public ISpiChipSelect
{
    public:

        /** \brief Constructor */
        SpiChipSelectDriver(IOutputPin& cs_pin_0, IOutputPin& cs_pin_1)
        : m_cs_pins()
        {
            m_cs_pins[0u] = &cs_pin_0;
            m_cs_pins[1u] = &cs_pin_1;
        }

        /** \brief Configure the SPI chip select driver */
        virtual bool configure() { m_cs_pins[0u]->setHigh(); m_cs_pins[1u]->setHigh(); return true; }

        /** \brief Enable the selected peripheral */
        virtual void enable(const uint8_t peripheral) { m_cs_pins[peripheral]->setLow(); }

        /** \brief Disable the selected peripheral */
        virtual void disable(const uint8_t peripheral) { m_cs_pins[peripheral]->setHigh(); }


    private:

        /** \brief CS pins */
        IOutputPin* m_cs_pins[2u];
};


/** \brief Nucleo STM32l476RG board */
class NucleoBoard
{
    public:

        /** \brief Constructor */
        NucleoBoard();


        /** \brief Configure the board peripherals */
        bool configure();


        /** \brief Get the board's CPU */
        ICpu& cpu() { return m_cpu; }

        /** \brief Get the board's RTC */
        IRtc& rtc() { return m_rtc; }

        /** \brief Get the board's activity LED */
        ILed& activityLed() { return m_activity_led; }

        /** \brief Get the board's debug UART */
        IUart& debugUart() { return m_debug_uart; }

        /** \brief Get the board's EEPROM */
        IEeprom& eeprom() { return m_eeprom; }


        /** \brief Get the BLE reset pin */
        IOutputPin& bleResetPin() { return m_ble_reset_pin; }

        /** \brief Get the BLE module */
        BlueNrgMs& bleModule() { return m_ble_module; }


    private:

        /** \brief CPU */
        Stm32l476Cpu m_cpu;

        /** \brief DMA controller 1 */
        Stm32l476Dma m_dma1;


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

        /** \brief Chip select driver for SPI bus 1 */
        SpiChipSelectDriver m_cs_driver_1;

        /** \brief SPI bus 1 */
        Stm32l476Spi m_spi_1;



        /** \brief Pin to drive the activity LED */
        Stm32l476Gpio m_activity_led_pin;

        /** \brief Activity LED */
        IoLed m_activity_led;

        
        /** \brief EEPROM */
        At25xxx m_eeprom;


        /** \brief Pin to drive BLE reset */
        Stm32l476Gpio m_ble_reset_pin;

        /** \brief Pin to drive BLE reset */
        Stm32l476Gpio m_ble_irq_pin;

        /** \brief BLE module rx task */
        TaskHelper<2048u> m_ble_rx_task;

        /** \brief BLE module */
        BlueNrgMs m_ble_module;

};

}

#endif // NUCLEOBOARD_H
