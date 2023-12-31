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

#ifndef HMIMANAGER_H
#define HMIMANAGER_H

#include "LedController.h"

namespace open_vario
{


/** \brief Human Machine Interface manager */
class HmiManager
{
    public:

        /** \brief Constructor */
        HmiManager(ILed& activity_led);


        /** \brief Start the HMI manager */
        bool start();


        /** \brief Get the LED controller for the activity LED */
        LedController& getActivityLed() { return m_activity_led_controller; }

        

    private:

        /** \brief Activity LED controller */
        LedController m_activity_led_controller;
};

}

#endif // HMIMANAGER_H
