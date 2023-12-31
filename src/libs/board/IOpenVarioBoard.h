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

#ifndef IOPENVARIOBOARD_H
#define IOPENVARIOBOARD_H

#include <cstdint>


namespace open_vario
{

class ICpu;
class IRtc;
class ILed;
class IUart;
class ICrc32;
class IEeprom;
class INorFlash;
class IBarometricAltimeter;
class IGnss;
class IBlePeripheralStack;
class IBuzzer;
class IUsbDeviceCdc;

/** \brief Interface for all open vario boards implementations */
class IOpenVarioBoard
{
    public:


        /** \brief Configure the board low-level peripherals */
        virtual bool configure() = 0;

        /** \brief Start the board peripherals */
        virtual bool start() = 0;

        /** \brief Get the board's name */
        virtual const char* name() const = 0;

        /** \brief Get the board's CPU */
        virtual ICpu& cpu() = 0;

        /** \brief Get the board's RTC */
        virtual IRtc& rtc() = 0;

        /** \brief Get the board's activity LED */
        virtual ILed& activityLed() = 0;

        /** \brief Get the board's debug UART */
        virtual IUart& debugUart() = 0;

        /** \brief Get the board's CRC-32 unit */
        virtual ICrc32& crc32() = 0;

        /** \brief Get the board's EEPROM */
        virtual IEeprom& config_eeprom() = 0;

        /** \brief Get the board's flight data NOR flash */
        virtual INorFlash& flight_data_norflash() = 0;

        /** \brief Get the board's altimeter sensor */
        virtual IBarometricAltimeter& altimeter() = 0;

        /** \brief Get the board's GNSS */
        virtual IGnss& gnss() = 0;

        /** \brief Get the board's Bluetooth Low Energy stack */
        virtual IBlePeripheralStack& ble_stack() = 0;

        /** \brief Get the board's buzzer */
        virtual IBuzzer& buzzer() = 0;

        /** \brief Get the board's USB Device CDC */
        virtual IUsbDeviceCdc& usbd_cdc() = 0;
};

}

#endif // IOPENVARIOBOARD_H
