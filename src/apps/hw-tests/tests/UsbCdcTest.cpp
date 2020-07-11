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


#include "UsbCdcTest.h"
#include "IOs.h"



namespace open_vario
{

/** \brief Constructor */
UsbCdcTest::UsbCdcTest(IUsbDeviceCdc& usb_cdc)
: HwTestBase("USB CDC", m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_usb_cdc(usb_cdc)
, m_menu_entries()
, m_console(NULL)
, m_bridge_task("USB CDC bridge task", 11u)
{
    MENU_ENTRY(0, "Probe", UsbCdcTest, probe);
    MENU_ENTRY(1, "Bridge", UsbCdcTest, bridge);
}

/** \brief Called when the USB cable has been plugged */
void UsbCdcTest::onUsbPlugged()
{
    m_console->writeLine("[USB] - USB cable plugged!");
}

/** \brief Called when the USB cable has been unplugged */
void UsbCdcTest::onUsbUnplugged()
{
    m_console->writeLine("[USB] - USB cable Unplugged!");
}

/** \brief Probe test */
void UsbCdcTest::probe(const size_t entry, Console& console)
{
    console.writeLine("Probing USB CDC link...");
    m_console = &console;
    m_usb_cdc.registerListener(*this);
    if (m_usb_cdc.configure() && m_usb_cdc.start())
    {
        console.writeLine("Success!");
    }
    else
    {
        console.writeLine("Failed!");
    }
}

/** \brief Bridge test */
void UsbCdcTest::bridge(const size_t entry, Console& console)
{
    console.writeLine("Starting bridge between Debug UART and USB CDC link...");
    
    ITask::TaskMethod task_method = TASK_METHOD(UsbCdcTest, bridgeTask);
    m_bridge_task.start(task_method, &console);

    while (true)
    {
        char c = console.readChar();
        if (!m_usb_cdc.write(reinterpret_cast<uint8_t*>(&c), 1u))
        {
            console.writeLine("USB CDC write failed!");
        }
    }
}

/** \brief Task for bridge test */
void UsbCdcTest::bridgeTask(void* param)
{
    uint8_t buffer[128u];
    Console& console = *reinterpret_cast<Console*>(param);

    while (true)
    {
        uint32_t read_size = 0;
        if (m_usb_cdc.read(buffer, sizeof(buffer), read_size, IOs::getInstance().getInfiniteTimeoutValue()))
        {
            console.writeBytes(buffer, read_size);
        }
        else
        {
            console.writeLine("USB CDC read failed!");
        }
        
    }
}

}
