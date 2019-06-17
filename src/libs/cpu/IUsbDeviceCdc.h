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

#ifndef IUSBDEVICECDC_H
#define IUSBDEVICECDC_H

#include "nano-stl.h"


namespace open_vario
{


/** \brief Interface for all USB Device Communication Device Clas listeners implementations */
class IUsbDeviceCdcListener
{
    public:

        /** \brief Called when the USB cable has been plugged */
        virtual void onUsbPlugged() = 0;

        /** \brief Called when the USB cable has been unplugged */
        virtual void onUsbUnplugged() = 0;

};


/** \brief Interface for all USB Device Communication Device Class implementations */
class IUsbDeviceCdc
{
    public:


        /** \brief Configure the USB CDC device */
        virtual bool configure() = 0;

        /** \brief Start the USB CDC device */
        virtual bool start() = 0;

        /** \brief Stop the USB CDC device */
        virtual bool stop() = 0;

        /** \brief Write data through the USB CDC device */
        virtual bool write(const uint8_t* const data, const uint32_t size) = 0;

        /** \brief Read data from the USB CDC device */
        virtual bool read(uint8_t* const data, const uint32_t size, uint32_t& read_size, const uint32_t timeout) = 0;

        /** \brief Register a listener */
        virtual bool registerListener(IUsbDeviceCdcListener& listener) = 0;
    
};

}

#endif // IUSBDEVICECDC_H
