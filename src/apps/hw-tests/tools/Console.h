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

#ifndef CONSOLE_H
#define CONSOLE_H

#include "IUart.h"
#include "Mutex.h"

namespace open_vario
{

/** \brief Handle console inputs and outputs on UART */
class Console
{
	public:

		/** \brief Constructor */
		Console(IUart& uart);

		/** \brief Write a formatted string to the console */
		void write(const char* format, ...);

		/** \brief Write a formatted line to the console */
		void writeLine(const char* format, ...);

		/** \brief Write an array of bytes to the console */
		void writeBytes(const uint8_t* bytes, const size_t size);

		/** \brief Peek an available character on the console */
		bool peekChar(char& c);

		/** \brief Read a character on the console */
		char readChar();

		/** \brief Read a line on the console */
		void readLine(char* line, const size_t max_size);

		/** \brief Read an unsigned integer value on the console */
		uint32_t readUint(const uint8_t radix = 10);

		/** \brief Read a signed integer value on the console */
		int32_t readInt();

		/** \brief Read an array of bytes on the console */
		void readBytes(uint8_t* bytes, const size_t size, size_t& count);


	private:

		/** \brief UART */
		IUart& m_uart;

		/** \brief Mutex to protect concurrent write accesses to console */
		Mutex m_write_mutex;

		/** \brief Mutex to protect concurrent read accesses to console */
		Mutex m_read_mutex;

		/** \brief Buffer to format strings before output */
		char m_format_buffer[1024u];


		/** \brief Write a formatted string to the console */
		void write(const char* format, va_list ap);


};

}

#endif // CONSOLE_H
