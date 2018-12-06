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

#include "FlagSet.h"

namespace open_vario
{


/** \brief Constructor */
FlagSet::FlagSet(const uint32_t initial_mask)
: m_cs()
, m_cond()
, m_flags(initial_mask)
{
    InitializeCriticalSection(&m_cs);
    InitializeConditionVariable(&m_cond);
}


/** \brief Reset flags in the flag set */
bool FlagSet::reset(const uint32_t flag_mask)
{
    EnterCriticalSection(&m_cs);
    m_flags &= ~flag_mask;
    LeaveCriticalSection(&m_cs);

    return true;
}

/** \brief Set flags in the flag set */
bool FlagSet::set(const uint32_t flag_mask, const bool from_isr)
{
    (void)from_isr;

    EnterCriticalSection(&m_cs);
    m_flags |= flag_mask;
    WakeAllConditionVariable(&m_cond);
    LeaveCriticalSection(&m_cs);

    return true;
}

/** \brief Wait for flags in the flag set */
bool FlagSet::wait(uint32_t& flag_mask, const bool reset_flags, const uint32_t timeout)
{
    bool ret = false;

    EnterCriticalSection(&m_cs);
    uint32_t active_flags;
    do
    {
        active_flags = m_flags & flag_mask;
        if (active_flags == 0u) 
        {
            const BOOL success = SleepConditionVariableCS(&m_cond, &m_cs, timeout);
            ret = (success != 0);
        }
        else
        {
            ret = true;
        }
    }
    while (ret && (active_flags == 0u));
    if (ret)
    {
        if (reset_flags)
        {
            m_flags &= ~(active_flags);
        }
    }
    flag_mask = active_flags;
    LeaveCriticalSection(&m_cs);

    return ret;
}


}
