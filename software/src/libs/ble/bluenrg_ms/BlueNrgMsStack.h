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

#ifndef BLUENRGMSSTACK_H
#define BLUENRGMSSTACK_H

#include "IBlePeripheralStack.h"
#include "IBleCharacteristic.h"
#include "BlueNrgMs.h"

namespace open_vario
{



/** \brief Interface for all Bluetooth Low Energy stack in peripheral role implementations */
class BlueNrgMsStack : public IBlePeripheralStack, public IBlueNrgMsListener, public IBleCharacteristicListener
{
    public:

        /** \brief Constructor */
        BlueNrgMsStack(BlueNrgMs& bluenrg_ms);


        /** \brief Get the stack's name */
        virtual const char* name() const { return "ST BlueNRG-MS"; };

        /** \brief Get the stack's version */
        virtual const char* version() const { return m_version; }

        /** \brief Get the BLE chip's name */
        virtual const char* bleChipName() const { return "ST BlueNRG-MS"; };

        /** \brief Get the BLE chip's version */
        virtual const char* bleChipVersion() const { return m_version; }


        /** \brief Set the BLE device configuration */
        virtual bool setDeviceConfiguration(const DeviceConfiguration& config) { m_config = config; return true; };

        /** \brief Set the list of the BLE services to expose */
        virtual bool setBleServices(nano_stl::IArray<IBleService*>& ble_services) { m_ble_services = &ble_services; return true; }

        /** \brief Set the BLE stack listener */
        virtual void setListener(IBlePeripheralStackListener& listener) { m_listener = &listener; }


        /** \brief Start the BLE stack */
        virtual bool start();

        /** \brief Stop the BLE stack */
        virtual bool stop();


        /** \brief Called when the module is connected to another device */
        virtual void bleConnected();

        /** \brief Called when the module is disconnected from another device */
        virtual void bleDisconnected();

        /** \brief Called when an attribute as been modified by a client */
        virtual void attributeModified(const uint16_t handle, const uint8_t size, const uint8_t* const data);


        /** \brief Called when the characteristic's value has changed */
        virtual void onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size);


    private:

        /** \brief BlueNRG-MS driver */
        BlueNrgMs& m_bluenrg_ms;

        /** \brief Device configuration */
        DeviceConfiguration m_config;

        /** \brief List of BLE services */
        nano_stl::IArray<IBleService*>* m_ble_services;

        /** \brief Listener */
        IBlePeripheralStackListener* m_listener;

        /** \brief Stack's version string */
        char m_version[32u];

};

}

#endif // BLUENRGMSSTACK_H
