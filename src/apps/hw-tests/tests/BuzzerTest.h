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

#ifndef BUZZERTEST_H
#define BUZZERTEST_H

#include "HwTestBase.h"
#include "IBuzzer.h"

namespace open_vario
{

/** \brief Hardware test for the buzzer */
class BuzzerTest : public HwTestBase
{
	public:

		/** \brief Constructor */
		BuzzerTest(IBuzzer& buzzer);


	private:

		/** \brief Buzzer to test */
		IBuzzer& m_buzzer;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[4];



		/** \brief Probe test */
		void probe(const size_t entry, Console& console);

		/** \brief On test */
		void on(const size_t entry, Console& console);

		/** \brief Off test */
		void off(const size_t entry, Console& console);

		/** \brief Song test */
		void song(const size_t entry, Console& console);
};

}

#endif // BUZZERTEST_H
