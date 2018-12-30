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

#ifndef MINMAXCONFIGVALUE_H
#define MINMAXCONFIGVALUE_H

#include "ConfigValue.h"

namespace open_vario
{


/** \brief Class for all configuration values which have a min and a max value */
template <typename T>
class MinMaxConfigValue : public ConfigValue<T>
{
    public:


        /** \brief Constructor */
        MinMaxConfigValue(const uint16_t id, const char* const name, const T& default_value, const T& min_value, const T& max_value, const bool reset_only)
        : ConfigValue<T>(id, name, default_value, reset_only)
        , m_min_value(min_value)
        , m_max_value(max_value)
        {}


        /** \brief Set the value from a buffer */
        virtual bool set(const uint8_t* const buffer) 
        {
            bool ret = false; 
            const T& value = *reinterpret_cast<const T*>(buffer); 
            if ((value >= m_min_value) && (value <= m_max_value))
            {
                this->value() = value;
                ret = true; 
            }
            return ret; 
        }

        /** \brief Get the buffer representing the min value */
        virtual const uint8_t* min() const { return reinterpret_cast<const uint8_t*>(&m_min_value); }

        /** \brief Get the buffer representing the max value */
        virtual const uint8_t* max() const { return reinterpret_cast<const uint8_t*>(&m_max_value); }


    private:

        /** \brief Min value */
        const T m_min_value;

        /** \brief Max value */
        const T m_max_value;
};


}

#endif // MINMAXCONFIGVALUE_H
