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

#ifndef DUMMYUSBDEVICECDC_H
#define DUMMYUSBDEVICECDC_H

#include "IUsbDeviceCdc.h"


namespace open_vario
{


/** \brief Dummy USB Device Communication Device Class implementation */
class DummyUsbDeviceCdc : public IUsbDeviceCdc
{
    public:


        /** \brief Configure the USB CDC device */
        virtual bool configure() { return true; }

        /** \brief Start the USB CDC device */
        virtual bool start() { return true; }

        /** \brief Stop the USB CDC device */
        virtual bool stop() { return true; }

        /** \brief Write data through the USB CDC device */
        virtual bool write(const uint8_t* const data, const uint32_t size) { (void)data; (void)size; return true; }

        /** \brief Read data from the USB CDC device */
        virtual bool read(uint8_t* const data, const uint32_t size, uint32_t& read_size, const uint32_t timeout) { (void)data; (void)size; (void)read_size; (void)timeout; return true; }

        /** \brief Register a listener */
        virtual bool registerListener(IUsbDeviceCdcListener& listener) { (void)listener; return true; }
    
};

}

#endif // DUMMYUSBDEVICECDC_H
