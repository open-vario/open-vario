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

#ifndef STM32L476USBDEVICE_H
#define STM32L476USBDEVICE_H

#include "nano-stl.h"

namespace open_vario
{

class Stm32l476Cpu;

/** \brief IRQ handler for USB */
extern "C" void OTG_FS_IRQHandler(void);



/** \brief STM32L476 USB Device */
class Stm32l476UsbDevice
{
    public:

        /** \brief Constructor */
        Stm32l476UsbDevice(const Stm32l476Cpu& cpu);

        /** \brief Configure the USB */
        bool configureUSB(void* descriptors, void* device_class);

        /** \brief Start USB */
        bool startUSB();

        /** \brief Stop USB */
        bool stopUSB();


    protected:

        /** \brief Get the USB device instance */
        void* getDevice() { return m_usbd; }

        /** \brief Disable USB device interrupts */
        void disableInterrupts();

        /** \brief Enable USB device interrupts */
        void enableInterrupts();

        /** \brief Retrieve the unique instance */
        static Stm32l476UsbDevice& getInstance();


    private:

        /** \brief CPU */
        const Stm32l476Cpu& m_cpu;

        /** \brief USB device instance */
        void* m_usbd;

        

        /** \brief IRQ handler */
        void irqHandler();

        // To allow access to generic IRQ handler
        friend void OTG_FS_IRQHandler(void);
};

}

#endif // STM32L476USBDEVICE_H
