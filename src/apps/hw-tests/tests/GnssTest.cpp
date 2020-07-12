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


#include "GnssTest.h"
#include "IOs.h"



namespace open_vario
{

/** \brief Constructor */
GnssTest::GnssTest(IGnss& gnss)
: HwTestBase("GNSS", m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_gnss(gnss)
, m_menu_entries()
{
    MENU_ENTRY(0, "Probe", GnssTest, probe);
    MENU_ENTRY(1, "Live read", GnssTest, live);
}

/** \brief Probe test */
void GnssTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing GNSS...");
    if (m_gnss.configure(1000u) && m_gnss.probe())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

/** \brief Live read test */
void GnssTest::live(const size_t entry, Console& console)
{
    console.writeLine("Starting GNSS...");
    if (m_gnss.start())
    {
        char c = 0;
        IGnss::NavigationData nav_data = {0};
        console.writeLine("Starting live read (press any key to stop)...");
        while (!console.peekChar(c))
        {
            if (m_gnss.readData(nav_data))
            {
                console.writeLine("-------------------------------------------");
                console.writeLine("Quality : %d satellites", nav_data.satellite_count);
                console.writeLine("Date : %d/%d/%d - %d:%d:%d", nav_data.date_time.year, nav_data.date_time.month, nav_data.date_time.day,
                                                                nav_data.date_time.hour, nav_data.date_time.minute, nav_data.date_time.second);
                console.writeLine("Position : %dm, lat. %d, long. %d", nav_data.altitude, static_cast<uint32_t>(nav_data.latitude * 100.), static_cast<uint32_t>(nav_data.longitude * 100.));
                console.writeLine("Movement : %dm/s, %d°", nav_data.speed, nav_data.track_angle);
            }
            else
            {
                console.writeLine("Failed!");
            }
            IOs::getInstance().waitMs(1000u);
        }
        console.writeLine("");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

}
