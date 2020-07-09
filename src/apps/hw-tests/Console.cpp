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

#include "Console.h"
#include "IOs.h"


namespace open_vario
{

/** \brief Constructor */
Console::Console(IUart& uart)
: m_uart(uart)
, m_write_mutex()
, m_read_mutex()
{}


/** \brief Write a formatted string to the console */
void Console::write(const char* format, ...)
{
	m_write_mutex.lock();

	va_list ap;
    va_start(ap, format);
	write(format, ap);
	va_end(ap);

	m_write_mutex.unlock();
}

/** \brief Write a formatted line to the console */
void Console::writeLine(const char* format, ...)
{
	m_write_mutex.lock();

	va_list ap;
    va_start(ap, format);
	write(format, ap);
	va_end(ap);
	write("\r\n");

	m_write_mutex.unlock();
}

/** \brief Read a character on the console */
char Console::read()
{
	m_read_mutex.lock();

	uint8_t c = 0;
	while (!m_uart.read(&c, 1u, IOs::getInstance().getInfiniteTimeoutValue()))
	{}

	m_read_mutex.unlock();

	return (char)c;
}

/** \brief Read a line on the console */
void Console::readLine(char* line, const size_t max_size)
{
	m_read_mutex.lock();

	uint8_t c = 0;
	size_t rx_size = 0;
	bool eol = false;

	while (!eol && (rx_size != max_size))
	{
		if (m_uart.read(&c, 1u, IOs::getInstance().getInfiniteTimeoutValue()))
		{
			*line = (char)c;
			line++;
			rx_size++;
			eol = (c == '\n');
		}
	}

	m_read_mutex.unlock();
}

/** \brief Write a formatted string to the console */
void Console::write(const char* format, va_list ap)
{
	size_t size = VSNPRINTF(m_format_buffer, sizeof(m_format_buffer), format, ap);
	m_uart.write(reinterpret_cast<uint8_t*>(m_format_buffer), size);
}

}
