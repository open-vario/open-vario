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

#ifndef OPENVARIOAPP_H
#define OPENVARIOAPP_H

#include "IOpenVarioApp.h"
#include "Os.h"
#include "OpenVarioBoard.h"
#include "ModeManager.h"
#include "HmiManager.h"

namespace open_vario
{


/** \brief Base class for all open-vario applications implementations */
class OpenVarioApp : public IOpenVarioApp
{
    public:

        /** \brief Constructor */
        OpenVarioApp();


        /** \brief Get the operating system */
        virtual IOs& getOs() { return m_os; }

        /** \brief Get the board */
        virtual IOpenVarioBoard& getBoard() { return m_board; }


        /** \brief Initialize the application */
        virtual bool init(uint8_t argc, char* argv[]);

        /** \brief Start the application */
        virtual bool start();


    protected:

        /** \brief Called during application initialization */
        virtual bool onInit(uint8_t argc, char* argv[]) = 0;

        /** \brief Called before application start */
        virtual bool onStart() = 0;


    private:

        /** \brief Operating system */
        Os m_os;

        /** \brief Board */
        OpenVarioBoard m_board;

        /** \brief Mode manager */
        ModeManager m_mode_manager;

        /** \brief HMI manager */
        HmiManager m_hmi_manager;
};

}

#endif // OPENVARIOAPP_H
