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

#ifndef IDENTIFICATIONSERVICE_H
#define IDENTIFICATIONSERVICE_H

#include "IOpenVarioBleService.h"

#include "BleService.h"
#include "BleCharacteristic.h"


namespace open_vario
{

class DeviceManager;


/** \brief BLE identification service */
class IdentificationService : public IOpenVarioBleService
{
    public:

        /** \brief Constructor */
        IdentificationService(DeviceManager& device_manager);


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues() {}

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_identification_service; } 
   

    private:


        /** \brief Device manager */
        DeviceManager& m_device_manager;



        /** \brief Identification service */
        BleService128<0u, 7u> m_identification_service;

        /** \brief Software version */
        BleCharacteristic128<char*, 0u> m_software_version;

        /** \brief Software manufacturer name */
        BleCharacteristic128<char*, 0u> m_software_manufacturer_name;

        /** \brief GATT profile version */
        BleCharacteristic128<char*, 0u> m_gatt_profile_version;

        /** \brief Hardware version */
        BleCharacteristic128<char*, 0u> m_hardware_version;

        /** \brief Hardware manufacturer name */
        BleCharacteristic128<char*, 0u> m_hardware_manufacturer_name;

        /** \brief Hardware manufacturer name */
        BleCharacteristic128<char*, 0u> m_hardware_serial_number;

        /** \brief Hardware manufacturing date */
        BleCharacteristic128<char*, 0u> m_hardware_manufacturing_date;

};

}

#endif // IDENTIFICATIONSERVICE_H
