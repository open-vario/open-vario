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


#include "SpiTest.h"




namespace open_vario
{

/** \brief Constructor */
SpiTest::SpiTest(const char* name, ISpi& spi)
: HwTestBase(name, m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_spi(spi)
, m_menu_entries()
{
    MENU_ENTRY(0, "Read", SpiTest, read);
    MENU_ENTRY(1, "Write", SpiTest, write);
    MENU_ENTRY(2, "Transfer", SpiTest, xfer);
}

/** \brief Read test */
void SpiTest::read(const size_t entry, Console& console)
{
    console.writeLine("Read test :");
    console.write("- chip select : ");
    uint32_t chip_select = console.readUint();
    console.write("- bytes count (max = 64) : ");
    uint32_t count = console.readUint();
    if (count > 64u)
    {
        count = 64u;
    }

    console.writeLine("Reading %d bytes on CS %d...", count, chip_select);

    uint8_t read_buffer[64u] = {0};
    ISpi::XFer read_xfer;
    read_xfer.cs = chip_select;
    read_xfer.size = count;
    read_xfer.read_data = read_buffer;
    read_xfer.write_data = NULL;
    read_xfer.keep_cs_active = false;
    read_xfer.next = NULL;
    if (m_spi.xfer(read_xfer))
    {
        console.writeBytes(read_buffer, count);
    }
    else
    {
        console.writeLine("Failed!");
    }
}

/** \brief Write test */
void SpiTest::write(const size_t entry, Console& console)
{
    uint8_t write_buffer[16u] = {0};

    console.writeLine("Write test :");
    console.write("- chip select : ");
    uint32_t chip_select = console.readUint();
    console.write("- bytes (max = 16) : ");
    size_t count = 0;
    console.readBytes(write_buffer, sizeof(write_buffer), count);

    console.writeLine("Writing %d bytes on CS %d...", count, chip_select);
    
    ISpi::XFer write_xfer;
    write_xfer.cs = chip_select;
    write_xfer.size = count;
    write_xfer.read_data = NULL;
    write_xfer.write_data = write_buffer;
    write_xfer.keep_cs_active = false;
    write_xfer.next = NULL;
    if (m_spi.xfer(write_xfer))
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }
}

/** \brief Transfer test */
void SpiTest::xfer(const size_t entry, Console& console)
{
    uint8_t read_buffer[16u] = {0};
    uint8_t write_buffer[16u] = {0};

    console.writeLine("Transfer test :");
    console.write("- chip select : ");
    uint32_t chip_select = console.readUint();
    console.write("- bytes (max = 16) : ");
    size_t count = 0;
    console.readBytes(write_buffer, sizeof(write_buffer), count);

    console.writeLine("Transfering %d bytes on CS %d...", count, chip_select);
    
    ISpi::XFer xfer;
    xfer.cs = chip_select;
    xfer.size = count;
    xfer.read_data = read_buffer;
    xfer.write_data = write_buffer;
    xfer.keep_cs_active = false;
    xfer.next = NULL;
    if (m_spi.xfer(xfer))
    {
        console.writeBytes(read_buffer, count);
    }
    else
    {
        console.writeLine("Failed!");
    }
}

}
