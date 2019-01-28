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

#ifndef SIMUBLEPERIPHERALSTACK_H
#define SIMUBLEPERIPHERALSTACK_H

#include "SimuDevice.h"
#include "IBlePeripheralStack.h"

namespace open_vario
{


/** \brief Interface for all Bluetooth Low Energy stack in peripheral role implementations */
class SimuBlePeripheralStack : public SimuDevice, public IBlePeripheralStack
{
    public:


        /** \brief Constructor */
        SimuBlePeripheralStack(ISimulator& simulator, const std::string& name);


        /** \brief Get the type of the device */
        static const std::string type() { return "ble_peripheral"; }


        /** \brief Get the stack's name */
        virtual const char* name() const { return "Simulated BLE stack"; }

        /** \brief Get the stack's version */
        virtual const char* version() const { return "v1.0"; }

        /** \brief Get the BLE chip's name */
        virtual const char* bleChipName() const { return "Simulated BLE chip"; }

        /** \brief Get the BLE chip's version */
        virtual const char* bleChipVersion() const { return "v1.0"; }


        /** \brief Set the BLE device configuration */
        virtual bool setDeviceConfiguration(const DeviceConfiguration& config) { m_device_config = config; return true; }

        /** \brief Set the list of the BLE services to expose */
        virtual bool setBleServices(nano_stl::IArray<IBleService*>& ble_services) { m_ble_services = &ble_services; return true; }

        /** \brief Set the BLE stack listener */
        virtual void setListener(IBlePeripheralStackListener& listener) { m_listener = &listener; }


        /** \brief Start the BLE stack */
        virtual bool start();

        /** \brief Stop the BLE stack */
        virtual bool stop();


    private:

        /** \brief Device configuration */
        DeviceConfiguration m_device_config;

        /** \brief BLE services */
        nano_stl::IArray<IBleService*>* m_ble_services;

        /** \brief Listener */
        IBlePeripheralStackListener* m_listener;

};

}

#endif // SIMUBLEPERIPHERALSTACK_H
