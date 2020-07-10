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

#include "HwTests.h"
#include "IOs.h"

namespace open_vario
{

/** \brief Constructor */
HwTests::HwTests()
: m_os()
, m_test_task("Test task", 10u)
, m_board()
, m_console(m_board.debugUart())
, m_menu("Hardware tests", m_menu_entries, sizeof(m_menu_entries) / sizeof(Menu::Entry))
, m_menu_entries()
, m_tests()
, m_spi1_test("SPI bus 1", m_board.spi1())
, m_spi2_test("SPI bus 2", m_board.spi2())
, m_eeprom_test("Configuration EEPROM", m_board.config_eeprom())
, m_altimeter_test(m_board.altimeter())
{
	m_tests[0] = &m_spi1_test;
	m_tests[1] = &m_spi2_test;
	m_tests[2] = &m_eeprom_test;
	m_tests[3] = &m_altimeter_test;


	for (size_t i = 0; i < HW_TESTS_COUNT; i++)
	{
		MENU_ENTRY(i, m_tests[i]->getName(), HwTests, selectTest);
	}
}

/** \brief Start hardware tests */
void HwTests::start()
{
	IOs::getInstance().init();
	
	ITask::TaskMethod task_method = TASK_METHOD(HwTests, task);
	m_test_task.start(task_method, NULL);

	IOs::getInstance().start();
}

/** \brief Test's task */
void HwTests::task(void* unused)
{
	(void)unused;
	
	// Board configuration
	if (!m_board.configure())
	{
		m_console.writeLine("Error : unable to configure board!");
		while(true)
		{
			IOs::getInstance().waitMs(IOs::getInstance().getInfiniteTimeoutValue());
		}
	}
	else
	{
		while (true)
		{
			m_menu.display(m_console);
		}
	}
}

/** \brief Generic menu handler to select a test */
void HwTests::selectTest(const size_t entry, Console& console)
{
	IHwTest* hw_test = m_tests[entry - 1u];
	hw_test->getMenu().display(console);
}

}


#ifdef OS_NANO_OS

// Disable NanoOS console

#include "nano_os_api.h"

/** \brief Initialize the hardware drivers of the user module which will send/receive the console packets */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleHwInit(void)
{
    return NOS_ERR_SUCCESS;
}

/** \brief Initialize the user module which will send/receive the console packets */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleInit(void)
{
    return NOS_ERR_SUCCESS;
}

/** \brief Wait forever for an incoming char on the console link (this function must be blocking) */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleReadChar(char* const c)
{
	(void)c;
	NANO_OS_TASK_Sleep(0xFFFFFFFFu);
    return NOS_ERR_SUCCESS;
}

/** \brief Write a string on the console link */
extern "C" nano_os_error_t NANO_OS_USER_ConsoleWriteString(const char* const string)
{
	(void)string;
    return NOS_ERR_SUCCESS;
}

#endif // OS_NANO_OS
