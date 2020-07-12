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

#ifndef SPITEST_H
#define SPITEST_H

#include "HwTestBase.h"
#include "ISpi.h"

namespace open_vario
{

/** \brief Hardware test for SPI bus */
class SpiTest : public HwTestBase
{
	public:

		/** \brief Constructor */
		SpiTest(const char* name, ISpi& spi);


	private:

		/** \brief SPI bus to test */
		ISpi& m_spi;

		/** \brief Menu entries */
		Menu::Entry m_menu_entries[3];


		/** \brief Read test */
		void read(const size_t entry, Console& console);

		/** \brief Write test */
		void write(const size_t entry, Console& console);

		/** \brief Transfer test */
		void xfer(const size_t entry, Console& console);
};

}

#endif // SPITEST_H
