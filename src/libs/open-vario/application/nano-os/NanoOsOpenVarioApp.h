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

#ifndef NANOOSOPENVARIOAPP_H
#define NANOOSOPENVARIOAPP_H

#include "OpenVarioApp.h"
#include "NanoOsConsole.h"

namespace open_vario
{


/** \brief Open-vario application */
class NanoOsOpenVarioApp : public OpenVarioApp
{
    public:

        /** \brief Constructor */
        NanoOsOpenVarioApp();


        /** \brief Singleton to retrieve the unique instance of the application */
        static IOpenVarioApp& getInstance() { return m_singleton; }


    protected:

        /** \brief Called during application initialization */
        virtual bool onInit(uint8_t argc, char* argv[]);

        /** \brief Called before application start */
        virtual bool onStart();


    private:


        /** \brief Specific commands implementation for Nano-OS console */
        NanoOsConsole m_nano_os_console;


        /** \brief Singleton */
        static NanoOsOpenVarioApp m_singleton;
};

}

#endif // NANOOSOPENVARIOAPP_H
