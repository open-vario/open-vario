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

#include <time.h>

namespace open_vario
{


/** \brief Constructor */
FlagSet::FlagSet(const uint32_t initial_mask)
: m_cs()
, m_cond()
, m_flags(initial_mask)
{
    pthread_mutex_init(&m_cs, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}


/** \brief Reset flags in the flag set */
bool FlagSet::reset(const uint32_t flag_mask)
{
    pthread_mutex_lock(&m_cs);
    m_flags &= ~flag_mask;
    pthread_mutex_unlock(&m_cs);

    return true;
}

/** \brief Set flags in the flag set */
bool FlagSet::set(const uint32_t flag_mask, const bool from_isr)
{
    (void)from_isr;

    pthread_mutex_lock(&m_cs);
    m_flags |= flag_mask;
    pthread_cond_broadcast(&m_cond);
    pthread_mutex_unlock(&m_cs);

    return true;
}

/** \brief Wait for flags in the flag set */
bool FlagSet::wait(uint32_t& flag_mask, const bool reset_flags, const uint32_t timeout)
{
    bool ret = false;

    struct timespec abstimeout;
    clock_gettime(CLOCK_REALTIME, &abstimeout);
    abstimeout.tv_sec += static_cast<time_t>(timeout / 1000u);
    abstimeout.tv_nsec += static_cast<long>((timeout % 1000u) * 1000000u);
    abstimeout.tv_sec += abstimeout.tv_nsec / 1000000;
    abstimeout.tv_nsec = abstimeout.tv_nsec % 1000000;

    pthread_mutex_lock(&m_cs);
    uint32_t active_flags;
    do
    {
        active_flags = m_flags & flag_mask;
        if (active_flags == 0u) 
        {
            const int success = (pthread_cond_timedwait(&m_cond, &m_cs, &abstimeout) == 0);
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
    pthread_mutex_unlock(&m_cs);

    return ret;
}


}
