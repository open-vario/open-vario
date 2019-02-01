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

#include "ConfigurationService.h"
#include "ConfigManager.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
ConfigurationService::ConfigurationService(ConfigManager& config_manager)
: OpenVarioBleServiceBase()
, m_config_manager(config_manager)

, m_configuration_service("Configuration service", "547d8117-3697-40ad-bf47-a8d6b6d3ae18")

, m_command("Command", "614ab7ea-1659-4ae6-8db8-cf1e628c04ed", true, IBleCharacteristic::PROP_WRITE)
, m_value("Value", "4985f149-be37-4515-a582-4566eca2d1e7", 64u, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_WRITE | IBleCharacteristic::PROP_NOTIFY)

, m_cmd(CMD_RD_PARAM_GROUP_COUNT)
, m_cmd_available(true)
, m_write_done(false)
, m_write_result(false)

, m_current_group(nullptr)
, m_current_value(nullptr)
{}


/** \brief Initialize the BLE service */
bool ConfigurationService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_configuration_service.addCharacteristic(m_command);
    ret = ret && m_configuration_service.addCharacteristic(m_value);

    return ret;
}

/** \brief Start the BLE service */
bool ConfigurationService::start()
{
    bool ret = true;

    // Register to characteristics notifications
    ret = m_command.registerListener(*this);
    ret = ret && m_value.registerListener(*this);
    if (ret)
    {
        // Update characteristics values
        updateCharacteristicsValues();
    }
    
    return ret;
}

/** \brief Update the BLE service characteristics values */
void ConfigurationService::updateCharacteristicsValues()
{
    bool ret = true;
    bool display_ret = false;

    // Check if a command is available
    if (m_cmd_available)
    {
        // Reset flag
        m_cmd_available = false;

        // Handle cmd
        switch (m_cmd)
        {
            case CMD_RESET_TO_DEFAULT:
            {
                m_config_manager.resetToDefault();
                display_ret = true;
                break;
            }

            case CMD_SAVE_TO_NV_MEM:
            {
                ret = m_config_manager.store();
                display_ret = true;
                break;
            }

            case CMD_RD_PARAM_GROUP_COUNT:
            {
                const uint8_t count = static_cast<uint8_t>(m_config_manager.getConfigValueGroupCount());
                m_value.update(reinterpret_cast<const uint8_t*>(&count), sizeof(count));
                break;
            }

            case CMD_RD_PARAM_GROUP_NAME:
            {
                if (m_current_group != nullptr)
                {
                    m_value.update(reinterpret_cast<const uint8_t*>(m_current_group->name()), NANO_STL_STRNLEN(m_current_group->name(), m_value.valueLength()));
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_COUNT:
            {
                if (m_current_group != nullptr)
                {
                    const uint8_t count = static_cast<uint8_t>(m_current_group->getCount());
                    m_value.update(reinterpret_cast<const uint8_t*>(&count), sizeof(count));
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_NAME:
            {
                if (m_current_value != nullptr)
                {
                    m_value.update(reinterpret_cast<const uint8_t*>(m_current_value->name()), NANO_STL_STRNLEN(m_current_value->name(), m_value.valueLength()));
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_SIZE:
            {
                if (m_current_value != nullptr)
                {
                    const uint8_t size = static_cast<uint8_t>(m_current_value->size());
                    m_value.update(reinterpret_cast<const uint8_t*>(&size), sizeof(size));
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_TYPE:
            {
                if (m_current_value != nullptr)
                {
                    m_value.update(reinterpret_cast<const uint8_t*>(m_current_value->type()), NANO_STL_STRNLEN(m_current_value->type(), m_value.valueLength()));
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_VALUE:
            {
                if (m_current_value != nullptr)
                {
                    m_current_group->lock();
                    m_value.update(m_current_value->buffer(), m_current_value->size());
                    m_current_group->unlock();
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_MIN_VALUE:
            {
                if ((m_current_value != nullptr) && (m_current_value->hasMinMax()))
                {
                    if ((NANO_STL_STRNCMP(m_current_value->type(), "string", 6u) == 0u) ||
                        (m_current_value->type()[NANO_STL_STRNLEN(m_current_value->type(), 255u) - 1u] == ']'))
                    {
                        m_value.update(m_current_value->min(), sizeof(uint8_t));
                    }
                    else
                    {
                        m_value.update(m_current_value->min(), m_current_value->size());
                    }
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            case CMD_RD_PARAM_MAX_VALUE:
            {
                if ((m_current_value != nullptr) && (m_current_value->hasMinMax()))
                {
                    if ((NANO_STL_STRNCMP(m_current_value->type(), "string", 6u) == 0u) ||
                        (m_current_value->type()[NANO_STL_STRNLEN(m_current_value->type(), 255u) - 1u] == ']'))
                    {
                        m_value.update(m_current_value->max(), sizeof(uint8_t));
                    }
                    else
                    {
                        m_value.update(m_current_value->max(), m_current_value->size());
                    }
                }
                else
                {
                    ret = false;
                    display_ret = true;
                }
                break;
            }

            default:
            {
                if ((m_cmd & CMD_SET_GROUP_PARAM) == CMD_SET_GROUP_PARAM)
                {
                    // Extract group and id
                    const uint16_t group = static_cast<uint16_t>((m_cmd >> 16u) & 0x00FFu);
                    const uint16_t id = static_cast<uint16_t>((m_cmd >> 0u) & 0x00FFu);
                    ret = m_config_manager.getConfigValueGroup(group, m_current_group);
                    if (ret)
                    {
                        ret = m_current_group->getConfigValue(id, m_current_value);
                    }
                    display_ret = true;
                }
                else
                {
                    m_value.update(reinterpret_cast<const uint8_t*>("Invalid command"), 15u);
                }
                break;
            }
        }
    }

    // Check if a write operation has been done
    if (m_write_done)
    {
        // Reset flag
        m_write_done = false;

        // Display result
        ret = m_write_result;
        display_ret = true;
    }

    // Command result
    if (display_ret)
    {
        if (ret)
        {
            m_value.update(reinterpret_cast<const uint8_t*>("Success"), 7u);
        }
        else
        {
            m_value.update(reinterpret_cast<const uint8_t*>("Failure"), 7u);
        }
        
    }
}

/** \brief Called when the characteristic's value has changed */
void ConfigurationService::onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size)
{
    // Only handle modifications mades by the client
    if (from_stack)
    {
        // Check which characteristic hsa been written
        if (&characteristic == static_cast<IBleCharacteristic*>(&m_command))
        {
            // Save command
            if (new_value_size == m_command.valueLength())
            {
                m_cmd = *reinterpret_cast<const Command*>(new_value);
                m_cmd_available = true;
                triggerAsyncUpdate();
            }
        }
        else
        {
            // Write configuration value
            m_write_result = false;
            if ((m_current_value != nullptr) &&
                (new_value_size <= m_current_value->size()))
            {
                m_current_group->lock();
                m_write_result = m_current_value->set(reinterpret_cast<const uint8_t*>(new_value));
                m_current_group->unlock();
            }
            m_write_done = true;
            triggerAsyncUpdate();
        }
        
    }
}

}
