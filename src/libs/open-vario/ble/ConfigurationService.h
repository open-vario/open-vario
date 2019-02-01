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

#ifndef CONFIGURATIONSERVICE_H
#define CONFIGURATIONSERVICE_H

#include "OpenVarioBleServiceBase.h"

#include "BleService.h"
#include "BleCharacteristic.h"


namespace open_vario
{

class ConfigManager;
class IConfigValueGroup;
class IConfigValue;


/** \brief BLE configuration service */
class ConfigurationService : public OpenVarioBleServiceBase, public IBleCharacteristicListener
{
    public:

        /** \brief Constructor */
        ConfigurationService(ConfigManager& config_manager);


        /** \brief Initialize the BLE service */
        virtual bool init();

        /** \brief Start the BLE service */
        virtual bool start();

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues();

        /** \brief Get the BLE service */
        virtual IBleService& getService() { return m_configuration_service; } 


        /** \brief Called when the characteristic's value has changed */
        virtual void onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size);
   

    private:


        /** \brief Commands */
        enum Command
        {
            /** \brief Reset to default values */
            CMD_RESET_TO_DEFAULT = 0xFF000000u,
            /** \brief Save to non-volatile memory */
            CMD_SAVE_TO_NV_MEM = 0xFF000001u,
            /** \brief Read total number of parameter groups */
            CMD_RD_PARAM_GROUP_COUNT = 0xFF000002u,
            /** \brief Read current parameter group name */
            CMD_RD_PARAM_GROUP_NAME = 0xFF000003u,
            /** \brief Read total number of parameters in current group */
            CMD_RD_PARAM_COUNT = 0xFF000004u,
            /** \brief Read current parameter name */
            CMD_RD_PARAM_NAME = 0xFF000005u,
            /** \brief Read current parameter size */
            CMD_RD_PARAM_SIZE = 0xFF000006u,
            /** \brief Read current parameter type */
            CMD_RD_PARAM_TYPE = 0xFF000007u,
            /** \brief Read current parameter value */
            CMD_RD_PARAM_VALUE = 0xFF000008u,
            /** \brief Read current parameter min value */
            CMD_RD_PARAM_MIN_VALUE = 0xFF000009u,
            /** \brief Read current parameter max value */
            CMD_RD_PARAM_MAX_VALUE = 0xFF00000Au,
            /** \brief Set the current group and parameter */
            CMD_SET_GROUP_PARAM = 0xAA005500u
        };


        /** \brief Configuration manager */
        ConfigManager& m_config_manager;



        /** \brief Configuration service */
        BleService128<0u, 8u> m_configuration_service;

        /** \brief Command */
        BleCharacteristic128<uint32_t, 0u> m_command;

        /** \brief Number of parameter groups */
        BleCharacteristic128<uint8_t*, 0u> m_value;


        /** \brief Current command */
        Command m_cmd;

        /** \brief Indicate if the command is available to process */
        bool m_cmd_available;

        /** \brief Indicate if a write config operation has been processed */
        bool m_write_done;

        /** \brief Result of the write config operation */
        bool m_write_result;

        /** \brief Current configuration group */
        IConfigValueGroup* m_current_group;

        /** \brief Current configuration value */
        IConfigValue* m_current_value;
};

}

#endif // CONFIGURATIONSERVICE_H
