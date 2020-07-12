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


#include "RtcTest.h"
#include "IOs.h"



namespace open_vario
{

/** \brief Constructor */
RtcTest::RtcTest(IRtc& rtc)
: HwTestBase("RTC", m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_rtc(rtc)
, m_menu_entries()
{
    MENU_ENTRY(0, "Probe", RtcTest, probe);
    MENU_ENTRY(1, "Read RTC time", RtcTest, read);
    MENU_ENTRY(2, "Set RTC time", RtcTest, set);
}

/** \brief Probe test */
void RtcTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing RTC...");
    if (m_rtc.configure())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

/** \brief Read test */
void RtcTest::read(const size_t entry, Console& console)
{
    console.writeLine("Reading time...");

    IRtc::DateTime date_time = {0};
    if (m_rtc.getDateTime(date_time))
    {
        console.writeLine("Current date and time : %d/%d/%d - %d:%d:%d.%03d", date_time.year, date_time.month, date_time.day,
                                                                              date_time.hour, date_time.minute, date_time.second,
                                                                              date_time.millis);
    }
    else
    {
        console.writeLine("Failed!");
    }
    
}

/** \brief Set test */
void RtcTest::set(const size_t entry, Console& console)
{
    IRtc::DateTime date_time = {0};

    console.write("Year : ");
    date_time.year = console.readUint();
    console.write("Month : ");
    date_time.month = console.readUint();
    console.write("Day : ");
    date_time.day = console.readUint();
    console.write("Hour : ");
    date_time.hour = console.readUint();
    console.write("Minute : ");
    date_time.minute = console.readUint();
    console.write("Second : ");
    date_time.second = console.readUint();

    console.writeLine("Setting date and time...");
    if (m_rtc.setDateTime(date_time))
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }
}

}
