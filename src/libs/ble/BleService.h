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

#ifndef BLESERVICE_H
#define BLESERVICE_H

#include "IBleService.h"
#include "IBleCharacteristic.h"
#include "StaticVector.h"
#include "ZeroSizeVector.h"
#include "BleUuid16.h"
#include "BleUuid128.h"

namespace open_vario
{


/** \brief Base class for Bluetooth Low Energy services implementations */
class BleServiceBase : public IBleService
{
    public:

        /** \brief Constructor */
        BleServiceBase(const char* const name, const IBleUuid& uuid)
        : m_name(name)
        , m_uuid(uuid)
        , m_handle(0x0000u)
        {}


        /** \brief Get the service's name */
        virtual const char* name() const { return m_name; }

        /** \brief Get the service's UUID */
        virtual const IBleUuid& uuid() const { return m_uuid; }

        /** \brief Set the stack handle */
        virtual void setHandle(const uint16_t handle) { m_handle = handle; }

        /** \brief Get the stack handle */
        virtual uint16_t handle() const { return m_handle; }

        /** \brief Get the service's included services */
        virtual const nano_stl::IArray<IBleService*>& services() const { return servicesVect(); }

        /** \brief Get the service's characteristics */
        virtual const nano_stl::IArray<IBleCharacteristic*>& characteristics() const { return characteristicsVect(); }


        /** \brief Add an included service */
        bool addService(IBleService& service)
        {
            return servicesVect().pushBack(&service);
        }

        /** \brief Add a characteristic */
        bool addCharacteristic(IBleCharacteristic& characteristic)
        {
            return characteristicsVect().pushBack(&characteristic);
        }


    protected:


        /** \brief Get the service's included services vector */
        virtual nano_stl::IVector<IBleService*>& servicesVect() = 0;

        /** \brief Get the service's characteristics vector */
        virtual nano_stl::IVector<IBleCharacteristic*>& characteristicsVect() = 0;

        /** \brief Get the service's included services vector */
        virtual const nano_stl::IVector<IBleService*>& servicesVect() const = 0;

        /** \brief Get the service's characteristics vector */
        virtual const nano_stl::IVector<IBleCharacteristic*>& characteristicsVect() const = 0;


    private:

        /** \brief Service's name */
        const char* const m_name;

        /** \brief Service's UUID */
        const IBleUuid& m_uuid;

        /** \brief Service's stack handle */
        uint16_t m_handle;
};



/** \brief Bluetooth Low Energy service (with included services) */
template <uint8_t INCLUDED_SERVICES_COUNT, uint8_t CHARACTERISTICS_COUNT>
class BleService : public BleServiceBase
{
    public:

        /** \brief Constructor */
        BleService(const char* const name, const IBleUuid& uuid)
        : BleServiceBase(name, uuid)
        , m_services()
        , m_characteristics()
        {}


    protected:

        /** \brief Get the service's included services vector */
        virtual nano_stl::IVector<IBleService*>& servicesVect() { return m_services; }

        /** \brief Get the service's characteristics vector */
        virtual nano_stl::IVector<IBleCharacteristic*>& characteristicsVect() { return m_characteristics; }

        /** \brief Get the service's included services vector */
        virtual const nano_stl::IVector<IBleService*>& servicesVect() const { return m_services; }

        /** \brief Get the service's characteristics vector */
        virtual const nano_stl::IVector<IBleCharacteristic*>& characteristicsVect() const { return m_characteristics; }


    private:

        /** \brief Included services */
        nano_stl::StaticVector<IBleService*, INCLUDED_SERVICES_COUNT> m_services;

        /** \brief Characteristics */
        nano_stl::StaticVector<IBleCharacteristic*, CHARACTERISTICS_COUNT> m_characteristics;
};


/** \brief Bluetooth Low Energy service (without included service) */
template <uint8_t CHARACTERISTICS_COUNT>
class BleService<0u, CHARACTERISTICS_COUNT> : public BleServiceBase
{
    public:

        /** \brief Constructor */
        BleService(const char* const name, const IBleUuid& uuid)
        : BleServiceBase(name, uuid)
        , m_services()
        , m_characteristics()
        {}


    protected:

        /** \brief Get the service's included services vector */
        virtual nano_stl::IVector<IBleService*>& servicesVect() { return m_services; }

        /** \brief Get the service's characteristics vector */
        virtual nano_stl::IVector<IBleCharacteristic*>& characteristicsVect() { return m_characteristics; }

        /** \brief Get the service's included services vector */
        virtual const nano_stl::IVector<IBleService*>& servicesVect() const { return m_services; }

        /** \brief Get the service's characteristics vector */
        virtual const nano_stl::IVector<IBleCharacteristic*>& characteristicsVect() const { return m_characteristics; }


    private:

        /** \brief Included services */
        nano_stl::ZeroSizeVector<IBleService*> m_services;

        /** \brief Characteristics */
        nano_stl::StaticVector<IBleCharacteristic*, CHARACTERISTICS_COUNT> m_characteristics;
};


/** \brief Bluetooth Low Energy service with 16 bits UUID */
template <uint8_t INCLUDED_SERVICES_COUNT, uint8_t CHARACTERISTICS_COUNT>
class BleService16 : public BleService<INCLUDED_SERVICES_COUNT, CHARACTERISTICS_COUNT>
{
    public:

        /** \brief Constructor */
        BleService16(const char* const name, const std::initializer_list<uint8_t>& uuid)
        : BleService<INCLUDED_SERVICES_COUNT, CHARACTERISTICS_COUNT>(name, m_uuid)
        , m_uuid(uuid)
        {}

        /** \brief Constructor */
        BleService16(const char* const name, const uint16_t& uuid)
        : BleService<INCLUDED_SERVICES_COUNT, CHARACTERISTICS_COUNT>(name, m_uuid)
        , m_uuid(uuid)
        {}


    private:

        /** \brief 16 bits UUID */
        BleUuid16 m_uuid;
};


/** \brief Bluetooth Low Energy service with 128 bits UUID */
template <uint8_t INCLUDED_SERVICES_COUNT, uint8_t CHARACTERISTICS_COUNT>
class BleService128 : public BleService<INCLUDED_SERVICES_COUNT, CHARACTERISTICS_COUNT>
{
    public:

        /** \brief Constructor */
        BleService128(const char* const name, const std::initializer_list<uint8_t>& uuid)
        : BleService<INCLUDED_SERVICES_COUNT, CHARACTERISTICS_COUNT>(name, m_uuid)
        , m_uuid(uuid)
        {}

        /** \brief Constructor */
        BleService128(const char* const name, const uint8_t uuid[])
        : BleService<INCLUDED_SERVICES_COUNT, CHARACTERISTICS_COUNT>(name, m_uuid)
        , m_uuid(uuid)
        {}


    private:

        /** \brief 128 bits UUID */
        BleUuid128 m_uuid;
};


}

#endif // BLESERVICE_H
