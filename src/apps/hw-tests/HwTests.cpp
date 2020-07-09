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
: m_test_task("Test task", 10u)
, m_board()
, m_console(m_board.debugUart())
{}

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

	}
	else
	{
		/* code */
	}
}

}


#ifdef OS_NANO_OS

// Disable NanoOS console

#include "nano_os_api.h"

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
