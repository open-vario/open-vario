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

#ifndef HWTESTS_H
#define HWTESTS_H

#include "Os.h"
#include "TaskHelper.h"
#include "OpenVarioBoard.h"
#include "Console.h"
#include "Menu.h"

#include "SpiTest.h"
#include "EepromTest.h"
#include "NorFlashTest.h"
#include "AltimeterTest.h"
#include "GnssTest.h"
#include "BuzzerTest.h"
#include "RtcTest.h"

namespace open_vario
{

/** \brief Handle hardware tests */
class HwTests
{
	public:

		/** \brief Constructor */
		HwTests();

		/** \brief Start hardware tests */
		void start();

	private:

		/** \brief OS */
		Os m_os;

		/** \brief Test's task */
		TaskHelper<4096u> m_test_task;

		/** \brief Board */
		OpenVarioBoard m_board;

		/** \brief Console */
		Console m_console;

		/** \brief Main menu */
		Menu m_menu;

		/** \brief Number of hardware tests */
		static const size_t HW_TESTS_COUNT = 8;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[HW_TESTS_COUNT];

		/** \brief Hardware tests */
		IHwTest* m_tests[HW_TESTS_COUNT];
		SpiTest m_spi1_test;
		SpiTest m_spi2_test;
		EepromTest m_eeprom_test;
		NorFlashTest m_norflash_test;
		AltimeterTest m_altimeter_test;
		GnssTest m_gnss_test;
		BuzzerTest m_buzzer_test;
		RtcTest m_rtc_test;


		

		/** \brief Test's task */
		void task(void* unused);

		/** \brief Generic menu handler to select a test */
		void selectTest(const size_t entry, Console& console);
};

}

#endif // HWTESTS_H
