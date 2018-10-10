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

#include "EepromTest.h"
#include "IOs.h"

namespace open_vario
{


/** \brief Constructor */
EepromTest::EepromTest(IEeprom& eeprom)
: m_eeprom(eeprom)
, m_task("EEPROM test", 5u)
{}

/** \brief Start the task */
bool EepromTest::start()
{
    return m_task.start(*this, NULL);
}


/** \brief Method which will be called at the task's startup */
void EepromTest::taskStart(void* const param)
{
    (void)param;

    // Read the whole eeprom
    static uint8_t data[512u];
    for (uint16_t i = 0; i < m_eeprom.getSize(); i += sizeof(data))
    {
        m_eeprom.read(i, data, sizeof(data));
    }

#if 0
    // Write data in the eeprom
    MEMSET(data, 0xFF, sizeof(data));
    m_eeprom.write(0, data, sizeof(data));

    for (uint16_t i = 0; i < sizeof(data); i++)
    {
        data[i] = i;
    }
    m_eeprom.write(10, data, sizeof(data));

    MEMSET(data, 0, sizeof(data));
    m_eeprom.read(0, data, sizeof(data));
#endif

    // Task loop
    while (true)
    {
        IOs::getInstance().waitMs(500u);
    }
}


}
