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

#include "EepromConfigAreaAccessor.h"
#include "IConfigValue.h"

namespace open_vario
{


/** \brief Constructor */
EepromConfigAreaAccessor::EepromConfigAreaAccessor(EepromPartition& eeprom_partition, ICrc32& crc32)
: m_eeprom(eeprom_partition)
, m_crc32(crc32)
{}

/** \brief Load the configuration from the configuration area */
bool EepromConfigAreaAccessor::load(const uint16_t config_version, const nano_stl::IArray<IConfigValueGroup*>& config_value_groups, bool& compatibility_mode)
{
    bool ret;
    uint8_t temp_buffer[128u];

    // Reset CRC-32 unit
    m_crc32.reset();

    // Read header
    ConfigHeader config_header;
    ret = m_eeprom.read(0u, &config_header, sizeof(ConfigHeader));
    if (ret)
    {
        // Check magic
        ret = (config_header.magic == ConfigHeader::MAGIC_VALUE);
        if (ret)
        {
            // Update CRC-32
            m_crc32.update(&config_header.magic, sizeof(config_header.magic));
            m_crc32.update(&config_header.config_version, sizeof(config_header.config_version));
            m_crc32.update(&config_header.config_value_groups_count, sizeof(config_header.config_value_groups_count));
            
            // Compatibility mode
            compatibility_mode = (config_header.config_version != config_version);
        }
    }

    // Read configuration groups
    uint16_t offset = sizeof(ConfigHeader);
    for (uint16_t i = 0; (i < config_header.config_value_groups_count) && ret; i++)
    {
        // Read header
        ConfigValueGroupHeader config_value_group_header;
        ret = read(&config_value_group_header, sizeof(config_value_group_header), offset);
        if (ret)
        {
            // Look for corresponding group
            if (config_value_group_header.id < config_value_groups.getCount())
            {
                // Read values
                IConfigValueGroup* config_value_group = config_value_groups[config_value_group_header.id];
                if (config_value_group != nullptr)
                {
                    for (uint16_t j = 0; (j < config_value_group_header.count) && ret; j++)
                    {
                        ConfigValueHeader config_value_header;
                        ret = read(&config_value_header, sizeof(config_value_header), offset);
                        if (ret)
                        {
                            // Look for corresponding value
                            IConfigValue* config_value = nullptr;
                            if (config_value_group->getConfigValue(config_value_header.id, config_value) && 
                                (config_value->size() == config_value_header.size))
                            {
                                // Read data
                                ret = read(temp_buffer, config_value_header.size, offset);
                                if (ret)
                                {
                                    config_value->set(temp_buffer);
                                }
                            }
                            else
                            {
                                // Invalid size, skip value
                                uint8_t left = config_value_header.size;
                                compatibility_mode = true;
                                do
                                {
                                    uint8_t size = left;
                                    if (left > sizeof(temp_buffer))
                                    {
                                        size = sizeof(temp_buffer);
                                    }
                                    ret = read(temp_buffer, size, offset);
                                    left -= size;
                                } while (ret && (left != 0));
                            }
                        }
                    }
                }
                else
                {
                    // Invalid group id
                    ret = false;
                }
            }
            else
            {
                // Unknown group, skip values
                compatibility_mode = true;
                for (uint16_t j = 0; (j < config_value_group_header.count) && ret; j++)
                {
                    ConfigValueHeader config_value_header;
                    ret = read(&config_value_header, sizeof(config_value_header), offset);
                    if (ret)
                    {
                        if (config_value_header.size <= sizeof(temp_buffer))
                        {
                            ret = read(temp_buffer, config_value_header.size, offset);
                        }
                        else
                        {
                            ret = false;
                        }
                        
                    }
                }
            }
        }
    }
    
    // Check CRC-32
    if (ret)
    {
        // TODO : find why CRC is always invalid
        //ret = (m_crc32.value() == config_header.crc32);
    }

    return ret;
}

/** \brief Store the configuration to the configuration area */
bool EepromConfigAreaAccessor::store(const uint16_t config_version, const nano_stl::IArray<IConfigValueGroup*>& config_value_groups)
{
    bool ret = true;

    // Reset CRC-32 unit
    m_crc32.reset();

    // Prepare header
    ConfigHeader config_header;
    config_header.magic = ConfigHeader::MAGIC_VALUE;
    config_header.config_version = config_version;
    config_header.config_value_groups_count = 0u;
    for (nano_stl::nano_stl_size_t i = 0; (i < config_value_groups.getCount()) && ret; i++)
    {
        IConfigValueGroup* config_value_group = config_value_groups[i];
        if (config_value_group != nullptr)
        {
            config_header.config_value_groups_count++;
        }
    }
    m_crc32.update(&config_header.magic, sizeof(config_header.magic));
    m_crc32.update(&config_header.config_version, sizeof(config_header.config_version));
    m_crc32.update(&config_header.config_value_groups_count, sizeof(config_header.config_value_groups_count));
    
    // Write configuration value groups into Eeprom
    uint16_t offset = sizeof(ConfigHeader);
    for (nano_stl::nano_stl_size_t i = 0; (i < config_value_groups.getCount()) && ret; i++)
    {
        IConfigValueGroup* config_value_group = config_value_groups[i];
        if (config_value_group != nullptr)
        {
            // Write header
            ConfigValueGroupHeader config_value_group_header;
            config_value_group_header.id = config_value_group->id();
            config_value_group_header.count = config_value_group->getCount();
            ret = write(&config_value_group_header, sizeof(config_value_group_header), offset);
            if (ret)
            {
                // Write configuration values into Eeprom
                for (nano_stl::nano_stl_size_t j = 0; (j < config_value_group->getCount()) && ret; j++)
                {
                    IConfigValue* config_value;
                    config_value_group->getConfigValue(j, config_value);

                    // Write header
                    ConfigValueHeader config_value_header;
                    config_value_header.id = config_value->id();
                    config_value_header.size = static_cast<uint8_t>(config_value->size());
                    ret = write(&config_value_header, sizeof(config_value_header), offset);
                    if (ret)
                    {
                        // Write data
                        ret = write(config_value->buffer(), config_value->size(), offset);
                    }
                }
            }
        }
    }

    // Write header
    if (ret)
    {
        config_header.crc32 = m_crc32.value();
        ret = m_eeprom.write(0u, &config_header, sizeof(config_header));
    }

    return ret;
}

/** \brief Read data from the Eeprom */
bool EepromConfigAreaAccessor::read(void* data, const size_t size, uint16_t& offset)
{
    bool ret = false;

    // Read data
    ret = m_eeprom.read(offset, data, static_cast<uint16_t>(size));
    if (ret)
    {
        offset += static_cast<uint16_t>(size);
        m_crc32.update(data, size);
    }

    return ret;
}

/** \brief Write data into the Eeprom */
bool EepromConfigAreaAccessor::write(const void* data, const size_t size, uint16_t& offset)
{
    bool ret = false;

    // Write data
    ret = m_eeprom.write(offset, data, static_cast<uint16_t>(size));
    if (ret)
    {
        offset += static_cast<uint16_t>(size);
        m_crc32.update(data, size);
    }

    return ret;
}

}
