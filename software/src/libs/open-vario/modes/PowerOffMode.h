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

#ifndef POWEROFFMODE_H
#define POWEROFFMODE_H

#include "IMode.h"
#include "ModeManager.h"
#include "HmiManager.h"

namespace open_vario
{

class HmiManager;

/** \brief Operating mode : Power off */
class PowerOffMode : public IMode
{
    public:

        /** \brief Constructor */
        PowerOffMode(ModeManager& mode_manager, HmiManager& hmi_manager);


        ////// Implementation of IMode interface //////


        /** \brief Enter into the operating mode */
        virtual void enter();

        /** \brief Leave the operating mode */
        virtual void leave();


    private:

        /** \brief Mode manager */
        ModeManager& m_mode_manager;

        /** \brief HMI manager */
        HmiManager& m_hmi_manager;
};

}

#endif // POWEROFFMODE_H
