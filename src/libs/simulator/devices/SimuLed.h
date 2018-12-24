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

#ifndef SIMULED_H
#define SIMULED_H

#include "SimuDevice.h"
#include "ILed.h"


namespace open_vario
{


/** \brief Simulated LED */
class SimuLed : public SimuDevice, public ILed
{
    public:

        /** \brief Constructor */
        SimuLed(ISimulator& simulator, const std::string& name, const State initial_state);

        /** \brief Get the type of the device */
        static const std::string type() { return "led"; }


        /** \brief Configure the LED */
        virtual bool configure();

        /** \brief Set LED on */
        virtual void setOn();

        /** \brief Set LED off */
        virtual void setOff();

        /** \brief Set the LED to a specified state */
        virtual void setState(const State state);


    private:

        /** \brief Initial state */
        const State m_initial_state;

        /** \brief State */
        State m_state;
};

}

#endif // SIMULED_H
