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

#ifndef EEPROMCONFIGAREAACCESSOR_H
#define EEPROMCONFIGAREAACCESSOR_H

#include "IConfigAreaAccessor.h"
#include "EepromPartition.h"
#include "ICrc32.h"

namespace open_vario
{


/** \brief Eeprom configuration area accessor */
class EepromConfigAreaAccessor : public IConfigAreaAccessor
{
    public:

        /** \brief Constructor */
        EepromConfigAreaAccessor(EepromPartition& eeprom_partition, ICrc32& crc32);

        /** \brief Load the configuration from the configuration area */
        virtual bool load(const uint16_t config_version, const nano_stl::IArray<IConfigValueGroup*>& config_value_groups, bool& compatibility_mode);

        /** \brief Store the configuration to the configuration area */
        virtual bool store(const uint16_t config_version, const nano_stl::IArray<IConfigValueGroup*>& config_value_groups);


    private:


        /** \brief Configuration header */
        struct ConfigHeader
        {
            /** \brief CRC32 of the configuration area */
            uint32_t crc32;
            /** \brief Magic value */
            uint32_t magic;
            /** \brief Configuration version */
            uint16_t config_version;
            /** \brief Configuration value groups count */
            uint16_t config_value_groups_count;

            /** \brief Magic value (Ascii word 'CONF') */
            static const uint32_t MAGIC_VALUE = 0x464E4F43u;
        };

        /** \brief Configuration value group header */
        struct ConfigValueGroupHeader
        {
            /** \brief Configuration value group id */
            uint16_t id;
            /** \brief Number of configuration values in the group */
            uint16_t count;
        };

        /** \brief Configuration value header */
        struct ConfigValueHeader
        {
            /** \brief Configuration value id */
            uint16_t id;
            /** \brief Configuration value size */
            uint8_t size;
        };


        /** \brief Eeprom partition */
        EepromPartition& m_eeprom;

        /** \brief CRC-32 unit */
        ICrc32& m_crc32;


        /** \brief Read data from the Eeprom */
        bool read(void* data, const size_t size, uint16_t& offset);

        /** \brief Write data into the Eeprom */
        bool write(const void* data, const size_t size, uint16_t& offset);

};

}

#endif // EEPROMCONFIGAREAACCESSOR_H
