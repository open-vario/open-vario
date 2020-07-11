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

#ifndef USBCDCTEST_H
#define USBCDCTEST_H

#include "HwTestBase.h"
#include "IUsbDeviceCdc.h"
#include "TaskHelper.h"

namespace open_vario
{

/** \brief Hardware test for the USB CDC link */
class UsbCdcTest : public HwTestBase, public IUsbDeviceCdcListener
{
	public:

		/** \brief Constructor */
		UsbCdcTest(IUsbDeviceCdc& usb_cdc);

		/** \brief Called when the USB cable has been plugged */
        virtual void onUsbPlugged() override;

        /** \brief Called when the USB cable has been unplugged */
        virtual void onUsbUnplugged() override;

	private:

		/** \brief USB CDC link to test */
		IUsbDeviceCdc& m_usb_cdc;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[2];

		/** \brief Console for outputs */
		Console* m_console;

		/** \brief Task for bridge test */
		TaskHelper<1024u> m_bridge_task;



		/** \brief Probe test */
		void probe(const size_t entry, Console& console);

		/** \brief Bridge test */
		void bridge(const size_t entry, Console& console);

		/** \brief Task for bridge test */
		void bridgeTask(void* param);		
};

}

#endif // USBCDCTEST_H
