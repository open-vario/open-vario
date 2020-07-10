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


#include "NorFlashTest.h"




namespace open_vario
{

/** \brief Constructor */
NorFlashTest::NorFlashTest(const char* name, INorFlash& flash)
: HwTestBase(name, m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_flash(flash)
, m_menu_entries()
{
    MENU_ENTRY(0, "Infos", NorFlashTest, infos);
    MENU_ENTRY(1, "Probe", NorFlashTest, probe);
    MENU_ENTRY(2, "Read bytes", NorFlashTest, read);
    MENU_ENTRY(3, "Write bytes", NorFlashTest, write);
    MENU_ENTRY(4, "Sector erase", NorFlashTest, sectorErase);
    MENU_ENTRY(5, "Bulk erase", NorFlashTest, bulkErase);
}

/** \brief Infos test */
void NorFlashTest::infos(const size_t entry, Console& console)
{
    console.writeLine("NOR Flash size in bytes : %u", m_flash.getSize());
}

/** \brief Probe test */
void NorFlashTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing NOR Flash...");
    if (m_flash.configure())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }    
}

/** \brief Read test */
void NorFlashTest::read(const size_t entry, Console& console)
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
        success = m_flash.read(start_address, buffer, size);
        if (success)
        {
            console.writeBytes(buffer, size);
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
void NorFlashTest::write(const size_t entry, Console& console)
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
        success = m_flash.write(start_address, buffer, size);
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

/** \brief Sector erase test */
void NorFlashTest::sectorErase(const size_t entry, Console& console)
{
    console.writeLine("Sector erase :");
    console.write("- sector address : ");
    uint32_t start_address = console.readUint(16u);

    console.writeLine("Executing sector erase operation at address 0x%x...", start_address);
    if (m_flash.sectorErase(start_address))
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    } 
}

/** \brief Bulk erase test */
void NorFlashTest::bulkErase(const size_t entry, Console& console)
{
    console.writeLine("Executing bulk erase operation...");
    if (m_flash.chipErase())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    } 
}

}
