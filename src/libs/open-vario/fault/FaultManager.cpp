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

#include "FaultManager.h"
#include "LogMacro.h"

namespace open_vario
{


/** \brief Constructor */
FaultManager::FaultManager(TimeManager& time_manager)
: m_time_manager(time_manager)
, m_mutex()
, m_event()
, m_active_fault_count(0u)
, m_faults()
{
}

/** \brief Initialize the fault manager */
bool FaultManager::init()
{
    bool ret = true;

    LOG_INFO("Initializing fault manager...");

    /* Initialize faults */
    for (uint8_t i = 0; i < m_faults.getCount(); i++)
    {
        m_faults[i].id = i;
        m_faults[i].active = false;
    }

    return ret;
}

/** \brief Raise a fault */
bool FaultManager::raise(const uint8_t id)
{
    const uint8_t context[Fault::MAX_CONTEXT_SIZE] = {0};
    return raise(id, context);
}

/** \brief Raise a fault */
bool FaultManager::raise(const uint8_t id, const uint8_t context[])
{
    bool ret = false;

    /* Check id */
    if (id < m_faults.getCount())
    {
        m_mutex.lock();

        /* Get corresponding fault and check state */
        Fault& fault = m_faults[id];
        if (!fault.active)
        {
            /* Update fault state */
            ret = m_time_manager.getDateTime(fault.timestamp);
            if (ret)
            {
                fault.active = true;
                NANO_STL_MEMCPY(fault.context, context, Fault::MAX_CONTEXT_SIZE);
                m_active_fault_count++;

                /* Trigger event */
                m_event.trigger(fault);
            }
        }

        m_mutex.unlock();
    }

    return ret;
}

/** \brief Drop a fault */
bool FaultManager::drop(const uint8_t id)
{
    const uint8_t context[Fault::MAX_CONTEXT_SIZE] = {0};
    return drop(id, context);
}

/** \brief Drop a fault */
bool FaultManager::drop(const uint8_t id, const uint8_t context[])
{
    bool ret = false;

    /* Check id */
    if (id < m_faults.getCount())
    {
        m_mutex.lock();

        /* Get corresponding fault and check state */
        Fault& fault = m_faults[id];
        if (fault.active)
        {
            /* Update fault state */
            ret = m_time_manager.getDateTime(fault.timestamp);
            if (ret)
            {
                fault.active = true;
                NANO_STL_MEMCPY(fault.context, context, Fault::MAX_CONTEXT_SIZE);
                m_active_fault_count--;

                /* Trigger event */
                m_event.trigger(fault);
            }
        }

        m_mutex.unlock();
    }

    return ret;
}

/** \brief Indicate if a fault is active */
bool FaultManager::isActive(const uint8_t id) const
{
    bool ret = false;

    /* Check id */
    if (id < m_faults.getCount())
    {
        const Fault& fault = m_faults[id];
        ret = fault.active;
    }

    return ret;
}

}
