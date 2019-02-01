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

#ifndef IOPENVARIOBLESERVICE_H
#define IOPENVARIOBLESERVICE_H


namespace open_vario
{

class IBleService;
class IOpenVarioBleService;


/** \brief Interface for all Open Vario BLE service listeners implementations */
class IOpenVarioBleServiceListener
{
    public:

        /** \brief Called when a service needs to asynchronously update its characteristics values */
        virtual void triggerAsyncUpdate(IOpenVarioBleService& ble_service) = 0;
};


/** \brief Interface for all Open Vario BLE service implementations */
class IOpenVarioBleService
{
    public:


        /** \brief Initialize the BLE service */
        virtual bool init() = 0;

        /** \brief Start the BLE service */
        virtual bool start() = 0;

        /** \brief Update the BLE service characteristics values */
        virtual void updateCharacteristicsValues() = 0;

        /** \brief Get the BLE service */
        virtual IBleService& getService() = 0;

        /** \brief Register a listener */
        virtual bool registerListener(IOpenVarioBleServiceListener& listener) = 0;
};

}

#endif // IOPENVARIOBLESERVICE_H
