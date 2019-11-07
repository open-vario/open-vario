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

#ifndef FAULTMANAGER_H
#define FAULTMANAGER_H


#include "EventHelper.h"
#include "StaticArray.h"
#include "TimeManager.h"
#include "Mutex.h"


namespace open_vario
{


/** \brief Fault manager */
class FaultManager
{
    public:


        /** \brief Fault */
        struct Fault
        {
            /** \brief Maximum context size */
            static const uint8_t MAX_CONTEXT_SIZE = 12u;

            /** \brief State */
            bool active;
            /** \brief Id */
            uint8_t id;
            /** \brief Context */
            uint8_t context[MAX_CONTEXT_SIZE];
            /** \brief Timestamp */
            IRtc::DateTime timestamp;


            /** \brief Comparison operator */
            bool operator == (const Fault& compare) const
            {
                return ((active == compare.active) && (id == compare.id) && 
                        (NANO_STL_MEMCMP(context, compare.context, MAX_CONTEXT_SIZE) == 0) &&
                        (NANO_STL_MEMCMP(&timestamp, &compare.timestamp, sizeof(timestamp)) == 0));
            }

            /** \brief Copy operator */
            Fault& operator = (const Fault& copy)
            {
                active = copy.active;
                id = copy.id;
                NANO_STL_MEMCPY(context, copy.context, MAX_CONTEXT_SIZE);
                timestamp = copy.timestamp;

                return *this;
            }

        };



        /** \brief Constructor */
        FaultManager(TimeManager& time_manager);


        /** \brief Initialize the fault manager */
        bool init();
        
        /** \brief Raise a fault */
        bool raise(const uint8_t id);

        /** \brief Raise a fault */
        bool raise(const uint8_t id, const uint8_t context[]);

        /** \brief Clear a fault */
        bool clear(const uint8_t id);

        /** \brief Clear a fault */
        bool clear(const uint8_t id, const uint8_t context[]);


        /** \brief Indicate if a fault is active */
        bool isActive(const uint8_t id) const;

        /** \brief Get the number of active faults */
        uint8_t getActiveCount() const { return m_active_fault_count; }

        /** \brief Get the fault state change event */
        nano_stl::IEvent<const Fault&>& faultChangedEvent() { return m_event; }


    private:

        /** \brief Time manager */
        TimeManager& m_time_manager;

        /** \brief Mutex for blackbox concurrent accesses */
        Mutex m_mutex;

        /** \brief Event triggered when a fault state change */
        nano_stl::EventHelper<2u, const Fault&> m_event;

        /** \brief Number of currently active faults */
        uint8_t m_active_fault_count;


        /** \brief List of all faults */
        nano_stl::StaticArray<Fault, 80u> m_faults;
};

}

#endif // FAULTMANAGER_H
