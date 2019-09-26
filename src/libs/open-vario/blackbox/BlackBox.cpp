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

#include "BlackBox.h"
#include "EEpromPartition.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Black box header */
const uint32_t BlackBox::BB_HEADER = 0x8BADF00Du;
/** \brief Black box footer */
const uint32_t BlackBox::BB_FOOTER = 0xDEADBEEF;


/** \brief Constructor */
BlackBox::BlackBox(TimeManager& time_manager, EepromPartition& eeprom)
: m_time_manager(time_manager)
, m_eeprom(eeprom)
, m_mutex()
, m_max_entry_count((eeprom.getSize() - (sizeof(BB_HEADER) + sizeof(BB_FOOTER))) / sizeof(Entry))
, m_current_entry_number(1u)
, m_current_entry(0u)
, m_oldest_entry(0u)
, m_entry_count(0u)
{
}

/** \brief Initialize the blackbox */
bool BlackBox::init()
{
    bool ret;

    LOG_INFO("Initializing blackbox...");

    // Check if eeprom area has been initialized
    uint32_t header = 0;
    uint32_t footer = 0;
    ret = m_eeprom.read(0u, &header, sizeof(header));
    ret = ret && m_eeprom.read(m_eeprom.getSize() - sizeof(footer), &footer, sizeof(footer));
    if (ret)
    {
        if ((header == BB_HEADER) && (footer == BB_FOOTER))
        {
            // Look for the last entry
            Entry entry;
            uint16_t index = 0u;
            uint16_t last_number = 0u;
            do
            {
                ret = readEntry(index, entry);
                if (ret && (entry.id != 0xFFFFu))
                {
                    if (entry.number > last_number)
                    {
                        m_current_entry_number = entry.number + 1u;
                        m_current_entry = index + 1u;
                        if (m_current_entry == m_max_entry_count)
                        {
                            m_current_entry = 0u;
                        }
                        m_entry_count++;
                    }
                    else
                    {
                        m_oldest_entry = index;
                        m_current_entry = index;
                        m_entry_count = m_max_entry_count;
                        m_current_entry_number = last_number + 1u;
                        if (m_current_entry_number == 0u)
                        {
                            m_current_entry_number++;
                        }
                    }
                    
                    last_number = entry.number;
                    index++;
                }
            } 
            while (ret && (entry.id != 0xFFFFu) && (m_entry_count != m_max_entry_count));
            
        }
        else
        {
            ret = format();
        }        
    }
    if (ret)
    {
        LOG_INFO("%d entries found!", m_entry_count);
    }

    return ret;
}

/** \brief Write data into the blackbox */
bool BlackBox::write(const uint16_t id)
{
    static const uint8_t context[Entry::MAX_CONTEXT_SIZE] = {0};
    return write(id, context);
}

/** \brief Write data into the blackbox */
bool BlackBox::write(const uint16_t id, const uint8_t context[])
{
    bool ret;

    Entry entry = {0};
    ret = m_time_manager.getDateTime(entry.timestamp);
    if (ret)
    {
        m_mutex.lock();

        entry.number = m_current_entry_number;
        entry.id = id;
        NANO_STL_MEMCPY(entry.context, context, Entry::MAX_CONTEXT_SIZE);
        ret = m_eeprom.write(sizeof(BB_HEADER) + m_current_entry * sizeof(Entry), &entry, sizeof(Entry));
        if (ret)
        {
            m_current_entry_number++;
            if (m_current_entry_number == 0u)
            {
                m_current_entry_number++;
            }
            m_current_entry++;
            if (m_current_entry == m_max_entry_count)
            {
                m_current_entry = 0u;
            }
            if (m_entry_count == m_max_entry_count)
            {
                m_oldest_entry++;
                if (m_oldest_entry == m_max_entry_count)
                {
                    m_oldest_entry = 0u;
                }
            }
            else
            {
                m_entry_count++;
            }            
        }

        m_mutex.unlock();
    }

    return ret;
}

/** \brief Read an entry in the black box */
bool BlackBox::readEntry(const uint16_t index, Entry& entry)
{
    bool ret;

    ret = m_eeprom.read(sizeof(BB_HEADER) + index * sizeof(Entry), &entry, sizeof(Entry));

    return ret;
}

/** \brief Format the blackbox area into EEPROM */
bool BlackBox::format()
{
    bool ret = true;

    m_mutex.lock();

    LOG_INFO("Formatting blackbox...");

    uint16_t index = 0u;
    const uint32_t empty_value = 0xFFFFFFFFu;
    const uint16_t eeprom_size_rounded = m_eeprom.getSize() & 0xFFC0;

    while (ret && (index < eeprom_size_rounded))
    {
        ret = m_eeprom.write(index, &empty_value, sizeof(empty_value));
        index += sizeof(empty_value);
    }
    if (ret)
    {
        ret = m_eeprom.write(0u, &BB_HEADER, sizeof(BB_HEADER));
        ret = ret && m_eeprom.write(m_eeprom.getSize() - sizeof(BB_FOOTER), &BB_FOOTER, sizeof(BB_FOOTER));

        m_current_entry = 0u;
        m_oldest_entry = 0u;
        m_current_entry_number = 1u;
        m_entry_count = 0u;
    }

    m_mutex.unlock();

    return ret;
}

}
