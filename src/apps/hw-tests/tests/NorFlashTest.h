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

#ifndef NORFLASHTEST_H
#define NORFLASHTEST_H

#include "HwTestBase.h"
#include "INorFlash.h"

namespace open_vario
{

/** \brief Hardware test for NOR Flashs */
class NorFlashTest : public HwTestBase
{
	public:

		/** \brief Constructor */
		NorFlashTest(const char* name, INorFlash& flash);


	private:

		/** \brief NOR Flash to test */
		INorFlash& m_flash;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[6];



		/** \brief Infos test */
		void infos(const size_t entry, Console& console);

		/** \brief Probe test */
		void probe(const size_t entry, Console& console);

		/** \brief Read test */
		void read(const size_t entry, Console& console);

		/** \brief Write test */
		void write(const size_t entry, Console& console);

		/** \brief Sector erase test */
		void sectorErase(const size_t entry, Console& console);

		/** \brief Bulk erase test */
		void bulkErase(const size_t entry, Console& console);
};

}

#endif // NORFLASHTEST_H
