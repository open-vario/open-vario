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
, m_set_discoverable(true)
{
    m_blue_nrg.setListener(*this);
}

/** \brief Start the task */
bool BleTest::start()
{
    return m_task.start(*this, NULL);
}


/** \brief Method which will be called at the task's startup */
void BleTest::taskStart(void* const param)
{
    (void)param;

    BlueNrgMs::HwConfig blue_nrg_hwconfig = {0};
    blue_nrg_hwconfig.ble_public_address[0u] = 0x12u;
    blue_nrg_hwconfig.ble_public_address[1u] = 0x34u;
    blue_nrg_hwconfig.ble_public_address[2u] = 0x00u;
    blue_nrg_hwconfig.ble_public_address[3u] = 0xE1u;
    blue_nrg_hwconfig.ble_public_address[4u] = 0x80u;
    blue_nrg_hwconfig.ble_public_address[5u] = 0x03u;
    m_blue_nrg.setHwConfig(blue_nrg_hwconfig);

    m_blue_nrg.configure();


    uint8_t hw_version;
    uint8_t fw_version; 
    uint8_t fw_subversion;
    m_blue_nrg.getVersion(hw_version, fw_version, fw_subversion);


    // Task loop
    while (true)
    {
        if (m_set_discoverable)
        {
            if (m_blue_nrg.setDiscoverable("Open Vario"))
            {
                m_set_discoverable = false;
            }
        }
        IOs::getInstance().waitMs(500u);
    }
}

/** \brief Called when the module is connected to another device */
void BleTest::bleConnected()
{
    
}

/** \brief Called when the module is disconnected from another device */
void BleTest::bleDisconnected()
{
    m_set_discoverable = true;
}


}
