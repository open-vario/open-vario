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

#include "BlueNrgMsStack.h"

#include "IBleUuid.h"
#include "IBleService.h"
#include "IBleCharacteristic.h"

#include "ForEach.h"
#include <cstring>
#include <cstdio>

namespace open_vario
{


/** \brief Constructor */
BlueNrgMsStack::BlueNrgMsStack(BlueNrgMs& bluenrg_ms)
: m_bluenrg_ms(bluenrg_ms)
, m_config()
, m_ble_services(nullptr)
, m_listener(nullptr)
, m_version()
{
    m_bluenrg_ms.setListener(*this);
}

/** \brief Start the BLE stack */
bool BlueNrgMsStack::start()
{
    bool ret = false;

    // Check configuration
    if ((m_ble_services != nullptr) && (m_listener != nullptr))
    {
        // Configure the module 
        BlueNrgMs::HwConfig blue_nrg_hwconfig = {0};
        NANO_STL_MEMCPY(blue_nrg_hwconfig.ble_public_address, m_config.hw_address, sizeof(blue_nrg_hwconfig.ble_public_address));
        m_bluenrg_ms.setHwConfig(blue_nrg_hwconfig);
        m_bluenrg_ms.setDeviceName(m_config.name);
        ret = m_bluenrg_ms.configure();
        if (ret)
        {
            // Configure the services
            for (nano_stl::nano_stl_size_t i = 0; i < m_ble_services->getCount(); i++)
            {
                IBleService* service = m_ble_services->operator[](i);

                // Add the service
                uint16_t service_handle = 0u;
                const IBleUuid& service_uuid = service->uuid();
                ret = ret && m_bluenrg_ms.addBleService(&service_uuid.value()[0u], service_uuid.value().getCount(), true, service->characteristics().getCount(), service_handle);
                if (ret)
                {
                    // Save service handle
                    service->setHandle(service_handle);

                    // Add characteristics
                    const nano_stl::IArray<IBleCharacteristic*>& characteristics = service->characteristics();
                    for (nano_stl::nano_stl_size_t i = 0; i < characteristics.getCount(); i++)
                    {
                        uint16_t char_handle = 0u;
                        IBleCharacteristic* characteristic = characteristics[i];
                        const IBleUuid& char_uuid = characteristic->uuid();
                        characteristic->setService(*service);
                        ret = ret && m_bluenrg_ms.addBleCharacteristic(service_handle, &char_uuid.value()[0u], char_uuid.value().getCount(), characteristic->valueLength(), 
                                                                    characteristic->isFixedLength(), characteristic->properties(), BlueNrgMs::SP_NONE, char_handle);
                        if (ret)
                        {
                            // Save characteristic handle
                            characteristic->setHandle(char_handle);

                            // Register to characteristic changes notifications
                            characteristic->registerListener(*this);
                        }
                    }
                }
            }
        }

        // Get the module's version
        if (ret)
        {
            uint8_t hw_version = 0; 
            uint8_t fw_version = 0; 
            uint8_t fw_subversion = 0;
            ret = m_bluenrg_ms.getVersion(hw_version, fw_version, fw_subversion);
            if (ret)
            {
                NANO_STL_SNPRINTF(m_version, sizeof(m_version), "%d.%d (Hw %d)", fw_version, fw_subversion, hw_version);
            }
        }

        // Set the module as discoverable
        ret = ret && m_bluenrg_ms.setDiscoverable();
    }

    return ret;
}

/** \brief Stop the BLE stack */
bool BlueNrgMsStack::stop()
{
    return true;
}

/** \brief Called when the module is connected to another device */
void BlueNrgMsStack::bleConnected()
{
    // Notify listener
    m_listener->bleClientConnected();
}

/** \brief Called when the module is disconnected from another device */
void BlueNrgMsStack::bleDisconnected()
{
    // Notify listener
    m_listener->bleClientDisconnected();
}

/** \brief Called when an attribute as been modified by a client */
void BlueNrgMsStack::attributeModified(const uint16_t handle, const uint8_t size, const uint8_t* const data)
{
    // Look for the corresponding characteristic
    bool found = false;
    for (nano_stl::nano_stl_size_t i = 0; (i < m_ble_services->getCount()) && !found; i++)
    {
        const nano_stl::IArray<IBleCharacteristic*>& characteristics = (*m_ble_services)[i]->characteristics();
        for (nano_stl::nano_stl_size_t j = 0; (j < characteristics.getCount()) && !found; j++)
        {
            if (characteristics[j]->handle() == handle)
            {
                // Notify listeners
                characteristics[j]->updateValue(true, data, size);
                found = true;
            }
        }
    }
}

/** \brief Called when the characteristic's value has changed */
void BlueNrgMsStack::onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size)
{
    // Update value in BLE module
    if (!from_stack)
    {
        m_bluenrg_ms.updateBleCharacteristicValue(characteristic.service().handle(), characteristic.handle(), new_value, new_value_size);
    }
}

}

