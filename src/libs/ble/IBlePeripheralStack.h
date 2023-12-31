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

#ifndef IBLEPERIPHERALSTACK_H
#define IBLEPERIPHERALSTACK_H

#include "IArray.h"

namespace open_vario
{


/** \brief Interface for all Bluetooth Low Energy stack in peripheral role listeners implementations */
class IBlePeripheralStackListener
{
    public:

        /** \brief Called when a client device is connected to the stack */
        virtual void bleClientConnected() = 0;

        /** \brief Called when a client device is connected to the stack */
        virtual void bleClientDisconnected() = 0;
};


class IBleService;

/** \brief Interface for all Bluetooth Low Energy stack in peripheral role implementations */
class IBlePeripheralStack
{
    public:

        /** \brief Bluetooth device name size in bytes */
        static const uint8_t DEVICE_NAME_SIZE = 31u;

        /** \brief Bluetooth hardware address size in bytes */
        static const uint8_t HW_ADDRESS_SIZE = 6u;

        /** \brief BLE device configuration */
        struct DeviceConfiguration
        {
            /** \brief Device's name */
            char name[DEVICE_NAME_SIZE + 1u];
            /** \brief Hardware address */
            uint8_t hw_address[HW_ADDRESS_SIZE];
        };


        /** \brief Get the stack's name */
        virtual const char* name() const = 0;

        /** \brief Get the stack's version */
        virtual const char* version() const = 0;

        /** \brief Get the BLE chip's name */
        virtual const char* bleChipName() const = 0;

        /** \brief Get the BLE chip's version */
        virtual const char* bleChipVersion() const = 0;


        /** \brief Set the BLE device configuration */
        virtual bool setDeviceConfiguration(const DeviceConfiguration& config) = 0;

        /** \brief Set the list of the BLE services to expose */
        virtual bool setBleServices(nano_stl::IArray<IBleService*>& ble_services) = 0;

        /** \brief Set the BLE stack listener */
        virtual void setListener(IBlePeripheralStackListener& listener) = 0;


        /** \brief Check if the BLE hardware is present and working */
        virtual bool probe() = 0;

        /** \brief Start the BLE stack */
        virtual bool start() = 0;

        /** \brief Do actions needed when a new client is connected */
        virtual void doConnectActions() = 0;

        /** \brief Do actions needed when a client is disconnected */
        virtual void doDisconnectActions() = 0;

};

}

#endif // IBLEPERIPHERALSTACK_H
