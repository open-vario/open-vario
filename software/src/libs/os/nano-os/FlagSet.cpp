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
: m_flag_set()
{
    NANO_OS_FLAG_SET_Create(&m_flag_set, initial_mask, QT_PRIORITY);
}


/** \brief Reset flags in the flag set */
bool FlagSet::reset(const uint32_t flag_mask)
{
    const nano_os_error_t ret = NANO_OS_FLAG_SET_Clear(&m_flag_set, flag_mask);
    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Set flags in the flag set */
bool FlagSet::set(const uint32_t flag_mask, const bool from_isr)
{
    nano_os_error_t ret;

    if (from_isr)
    {
        ret = NANO_OS_FLAG_SET_SetFromIsr(&m_flag_set, flag_mask);
    }
    else
    {
        ret = NANO_OS_FLAG_SET_Set(&m_flag_set, flag_mask);
    }

    return (ret == NOS_ERR_SUCCESS);
}

/** \brief Wait for flags in the flag set */
bool FlagSet::wait(uint32_t& flag_mask, const bool reset_flags, const uint32_t timeout)
{
    const nano_os_error_t ret = NANO_OS_FLAG_SET_Wait(&m_flag_set, flag_mask, false, reset_flags, &flag_mask, NANO_OS_MS_TO_TICKS(timeout));
    return (ret == NOS_ERR_SUCCESS);
}


}
