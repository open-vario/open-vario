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


#include "AltimeterTest.h"
#include "IOs.h"



namespace open_vario
{

/** \brief Constructor */
AltimeterTest::AltimeterTest(IAltimeterSensor& altimeter)
: HwTestBase("Altimeter", m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_altimeter(altimeter)
, m_menu_entries()
{
    MENU_ENTRY(0, "Probe", AltimeterTest, probe);
    MENU_ENTRY(1, "Read altimeter value", AltimeterTest, read);
    MENU_ENTRY(2, "Set altimeter value", AltimeterTest, set);
    MENU_ENTRY(3, "Live read", AltimeterTest, live);
}

/** \brief Probe test */
void AltimeterTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing altimeter...");
    if (m_altimeter.configure())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

/** \brief Read test */
void AltimeterTest::read(const size_t entry, Console& console)
{
    int32_t altitude = 0;
    console.writeLine("Reading altitude...");
    displayAltitude(console, "=> Altitude : %d.%dm");
}

/** \brief Set test */
void AltimeterTest::set(const size_t entry, Console& console)
{
    console.write("New altitude in meters : ");
    int32_t altitude = console.readInt();

    console.writeLine("Setting altitude...");
    if (m_altimeter.setReferenceAltitude(altitude * 10))
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }
}

/** \brief Live read test */
void AltimeterTest::live(const size_t entry, Console& console)
{
    console.write("Read period in milliseconds : ");
    uint32_t period = console.readUint();

    char c = 0;
    console.peekChar(c);
    console.writeLine("Starting live read (press any key to stop)...");
    while (console.peekChar(c))
    {
        displayAltitude(console, "%d.%dm");
        IOs::getInstance().waitMs(period);
    }
    console.writeLine("");
}

/** \brief Display current altitude */
void AltimeterTest::displayAltitude(Console& console, const char* format)
{
    int32_t altitude = 0;
    if (m_altimeter.readAltitude(altitude))
    {
        int32_t int_altitude = altitude / 10;
        int32_t frac_altitude = altitude - 10 * int_altitude;
        console.writeLine(format, int_altitude, frac_altitude);
    }
    else
    {
        console.writeLine("Failed!");
    }
}

}
