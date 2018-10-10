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

namespace open_vario
{


/** \brief BLE test task */
class BleTest : public ITaskStart, public IBlueNrgMsListener
{
    public:

        /** \brief Constructor */
        BleTest(BlueNrgMs& blue_nrg);

        /** \brief Start the task */
        bool start();


        /** \brief Method which will be called at the task's startup */
        virtual void taskStart(void* const param);

        /** \brief Called when the module is connected to another device */
        virtual void bleConnected();

        /** \brief Called when the module is disconnected from another device */
        virtual void bleDisconnected();


    private:

        /** \brief ST BlueNrg MS chip */
        BlueNrgMs& m_blue_nrg;

        /** \brief Task */
        TaskHelper<1024u> m_task;


        /** \brief Indicate if the module must be discoverable again */
        bool m_set_discoverable;


};

}

#endif // BLETEST_H
