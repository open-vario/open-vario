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

#include "BleTest.h"
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
BleTest::BleTest(BlueNrgMs& blue_nrg)
: m_blue_nrg(blue_nrg)
, m_task("BLE test", 5u)
{}

/** \brief Start the task */
bool BleTest::start()
{
    return m_task.start(*this, NULL);
}


/** \brief Method which will be called at the task's startup */
void BleTest::taskStart(void* const param)
{
    (void)param;



    // Task loop
    while (true)
    {
        IOs::getInstance().waitMs(500u);
    }
}


}
