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

#ifndef STM32L476USBDEVICECDC_H
#define STM32L476USBDEVICECDC_H

#include "IUsbDeviceCdc.h"
#include "Stm32l476UsbDevice.h"
#include "FlagSet.h"

namespace open_vario
{


/** \brief STM32L476 USB Device Communication Device Class */
class Stm32l476UsbDeviceCdc : public Stm32l476UsbDevice, public IUsbDeviceCdc
{
    public:

        /** \brief Constructor */
        Stm32l476UsbDeviceCdc(const Stm32l476Cpu& cpu);

        /** \brief Configure the USB CDC device */
        virtual bool configure();

        /** \brief Start the USB CDC device */
        virtual bool start();

        /** \brief Stop the USB CDC device */
        virtual bool stop();

        /** \brief Write data through the USB CDC device */
        virtual bool write(const uint8_t* const data, const uint32_t size);

        /** \brief Read data from the USB CDC device */
        virtual bool read(uint8_t* const data, const uint32_t size, uint32_t& read_size, const uint32_t timeout);

        /** \brief Register a listener */
        virtual bool registerListener(IUsbDeviceCdcListener& listener) { m_listener = &listener; return true; }


    private:

        /** \brief USB CDC device status flags */
        FlagSet m_status_flags;

        /** \brief Received data queue */
        nano_stl::StaticQueue<uint8_t, 1024u> m_rx_queue;

        /** \brief Low level receive buffer */
        uint8_t m_ll_rx_buffer[64u];

        /** \brief Low level transmit buffer */
        uint8_t m_ll_tx_buffer[64u];

        /** \brief Listener */
        IUsbDeviceCdcListener* m_listener;




        /** \brief Retrieve the unique instance */
        static Stm32l476UsbDeviceCdc& getInstance();

        
        /** \brief Initializes the CDC media low layer */
        static int8_t ifaceInit(void);

        /** \brief DeInitializes the CDC media low layer */
        static int8_t ifaceDeInit(void);

        /** \brief Manage the CDC class requests */
        static int8_t ifaceControl(uint8_t cmd, uint8_t* buff, uint16_t length);

        /** \brief Data received over USB OUT endpoint are sent over CDC interface through this function */
        static int8_t ifaceReceive(uint8_t* buff, uint32_t* length);
        
        /** \brief Data has been transmitted over USB IN endpoint */
        static int8_t ifaceTransmitDone();
    
};

}

#endif // STM32L476USBDEVICECDC_H
