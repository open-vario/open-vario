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

#include "IdentificationService.h"
#include "DeviceManager.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
IdentificationService::IdentificationService(DeviceManager& device_manager)
: m_device_manager(device_manager)

, m_identification_service("Identification service", {0x38u, 0xdfu, 0x4du, 0xa7u, 0x94u, 0xf3u, 0x44u, 0xdcu, 0x83u, 0xadu, 0x4eu, 0x10u, 0x86u, 0x4fu, 0xbdu, 0x44u})

, m_software_version("Software version", {0x52u, 0x0bu, 0x42u, 0xa8u, 0xeeu, 0x29u, 0x46u, 0xecu, 0x9eu, 0xffu, 0x24u, 0xe7u, 0x32u, 0xcau, 0x0cu, 0xb5u}, 5u, IBleCharacteristic::PROP_READ)
, m_software_manufacturer_name("Software manufacturer name", {0xdeu, 0xa2u, 0x33u, 0xccu, 0xdau, 0xbbu, 0x4bu, 0x00u, 0x90u, 0x46u, 0xf7u, 0x0au, 0x44u, 0xc1u, 0xceu, 0xdau}, 16u /* 64u */, IBleCharacteristic::PROP_READ)
, m_gatt_profile_version("Open Vario GATT profile version", {0x26u, 0xacu, 0xddu, 0x71u, 0xabu, 0x60u, 0x4au, 0x9cu, 0xbbu, 0x5bu, 0x10u, 0x1eu, 0xecu, 0xc4u, 0x3cu, 0x00u}, 5u, IBleCharacteristic::PROP_READ)
, m_hardware_version("Hardware version", {0x77u, 0x37u, 0x30u, 0x05u, 0x83u, 0xc6u, 0x41u, 0x17u, 0xb8u, 0xa9u, 0x04u, 0xccu, 0x2bu, 0xe0u, 0x82u, 0x24u}, 5u, IBleCharacteristic::PROP_READ)
, m_hardware_manufacturer_name("Hardware manufacturer name", {0x82u, 0xd0u, 0x7du, 0x5au, 0x16u, 0x1au, 0x40u, 0xa8u, 0xb3u, 0x6fu, 0x9fu, 0x81u, 0x6eu, 0xc5u, 0xd3u, 0x4fu}, 16u /* 64u */, IBleCharacteristic::PROP_READ)
, m_hardware_serial_number("Hardware serial number", {0xcbu, 0xb1u, 0x21u, 0x93u, 0x2bu, 0x57u, 0x42u, 0x3eu, 0x83u, 0x81u, 0xccu, 0x69u, 0x96u, 0xaeu, 0xe5u, 0x33u}, 16u /* 64u */, IBleCharacteristic::PROP_READ)
, m_hardware_manufacturing_date("Hardware manufacturing date", {0xddu, 0xd3u, 0x4du, 0x61u, 0xe8u, 0x35u, 0x48u, 0x78u, 0xa8u, 0xacu, 0xcfu, 0x22u, 0x21u, 0xf3u, 0x19u, 0x29u}, 19u, IBleCharacteristic::PROP_READ)
{}


/** \brief Initialize the BLE service */
bool IdentificationService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_identification_service.addCharacteristic(m_software_version);
    ret = ret && m_identification_service.addCharacteristic(m_software_manufacturer_name);
    ret = ret && m_identification_service.addCharacteristic(m_gatt_profile_version);
    ret = ret && m_identification_service.addCharacteristic(m_hardware_version);
    ret = ret && m_identification_service.addCharacteristic(m_hardware_manufacturer_name);
    ret = ret && m_identification_service.addCharacteristic(m_hardware_serial_number);

    return ret;
}

/** \brief Start the BLE service */
bool IdentificationService::start()
{
    bool ret = true;

    // Update characteristics values
    m_software_version.update(IOpenVarioApp::getInstance().getVersion());
    m_software_manufacturer_name.update(IOpenVarioApp::getInstance().getSwManufacturer());
    m_gatt_profile_version.update("1.0");
    m_hardware_version.update(m_device_manager.getHwVersion());
    m_hardware_manufacturer_name.update(m_device_manager.getHwManufacturer());
    m_hardware_serial_number.update(m_device_manager.getHwSerialNumber());
    m_hardware_manufacturing_date.update(m_device_manager.getHwManufacturingDate());
    
    return ret;
}


}
