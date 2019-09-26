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

#ifndef BLACKBOX_H
#define BLACKBOX_H


#include "nano-stl-conf.h"
#include "TimeManager.h"
#include "Mutex.h"


namespace open_vario
{

class EepromPartition;


/** \brief Blackbox manager */
class BlackBox
{
    public:


        /** \brief Blackbox entry */
        struct Entry
        {
            /** \brief Maximum context size */
            static const uint8_t MAX_CONTEXT_SIZE = 12u;

            /** \brief Number */
            uint16_t number;

            /** \brief Timestamp */
            IRtc::DateTime timestamp;

            /** \brief Id */
            uint16_t id;
            /** \bried Context */
            uint8_t context[MAX_CONTEXT_SIZE];
        };



        /** \brief Constructor */
        BlackBox(TimeManager& time_manager, EepromPartition& eeprom);


        /** \brief Initialize the blackbox */
        bool init();

        /** \brief Write data into the blackbox */
        bool write(const uint16_t id);

        /** \brief Write data into the blackbox */
        bool write(const uint16_t id, const uint8_t context[]);

        

    private:

        /** \brief Time manager */
        TimeManager& m_time_manager;

        /** \brief EEPROM partition to store blackbox data into */
        EepromPartition& m_eeprom;

        /** \brief Mutex for blackbox concurrent accesses */
        Mutex m_mutex;

        /** \brief Maximum number of entries that can be stored */
        const uint16_t m_max_entry_count;

        /** \brief Current entry number */
        uint16_t m_current_entry_number;

        /** \brief Current entry index */
        uint16_t m_current_entry;

        /** \brief Oldest entry index */
        uint16_t m_oldest_entry;

        /** \brief Current entry count */
        uint16_t m_entry_count;



        /** \brief Black box header */
        static const uint32_t BB_HEADER;
        /** \brief Black box footer */
        static const uint32_t BB_FOOTER;


        /** \brief Read an entry in the black box */
        bool readEntry(const uint16_t index, Entry& entry);

        /** \brief Format the blackbox area into EEPROM */
        bool format();

};

}

#endif // BLACKBOX_H
