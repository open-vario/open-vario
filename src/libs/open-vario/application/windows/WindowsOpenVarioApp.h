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

#ifndef WINDOWSOPENVARIOAPP_H
#define WINDOWSOPENVARIOAPP_H

#include "OpenVarioApp.h"
#include "ILogger.h"

namespace open_vario
{


/** \brief Open-vario application */
class WindowsOpenVarioApp : public OpenVarioApp, public ILogListener
{
    public:

        /** \brief Constructor */
        WindowsOpenVarioApp();


        /** \brief Called when a new log is available */
        virtual void onNewLog(const Log& log);


        /** \brief Singleton to retrieve the unique instance of the application */
        static IOpenVarioApp& getInstance() { return m_singleton; }


    protected:

        /** \brief Called during application initialization */
        virtual bool onInit(uint8_t argc, char* argv[]);

        /** \brief Called before application start */
        virtual bool onStart();


    private:


        /** \brief Singleton */
        static WindowsOpenVarioApp m_singleton;
};

}

#endif // WINDOWSOPENVARIOAPP_H
