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

#ifndef BLETEST_H
#define BLETEST_H

#include "TaskHelper.h"
#include "BlueNrgMs.h"
#include "BlueNrgMsStack.h"

namespace open_vario
{


/** \brief BLE test task */
class BleTest : public IBlePeripheralStackListener
{
    public:

        /** \brief Constructor */
        BleTest(BlueNrgMs& blue_nrg);

        /** \brief Start the task */
        bool start();


        /** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param);

        /** \brief Called when a client device is connected to the stack */
        virtual void bleClientConnected();

        /** \brief Called when a client device is connected to the stack */
        virtual void bleClientDisconnected();


    private:

        /** \brief ST BlueNrg MS chip */
        BlueNrgMs& m_blue_nrg;

        /** \brief ST BlueNrg MS stack */
        BlueNrgMsStack m_blue_nrg_stack;

        /** \brief Task */
        TaskHelper<2048u> m_task;

};

}

#endif // BLETEST_H
