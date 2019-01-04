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

#ifndef ICONFIGVALUE_H
#define ICONFIGVALUE_H

#include <cstdint>

namespace open_vario
{

class IConfigValue;

/** \brief Interface for all configuration values listeners implementations */
class IConfigValueListener
{
    public:

        /** \brief Called when a configuration value has been modified */
        virtual void onConfigValueChange(const IConfigValue& config_value) = 0;
};


/** \brief Interface for all configuration values implementations */
class IConfigValue
{
    public:

        /** \brief Get the value id */
        virtual uint16_t id() = 0;
        
        /** \brief Get the value name */
        virtual const char* name() const = 0; 

        /** \brief Get the value type name */
        virtual const char* type() const = 0; 

        /** \brief Get the buffer representing the value */
        virtual const uint8_t* buffer() const = 0;

        /** \brief Get the size in bytes of the buffer representing the value */
        virtual uint32_t size() const = 0;

        /** \brief Set the value from a buffer */
        virtual bool set(const uint8_t* const buffer) = 0;

        /** \brief Copy the value to a buffer */
        virtual bool get(uint8_t* const buffer) const = 0;

        /** \brief Reset the value to its default value */
        virtual void reset() = 0;

        /** \brief Indicate if the value has a min and max value */
        virtual bool hasMinMax() const = 0;

        /** \brief Get the buffer representing the min value */
        virtual const uint8_t* min() const = 0;

        /** \brief Get the buffer representing the max value */
        virtual const uint8_t* max() const = 0;

        /** \brief Indicate if the value will be taken into account only after a reset */
        virtual bool isResetOnly() const = 0;

        /** \brief Register a listener to a configuration value change event */
        virtual bool registerListener(IConfigValueListener& listener) = 0;

        
        /** \brief Generic accessor to the value */
        template <typename T>
        const T& value() const { return (*reinterpret_cast<const T*>(this->buffer())); }
};


/** \brief Template function for returning a config value type name */
template <typename T>
inline const char* configvalue_type() { return "unknown"; }

/** \brief Template function for returning a config value array type name */
template <typename T>
inline const char* configvalue_array_type() { return "unknown[]"; }

/** \brief Template function for returning the size of a config value */
template <typename T>
inline uint32_t configvalue_size(const T& obj) { return static_cast<uint32_t>(sizeof(T)); }

/** \brief Template function for returning the size of a config value */
template <typename T>
inline uint32_t configvalue_size() { return static_cast<uint32_t>(sizeof(T)); }

/** \brief Macro to stringify a type name */
#define CONFIGVALUE_TYPE_NAME(t) #t

/** \brief Macro to generate the template function for returning a config value type name */
#define CONFIGVALUE_TYPE(t) template<> inline const char* configvalue_type< t >() { return CONFIGVALUE_TYPE_NAME(t); }

/** \brief Macro to generate the template function for returning a config value array type name */
#define CONFIGVALUE_ARRAY_TYPE(t) template<> inline const char* configvalue_array_type< t >() { return CONFIGVALUE_TYPE_NAME(t)"[]"; }

/** \brief Macro to generate the template function for returning a config value type size */
#define CONFIGVALUE_SIZE(t, method) template<> inline uint32_t configvalue_size< t >(const t& obj) { return obj.method(); }

///////////////////////////////////////////////
//           Builtin types names
///////////////////////////////////////////////

CONFIGVALUE_TYPE(bool)
CONFIGVALUE_TYPE(char)
CONFIGVALUE_TYPE(uint8_t)
CONFIGVALUE_TYPE(int8_t)
CONFIGVALUE_TYPE(uint16_t)
CONFIGVALUE_TYPE(int16_t)
CONFIGVALUE_TYPE(uint32_t)
CONFIGVALUE_TYPE(int32_t)
CONFIGVALUE_TYPE(uint64_t)
CONFIGVALUE_TYPE(int64_t)
CONFIGVALUE_TYPE(float)
CONFIGVALUE_TYPE(double)

CONFIGVALUE_TYPE(void*)
CONFIGVALUE_TYPE(bool*)
CONFIGVALUE_TYPE(char*)
CONFIGVALUE_TYPE(uint8_t*)
CONFIGVALUE_TYPE(int8_t*)
CONFIGVALUE_TYPE(uint16_t*)
CONFIGVALUE_TYPE(int16_t*)
CONFIGVALUE_TYPE(uint32_t*)
CONFIGVALUE_TYPE(int32_t*)
CONFIGVALUE_TYPE(uint64_t*)
CONFIGVALUE_TYPE(int64_t*)
CONFIGVALUE_TYPE(float*)
CONFIGVALUE_TYPE(double*)

}

#endif // ICONFIGVALUE_H
