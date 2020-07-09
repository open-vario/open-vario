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

#ifndef MENU_H
#define MENU_H


#include "Delegate.h"

/** \brief Macro to initialise a menu entry */
#define MENU_ENTRY(index, name, class, method)  {\
                                                    m_menu_entries[index].text = name; \
                                                    m_menu_entries[index].handler = Menu::EntryHandler::create<class, &class::method>(*this); \
                                                }


namespace open_vario
{

class Console;

/** \brief Display a menu on the console */
class Menu
{
	public:

		/** \brief Entry handler */
		typedef nano_stl::Delegate<void, const size_t, Console&>  EntryHandler;

		/** \brief Entry in the menu */
		struct Entry
		{
			/** \brief Text */
			const char* text;
			/** \brief Handler */
			EntryHandler handler;
		};


		/** \brief Constructor */
		Menu(const char* title, const Entry entries[], const size_t entries_count);

		/** \brief Display the menu on the selected console */
		void display(Console& console);


	private:

	
		/** \brief Title */
		const char* m_title;

		/** \brief Entries */
		const Entry* const m_entries;

		/** \brief Number of entries */
		const size_t m_entries_count;


};

}

#endif // MENU_H
