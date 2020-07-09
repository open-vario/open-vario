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

#ifndef HWTESTBASE_H
#define HWTESTBASE_H

#include "IHwTest.h"
#include "Console.h"

namespace open_vario
{

/** \brief Base class for hardware tests */
class HwTestBase : public IHwTest
{
	public:

		/** \brief Constructor */
		HwTestBase(const char* name, const Menu::Entry menu_entries[], const size_t entries_count) 
		: m_name(name)
		, m_menu(name, menu_entries, entries_count)
		{}

		/** \brief Get test name */
		virtual const char* getName() const { return m_name; }

		/** \brief Get test menu */
		virtual Menu& getMenu() { return m_menu; }


	private:

		/** \brief Test name */
		const char* m_name;

		/** \brief Test menu */
		Menu m_menu;
};

}

#endif // HWTESTBASE_H
