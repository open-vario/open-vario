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

#ifndef SIMUUSBDEVICECDC_H
#define SIMUUSBDEVICECDC_H

#include "SimuDevice.h"
#include "IUsbDeviceCdc.h"


namespace open_vario
{


/** \brief Simulated USB Device Communication Device Class */
class SimuUsbDeviceCdc : public SimuDevice, public IUsbDeviceCdc
{
    public:

        /** \brief Constructor */
        SimuUsbDeviceCdc(ISimulator& simulator, const std::string& name);

        /** \brief Get the type of the device */
        static const std::string type() { return "usbd-cdc"; }


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
        virtual bool registerListener(IUsbDeviceCdcListener& listener);


    private:

        /** \brief Listener */
        IUsbDeviceCdcListener* m_listener;

        /** \brief Indicate if the device is running */
        bool m_running;
};

}

#endif // SIMUUSBDEVICECDC_H
