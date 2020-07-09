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

#ifndef ALTIMETERTEST_H
#define ALTIMETERTEST_H

#include "HwTestBase.h"
#include "IAltimeterSensor.h"

namespace open_vario
{

/** \brief Hardware test for the altimeter */
class AltimeterTest : public HwTestBase
{
	public:

		/** \brief Constructor */
		AltimeterTest(IAltimeterSensor& altimeter);


	private:

		/** \brief Altimeter to test */
		IAltimeterSensor& m_altimeter;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[4];



		/** \brief Probe test */
		void probe(const size_t entry, Console& console);

		/** \brief Read test */
		void read(const size_t entry, Console& console);

		/** \brief Set test */
		void set(const size_t entry, Console& console);

		/** \brief Live read test */
		void live(const size_t entry, Console& console);

		/** \brief Display current altitude */
		void displayAltitude(Console& console, const char* format);
};

}

#endif // ALTIMETERTEST_H
