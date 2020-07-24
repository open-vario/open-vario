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




namespace open_vario
{

/** \brief Constructor */
EepromTest::EepromTest(const char* name, IEeprom& eeprom)
: HwTestBase(name, m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_eeprom(eeprom)
, m_menu_entries()
{
    MENU_ENTRY(0, "Infos", EepromTest, infos);
    MENU_ENTRY(1, "Probe", EepromTest, probe);
    MENU_ENTRY(2, "Read bytes", EepromTest, read);
    MENU_ENTRY(3, "Write bytes", EepromTest, write);
    MENU_ENTRY(4, "Erase", EepromTest, erase);
}

/** \brief Infos test */
void EepromTest::infos(const size_t entry, Console& console)
{
    console.writeLine("EEPROM size in bytes : %u", m_eeprom.getSize());
}

/** \brief Probe test */
void EepromTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing EEPROM...");
    if (m_eeprom.configure())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

/** \brief Read test */
void EepromTest::read(const size_t entry, Console& console)
{
    console.writeLine("Read test :");
    console.write("- start address : ");
    uint32_t start_address = console.readUint(16u);
    console.write("- bytes count : ");
    uint32_t count = console.readUint(10u);

    console.writeLine("Reading %d bytes starting at address 0x%x...", count, start_address);
    bool success = true;
    uint8_t buffer[64u];
    uint32_t left = count;
    while (success && (left != 0))
    {
        uint32_t size = left;
        if (size > sizeof(buffer))
        {
            size = sizeof(buffer);
        }
        success = m_eeprom.read(start_address, buffer, size);
        if (success)
        {
            console.writeFormattedBytes(buffer, size);
            left -= size;
            start_address += size;
        }
    }
    if (!success)
    {
        console.writeLine("Failed!");
    }
}

/** \brief Write test */
void EepromTest::write(const size_t entry, Console& console)
{
    console.writeLine("Write test :");
    console.write("- start address : ");
    uint32_t start_address = console.readUint(16u);
    console.write("- bytes count : ");
    uint32_t count = console.readUint(10u);
    console.write("- start value : ");
    uint8_t start_value = console.readUint(16u);

    console.writeLine("Writing %d bytes starting at address 0x%x with value 0x%02x...", count, start_address, (int)(start_value));
    bool success = true;
    uint8_t buffer[64u];
    uint32_t left = count;
    while (success && (left != 0))
    {
        uint32_t size = left;
        if (size > sizeof(buffer))
        {
            size = sizeof(buffer);
        }
        for (uint32_t i = 0; i < size; i++)
        {
            buffer[i] = start_value;
            start_value++;
        }
        success = m_eeprom.write(start_address, buffer, size);
        if (success)
        {
            left -= size;
            start_address += size;
        }
    }
    if (success)
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }  
}

/** \brief Erase test */
void EepromTest::erase(const size_t entry, Console& console)
{
    console.writeLine("Erase test :");
    console.write("- start address : ");
    uint32_t start_address = console.readUint(16u);
    console.write("- bytes count : ");
    uint32_t count = console.readUint(10u);

    console.writeLine("Erasing %d bytes starting at address 0x%x...", count, start_address);
    bool success = true;
    uint8_t buffer[64u];
    uint32_t left = count;
    NANO_STL_MEMSET(buffer, 0xFF, sizeof(buffer));
    while (success && (left != 0))
    {
        uint32_t size = left;
        if (size > sizeof(buffer))
        {
            size = sizeof(buffer);
        }
        success = m_eeprom.write(start_address, buffer, size);
        if (success)
        {
            left -= size;
            start_address += size;
        }
    }
    if (success)
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }  
}

}
