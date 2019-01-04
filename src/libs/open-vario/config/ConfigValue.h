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

#ifndef CONFIGVALUE_H
#define CONFIGVALUE_H

#include "IConfigValue.h"

namespace open_vario
{


/** \brief Base class for all configuration values implementations */
template <typename T>
class ConfigValue : public IConfigValue
{
    public:


        /** \brief Constructor */
        ConfigValue(const uint16_t id, const char* const name, const T& default_value, const bool reset_only)
        : m_id(id)
        , m_name(name)
        , m_value(default_value)
        , m_default_value(default_value)
        , m_reset_only(reset_only)
        , m_listener(NULL)
        {}


        /** \brief Get the value id */
        virtual uint16_t id() { return m_id; }

        /** \brief Get the value name */
        virtual const char* name() const { return m_name; }

        /** \brief Get the value type name */
        virtual const char* type() const { return configvalue_type<T>(); }

        /** \brief Get the buffer representing the value */
        virtual const uint8_t* buffer() const { return reinterpret_cast<const uint8_t*>(&m_value); }

        /** \brief Get the size in bytes of the buffer representing the value */
        virtual uint32_t size() const { return configvalue_size<T>(); }

        /** \brief Set the value from a buffer */
        virtual bool set(const uint8_t* const buffer) 
        { 
            const T& value = *reinterpret_cast<const T*>(buffer); 
            m_value = value; 
            if (m_listener != NULL)
            {
                m_listener->onConfigValueChange(*this);
            }
            return true; 
        }

        /** \brief Copy the value to a buffer */
        virtual bool get(uint8_t* const buffer) const { T& value = *reinterpret_cast<T*>(buffer); value = m_value; return true; }

        /** \brief Reset the value to its default value */
        virtual void reset() { m_value = m_default_value; }

        /** \brief Indicate if the value has a min and max value */
        virtual bool hasMinMax() const { return ((min() != NULL) && (max() != NULL)); }

        /** \brief Get the buffer representing the min value */
        virtual const uint8_t* min() const { return NULL; }

        /** \brief Get the buffer representing the max value */
        virtual const uint8_t* max() const { return NULL; }

        /** \brief Indicate if the value will be taken into account only after a reset */
        virtual bool isResetOnly() const { return m_reset_only; }

        /** \brief Register a listener to a configuration value change event */
        virtual bool registerListener(IConfigValueListener& listener) 
        { 
            bool ret = false;
            if (m_listener == NULL)
            {
                m_listener = &listener;
                ret = true;
            }
            return ret;
        }


    protected:


        /** \brief Get the current value */
        T& value() { return m_value; }

        /** \brief Get the defaut value */
        const T& defaultValue() const { return m_default_value; }

        /** \brief Get the listener */
        IConfigValueListener* listener() { return m_listener; }


    private:

        /** \brief Id */
        const uint16_t m_id;

        /** \brief Name */
        const char* const m_name;

        /** \brief Value */
        T m_value;

        /** \brief Default value */
        const T m_default_value;

        /** \brief Indicate if the value will be taken into account only after a reset */
        const bool m_reset_only;

        /** \brief Listener */
        IConfigValueListener* m_listener;
};


/** \brief Base class for all string configuration values implementations */
template <uint32_t MAX_STRING_SIZE>
class StringConfigValue : public IConfigValue
{
    public:

        /** \brief Constructor */
        StringConfigValue(const uint16_t id, const char* const name, const char* default_value, const bool reset_only)
        : m_id(id)
        , m_name(name)
        , m_value()
        , m_default_value(default_value)
        , m_reset_only(reset_only)
        , m_listener(NULL)
        , m_min_size(0u)
        , m_max_size(MAX_STRING_SIZE)
        {
            NANO_STL_STRNCPY(m_value, m_default_value, MAX_STRING_SIZE);
        }


                /** \brief Get the value id */
        virtual uint16_t id() { return m_id; }

        /** \brief Get the value name */
        virtual const char* name() const { return m_name; }

        /** \brief Get the value type name */
        virtual const char* type() const { return "string"; }

        /** \brief Get the buffer representing the value */
        virtual const uint8_t* buffer() const { return reinterpret_cast<const uint8_t*>(m_value); }

        /** \brief Get the size in bytes of the buffer representing the value */
        virtual uint32_t size() const { return MAX_STRING_SIZE; }

        /** \brief Set the value from a buffer */
        virtual bool set(const uint8_t* const buffer) 
        { 
            strncpy(m_value, reinterpret_cast<const char*>(buffer), MAX_STRING_SIZE); 
            if (m_listener != NULL)
            {
                m_listener->onConfigValueChange(*this);
            }
            return true; 
        }

        /** \brief Copy the value to a buffer */
        virtual bool get(uint8_t* const buffer) const { strncpy(reinterpret_cast<char*>(buffer), m_value, MAX_STRING_SIZE); return true; }

        /** \brief Reset the value to its default value */
        virtual void reset() { strncpy(m_value, m_default_value, MAX_STRING_SIZE); }

        /** \brief Indicate if the value has a min and max value */
        virtual bool hasMinMax() const { return ((min() != NULL) && (max() != NULL)); }

        /** \brief Get the buffer representing the min value */
        virtual const uint8_t* min() const { return reinterpret_cast<const uint8_t*>(&m_min_size); }

        /** \brief Get the buffer representing the max value */
        virtual const uint8_t* max() const { return reinterpret_cast<const uint8_t*>(&m_max_size); }

        /** \brief Indicate if the value will be taken into account only after a reset */
        virtual bool isResetOnly() const { return m_reset_only; }

        /** \brief Register a listener to a configuration value change event */
        virtual bool registerListener(IConfigValueListener& listener) 
        { 
            bool ret = false;
            if (m_listener == NULL)
            {
                m_listener = &listener;
                ret = true;
            }
            return ret;
        }


    private:

        /** \brief Id */
        const uint16_t m_id;

        /** \brief Name */
        const char* const m_name;

        /** \brief Value */
        char m_value[MAX_STRING_SIZE + 1u];

        /** \brief Default value */
        const char* m_default_value;

        /** \brief Indicate if the value will be taken into account only after a reset */
        const bool m_reset_only;

        /** \brief Listener */
        IConfigValueListener* m_listener;

        /** \brief Minimum string size */
        const uint32_t m_min_size;

        /** \brief Maximum string size */
        const uint32_t m_max_size; 
};


}

#endif // CONFIGVALUE_H
