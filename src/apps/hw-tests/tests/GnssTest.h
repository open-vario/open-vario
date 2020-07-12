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

#ifndef GNSSTEST_H
#define GNSSTEST_H

#include "HwTestBase.h"
#include "IGnss.h"

namespace open_vario
{

/** \brief Hardware test for the GNSS */
class GnssTest : public HwTestBase
{
	public:

		/** \brief Constructor */
		GnssTest(IGnss& gnss);


	private:

		/** \brief GNSS to test */
		IGnss& m_gnss;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[2];


		/** \brief Probe test */
		void probe(const size_t entry, Console& console);

		/** \brief Live read test */
		void live(const size_t entry, Console& console);

};

}

#endif // GNSSTEST_H
