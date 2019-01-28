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

#ifndef BLECHARACTERISTIC_H
#define BLECHARACTERISTIC_H

#include "IBleCharacteristic.h"
#include "IBleCharacteristicDescriptor.h"
#include "StaticVector.h"
#include "ZeroSizeVector.h"
#include "BleUuid16.h"
#include "BleUuid128.h"

using namespace nano_stl;

namespace open_vario
{


/** \brief Base class for Bluetooth Low Energy characteristics implementations */
class BleCharacteristicBase : public IBleCharacteristic
{
    public:

        /** \brief Constructor */
        BleCharacteristicBase(const char* const name, const IBleUuid& uuid, const uint16_t value_length, const bool is_fixed_length, const uint8_t properties)
        : m_name(name)
        , m_uuid(uuid)
        , m_handle(0x0000u)
        , m_service(nullptr)
        , m_value_length(value_length)
        , m_is_fixed_length(is_fixed_length)
        , m_properties(properties)
        , m_listeners()
        {}


        /** \brief Get the service's name */
        virtual const char* name() const { return m_name; }

        /** \brief Get the service's UUID */
        virtual const IBleUuid& uuid() const { return m_uuid; }

        /** \brief Set the stack handle */
        virtual void setHandle(const uint16_t handle) { m_handle = handle; }

        /** \brief Get the stack handle */
        virtual uint16_t handle() const { return m_handle; }

        /** \brief Get the associated service */
        virtual IBleService& service() const { return (*m_service); }

        /** \brief Set the associated service */
        virtual void setService(IBleService& service) { m_service = &service; }

        /** \brief Get the maximal value length in bytes */
        virtual uint16_t valueLength() const { return m_value_length; }

        /** \brief Indicate if the value length can change */
        virtual bool isFixedLength() const { return m_is_fixed_length; }

        /** \brief Properties */
        virtual uint8_t properties() const { return m_properties; }

        /** \brief Get the characteristics' descriptors */
        virtual const nano_stl::IArray<IBleCharacteristicDescriptor*>& descriptors() const { return descriptorsVect(); }

        /** \brief Register a listener to the value changed event */
        virtual bool registerListener(IBleCharacteristicListener& listener) { return m_listeners.pushBack(&listener); }

        /** \brief Update the characteristics' value */
        virtual void updateValue(const bool from_stack, const void* new_value, const uint16_t new_value_size)
        {
            for (nano_stl::nano_stl_size_t i = 0; i < m_listeners.getCount(); i++)
            {
                m_listeners[i]->onValueChanged((*this), from_stack, new_value, new_value_size);
            }
        }


        /** \brief Add a descriptor */
        bool addDescriptor(IBleCharacteristicDescriptor& descriptor)
        {
            return descriptorsVect().pushBack(&descriptor);
        }

        


    protected:

        /** \brief Get the characteristic's descriptors vector */
        virtual nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() = 0;

        /** \brief Get the characteristic's descriptors vector */
        virtual const nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() const = 0;


    private:

        /** \brief Characteristic's name */
        const char* const m_name;

        /** \brief Characteristic's UUID */
        const IBleUuid& m_uuid;

        /** \brief Characteristic's stack handle */
        uint16_t m_handle;

        /** \brief Associated service */
        IBleService* m_service;

        /** \brief Characteristic's maximal value length in bytes */
        const uint16_t m_value_length;

        /** \brief Indicate if the value length can change */
        const bool m_is_fixed_length;

        /** \brief Characteristic's properties */
        const uint8_t m_properties;

        /** \brief Characteristic's listeners vector */
        nano_stl::StaticVector<IBleCharacteristicListener*, 2u> m_listeners;
};



/** \brief Bluetooth Low Energy characteristic (with descriptors) */
template <typename T, uint8_t DESCRIPTORS_COUNT>
class BleCharacteristic : public BleCharacteristicBase
{
    public:

        /** \brief Constructor */
        BleCharacteristic(const char* const name, const IBleUuid& uuid, const bool is_fixed_length, const uint8_t properties)
        : BleCharacteristicBase(name, uuid, sizeof(T), is_fixed_length, properties)
        , m_descriptors()
        {}

        /** \brief Update the characteristics' value */
        virtual void update(const T& new_value)
        {
            BleCharacteristicBase::updateValue(false, &new_value, sizeof(T));
        }


    protected:

        /** \brief Get the characteristic's descriptors vector */
        virtual nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() { return m_descriptors; }

        /** \brief Get the characteristic's descriptors vector */
        virtual const nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() const { return m_descriptors; }


    private:

        /** \brief Descriptors */
        nano_stl::StaticVector<IBleCharacteristicDescriptor*, DESCRIPTORS_COUNT> m_descriptors;
};


/** \brief Bluetooth Low Energy characteristic (with descriptors) */
template <typename T>
class BleCharacteristic<T, 0u> : public BleCharacteristicBase

{
    public:

        /** \brief Constructor */
        BleCharacteristic(const char* const name, const IBleUuid& uuid, const bool is_fixed_length, const uint8_t properties)
        : BleCharacteristicBase(name, uuid, sizeof(T), is_fixed_length, properties)
        , m_descriptors()
        {}

        /** \brief Update the characteristics' value */
        virtual void update(const T& new_value)
        {
            BleCharacteristicBase::updateValue(false, &new_value, sizeof(T));
        }


    protected:

        /** \brief Get the characteristic's descriptors vector */
        virtual nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() { return m_descriptors; }

        /** \brief Get the characteristic's descriptors vector */
        virtual const nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() const { return m_descriptors; }


    private:

        /** \brief Descriptors */
        nano_stl::ZeroSizeVector<IBleCharacteristicDescriptor*> m_descriptors;

};


/** \brief Bluetooth Low Energy characteristic (with descriptors) */
template <uint8_t DESCRIPTORS_COUNT>
class BleCharacteristic<char*, DESCRIPTORS_COUNT> : public BleCharacteristicBase
{
    public:

        /** \brief Constructor */
        BleCharacteristic(const char* const name, const IBleUuid& uuid, const uint8_t max_length, const uint8_t properties)
        : BleCharacteristicBase(name, uuid, max_length, false, properties)
        , m_descriptors()
        {}

        /** \brief Update the characteristics' value */
        virtual void update(const char new_value[])
        {
            BleCharacteristicBase::updateValue(false, new_value, NANO_STL_STRNLEN(new_value, this->valueLength()));
        }


    protected:

        /** \brief Get the characteristic's descriptors vector */
        virtual nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() { return m_descriptors; }

        /** \brief Get the characteristic's descriptors vector */
        virtual const nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() const { return m_descriptors; }


    private:

        /** \brief Descriptors */
        nano_stl::StaticVector<IBleCharacteristicDescriptor*, DESCRIPTORS_COUNT> m_descriptors;
};


/** \brief Bluetooth Low Energy characteristic (with descriptors) */
template <>
class BleCharacteristic<char*, 0u> : public BleCharacteristicBase

{
    public:

        /** \brief Constructor */
        BleCharacteristic(const char* const name, const IBleUuid& uuid, const uint8_t max_length, const uint8_t properties)
        : BleCharacteristicBase(name, uuid, max_length, false, properties)
        , m_descriptors()
        {}

        /** \brief Update the characteristics' value */
        virtual void update(const char new_value[])
        {
            BleCharacteristicBase::updateValue(false, new_value, NANO_STL_STRNLEN(new_value, this->valueLength()));
        }


    protected:

        /** \brief Get the characteristic's descriptors vector */
        virtual nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() { return m_descriptors; }

        /** \brief Get the characteristic's descriptors vector */
        virtual const nano_stl::IVector<IBleCharacteristicDescriptor*>& descriptorsVect() const { return m_descriptors; }


    private:

        /** \brief Descriptors */
        nano_stl::ZeroSizeVector<IBleCharacteristicDescriptor*> m_descriptors;

};


/** \brief Bluetooth Low Energy characteristic with 16 bits UUID */
template <typename T, uint8_t DESCRIPTORS_COUNT>
class BleCharacteristic16 : public BleCharacteristic<T, DESCRIPTORS_COUNT>
{
    public:

        /** \brief Constructor */
        BleCharacteristic16(const char* const name, const std::initializer_list<uint8_t>& uuid, const bool is_fixed_length, const uint8_t properties)
        : BleCharacteristic<T, DESCRIPTORS_COUNT>(name, m_uuid, is_fixed_length, properties)
        , m_uuid(uuid)
        {}

        /** \brief Constructor */
        BleCharacteristic16(const char* const name, const uint16_t uuid, const bool is_fixed_length, const uint8_t properties)
        : BleCharacteristic<T, DESCRIPTORS_COUNT>(name, m_uuid, is_fixed_length, properties)
        , m_uuid(uuid)
        {}


    private:

        /** \brief 16 bits UUID */
        BleUuid16 m_uuid;
};


/** \brief Bluetooth Low Energy characteristic with 16 bits UUID */
template <uint8_t DESCRIPTORS_COUNT>
class BleCharacteristic16<char*, DESCRIPTORS_COUNT> : public BleCharacteristic<char*, DESCRIPTORS_COUNT>
{
    public:

        /** \brief Constructor */
        BleCharacteristic16(const char* const name, const std::initializer_list<uint8_t>& uuid, const uint8_t max_length, const uint8_t properties)
        : BleCharacteristic<char*, DESCRIPTORS_COUNT>(name, m_uuid, max_length, properties)
        , m_uuid(uuid)
        {}

        /** \brief Constructor */
        BleCharacteristic16(const char* const name, const uint16_t uuid, const uint8_t max_length, const uint8_t properties)
        : BleCharacteristic<char*, DESCRIPTORS_COUNT>(name, m_uuid, max_length, properties)
        , m_uuid(uuid)
        {}


    private:

        /** \brief 16 bits UUID */
        BleUuid16 m_uuid;
};


/** \brief Bluetooth Low Energy characteristic with 128 bits UUID */
template <typename T, uint8_t DESCRIPTORS_COUNT>
class BleCharacteristic128 : public BleCharacteristic<T, DESCRIPTORS_COUNT>
{
    public:

        /** \brief Constructor */
        BleCharacteristic128(const char* const name, const std::initializer_list<uint8_t>& uuid, const bool is_fixed_length, const uint8_t properties)
        : BleCharacteristic<T, DESCRIPTORS_COUNT>(name, m_uuid, is_fixed_length, properties)
        , m_uuid(uuid)
        {}

        /** \brief Constructor */
        BleCharacteristic128(const char* const name, const uint8_t uuid[], const bool is_fixed_length, const uint8_t properties)
        : BleCharacteristic<T, DESCRIPTORS_COUNT>(name, m_uuid, is_fixed_length, properties)
        , m_uuid(uuid)
        {}


    private:

        /** \brief 128 bits UUID */
        BleUuid128 m_uuid;
};


/** \brief Bluetooth Low Energy characteristic with 128 bits UUID */
template <uint8_t DESCRIPTORS_COUNT>
class BleCharacteristic128<char*, DESCRIPTORS_COUNT> : public BleCharacteristic<char*, DESCRIPTORS_COUNT>
{
    public:

        /** \brief Constructor */
        BleCharacteristic128(const char* const name, const std::initializer_list<uint8_t>& uuid, const uint8_t max_length, const uint8_t properties)
        : BleCharacteristic<char*, DESCRIPTORS_COUNT>(name, m_uuid, max_length, properties)
        , m_uuid(uuid)
        {}

        /** \brief Constructor */
        BleCharacteristic128(const char* const name, const uint8_t uuid[], const uint8_t max_length, const uint8_t properties)
        : BleCharacteristic<char*, DESCRIPTORS_COUNT>(name, m_uuid, max_length, properties)
        , m_uuid(uuid)
        {}


    private:

        /** \brief 128 bits UUID */
        BleUuid128 m_uuid;
};

}

#endif // BLECHARACTERISTIC_H
