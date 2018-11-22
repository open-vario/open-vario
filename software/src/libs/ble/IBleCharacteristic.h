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

#ifndef IBLECHARACTERISTIC_H
#define IBLECHARACTERISTIC_H

#include "IBleAttribute.h"


namespace nano_stl
{

template <typename ItemType>
class IArray;

};


namespace open_vario
{

class IBleService;
class IBleCharacteristic;
class IBleCharacteristicDescriptor;


/** \brief Interface for all Bluetooth Low Energy characteristic listeners implementations */
class IBleCharacteristicListener
{
    public:

        /** \brief Called when the characteristic's value has changed */
        virtual void onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size) = 0;
};


/** \brief Interface for all Bluetooth Low Energy characteristics implementations */
class IBleCharacteristic : public IBleAttribute
{
    public:


        /** \brief Properties */
        enum Properties
        {
            /** \brief Broadcast */
            PROP_BROADCAST = 0x01u,
            /** \brief Read */
            PROP_READ = 0x02u,
            /** \brief Write without response */
            PROP_WRITE_WO_RESP = 0x04u,
            /** \brief Write */
            PROP_WRITE = 0x08u,
            /** \brief Notify */
            PROP_NOTIFY = 0x10u,
            /** \brief Indicate */
            PROP_INDICATE = 0x20u,
            /** \brief Signed write */
            PROP_SIGNED_WRITE = 0x40u,
            /** \brief Ext */
            PROP_EXT = 0x80u,
        };


        /** \brief Get the associated service */
        virtual IBleService& service() const = 0;

        /** \brief Set the associated service */
        virtual void setService(IBleService& service) = 0;

        /** \brief Get the maximal value length in bytes */
        virtual uint16_t valueLength() const = 0;

        /** \brief Indicate if the value length can change */
        virtual bool isFixedLength() const = 0;

        /** \brief Properties */
        virtual uint8_t properties() const = 0;

        /** \brief Get the characteristics' descriptors */
        virtual const nano_stl::IArray<IBleCharacteristicDescriptor*>& descriptors() const = 0;

        /** \brief Register a listener to the value changed event */
        virtual bool registerListener(IBleCharacteristicListener& listener) = 0;

        /** \brief Update the characteristics' value */
        virtual void updateValue(const bool from_stack, const void* new_value, const uint16_t new_value_size) = 0;

};

}

#endif // IBLECHARACTERISTIC_H
