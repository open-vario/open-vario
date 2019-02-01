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

#ifndef OPENVARIOBLESERVICEBASE_H
#define OPENVARIOBLESERVICEBASE_H

#include "IOpenVarioBleService.h"

namespace open_vario
{


/** \brief Base class for all Open Vario BLE service implementations */
class OpenVarioBleServiceBase : public IOpenVarioBleService
{
    public:


        /** \brief Constructor */
        OpenVarioBleServiceBase()
        : m_listener(nullptr)
        {}


        /** \brief Register a listener */
        virtual bool registerListener(IOpenVarioBleServiceListener& listener)
        {
            bool ret = false;

            if (m_listener == nullptr)
            {
                m_listener = &listener;
                ret = true;
            }

            return ret;
        }


    protected:

        /** \brief Trigger the asynchronous update */
        void triggerAsyncUpdate()
        {
            if (m_listener != nullptr)
            {
                m_listener->triggerAsyncUpdate(*this);
            }
        }

        /** \brief Get the listener */
        IOpenVarioBleServiceListener* getListener() { return m_listener; }


    private:

        /** \brief Listener */
        IOpenVarioBleServiceListener* m_listener;
};

}

#endif // OPENVARIOBLESERVICEBASE_H
