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

#include "OpenVarioBleServiceBase.h"

#include "BleService.h"
#include "BleCharacteristic.h"


namespace open_vario
{

class DeviceManager;


/** \brief BLE identification service */
class IdentificationService : public OpenVarioBleServiceBase, public IBleCharacteristicListener
{
    public:

        /** \brief Constructor */
        IdentificationService(DeviceManager& device_manager);


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_identification_service; } 


        /** \brief Called when the characteristic's value has changed */
        virtual void onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size);
   

    private:


        /** \brief Commands */
        enum Command
        {
            /** \brief Read GATT version */
            CMD_RD_GATT_VERSION = 0u,
            /** \brief Read software version */
            CMD_RD_SOFT_VERSION = 1u,
            /** \brief Read software manufacturer name */
            CMD_RD_SOFT_MANUF_NAME = 2u,
            /** \brief Read hardware version */
            CMD_RD_HARD_VERSION = 3u,
            /** \brief Read hardware manufacturer name */
            CMD_RD_HARD_MANUF_NAME = 4u,
            /** \brief Read hardware serial number */
            CMD_RD_HARD_SERIAL_NUMBER = 5u,
            /** \brief Read hardware manufacturer date */
            CMD_RD_HARD_MANUF_DATE = 6u,

            /** \brief Max value */
            CMD_MAX = 7u
        };


        /** \brief Device manager */
        DeviceManager& m_device_manager;



        /** \brief Identification service */
        BleService128<0u, 2u> m_identification_service;

        /** \brief Command */
        BleCharacteristic128<uint8_t, 0u> m_command;

        /** \brief Idendification info */
        BleCharacteristic128<char*, 0u> m_identification_info;


        /** \brief Current command */
        Command m_cmd;

        /** \brief Indicate if the command is available to process */
        bool m_cmd_available;

};

}

#endif // IDENTIFICATIONSERVICE_H
