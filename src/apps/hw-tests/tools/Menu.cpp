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

#include "Menu.h"
#include "Console.h"


namespace open_vario
{

/** \brief Constructor */
Menu::Menu(const char* title, const Entry entries[], const size_t entries_count)
: m_title(title)
, m_entries(entries)
, m_entries_count(entries_count)
{
}

/** \brief Display the menu on the selected console */
void Menu::display(Console& console)
{
	size_t choice = 0;

	do
	{
		console.writeLine("-------------------------------------------");
		console.writeLine(m_title);
		console.writeLine("-------------------------------------------");
		for (size_t i = 0; i < m_entries_count; i++)
		{
			console.writeLine("%d - %s", (i + 1), m_entries[i].text);
		}
		console.writeLine("0 - Exit");

		do
		{
			console.write("=>");
			choice = console.readUint();
			if (choice > m_entries_count)
			{
				console.writeLine("Invalid choice!");
			}
		} 
		while (choice > m_entries_count);
		if (choice != 0)
		{
			m_entries[choice - 1u].handler.invoke((const size_t)choice, console);
		}
	} 
	while (choice != 0);
}

}
