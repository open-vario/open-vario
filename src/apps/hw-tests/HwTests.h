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

#include "TaskHelper.h"
#include "OpenVarioBoard.h"
#include "Console.h"

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

		/** \brief Test's task */
		TaskHelper<4096u> m_test_task;

		/** \brief Board */
		OpenVarioBoard m_board;

		/** \brief Console */
		Console m_console;

		

		/** \brief Test's task */
		void task(void* unused);
};

}

#endif // HWTESTS_H
