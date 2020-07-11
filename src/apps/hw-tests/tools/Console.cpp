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
#include "nano-stl-libc.h"


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

/** \brief Write an array of bytes to the console */
void Console::writeFormattedBytes(const uint8_t* bytes, const size_t size)
{
	m_write_mutex.lock();

	size_t count = 0;
	while (count != size)
	{
		int val = *bytes;
		write("0x%02x ", val);
		count++;
		bytes++;
		if ((count % 16) == 0)
		{
			write("\r\n");
		}
	}
	if ((count % 16) != 0)
	{
		write("\r\n");
	}

	m_write_mutex.unlock();
}

/** \brief Write an array of bytes to the console */
void Console::writeBytes(const uint8_t* bytes, const size_t size)
{
	m_write_mutex.lock();

	m_uart.write(bytes, size);

	m_write_mutex.unlock();
}

/** \brief Peek an available character on the console */
bool Console::peekChar(char& c)
{
	m_read_mutex.lock();

	uint8_t uc = 0;
	bool ret = m_uart.read(&uc, 1u, 0);
	if (ret)
	{
		m_uart.write(&uc, 1u);
		c = static_cast<char>(uc);
	}

	m_read_mutex.unlock();

	return ret;
}

/** \brief Read a character on the console */
char Console::readChar()
{
	m_read_mutex.lock();

	uint8_t c = 0;
	while (!m_uart.read(&c, 1u, IOs::getInstance().getInfiniteTimeoutValue()))
	{}
	m_uart.write(&c, 1u);

	m_read_mutex.unlock();

	return static_cast<char>(c);
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
			bool echo = true;
			if (c == '\r')
			{
				*line = 0;
				eol = true;
			}
			else if (c == '\n')
			{
				echo = false;
			}
			else if ((c == '\b') || (c == 127))
			{
				if (rx_size != 0)
				{
					line--;
					rx_size--;
				}
				else
				{
					echo = false;
				}				
			}
			else
			{
				*line = (char)c;
				line++;
				rx_size++;
			}
			if (echo)
			{
				m_uart.write(&c, 1u);
			}
		}
	}
	c = '\n';
	m_uart.write(&c, 1u);

	m_read_mutex.unlock();
}

/** \brief Read an unsigned integer value on the console */
uint32_t Console::readUint(const uint8_t radix)
{
	char line[20u] = {0};
	readLine(line, sizeof(line));
	return static_cast<uint32_t>(NANO_STL_LIBC_Antoi(line, radix, sizeof(line)));
}

/** \brief Read a signed integer value on the console */
int32_t Console::readInt()
{
	char line[20u] = {0};
	readLine(line, sizeof(line));
	return static_cast<int32_t>(NANO_STL_LIBC_Antoi(line, 10, sizeof(line)));
}

/** \brief Read an array of bytes on the console */
void Console::readBytes(uint8_t* bytes, const size_t size, size_t& count)
{
	char line[64u] = {0};
	readLine(line, sizeof(line));

	char* byte_start = line;
	char* pline = line;
	count = 0;
	while (((*pline) != 0) && (count != size))
	{
		if ((*pline == ' '))
		{
			*pline = 0;
			bytes[count] = static_cast<uint8_t>(NANO_STL_LIBC_Antoi(byte_start, 16, sizeof(line)));
			byte_start = pline + 1;
			count++;
		}
		pline++;		
	}
	if (count != size)
	{
		*pline = 0;
		bytes[count] = static_cast<uint8_t>(NANO_STL_LIBC_Antoi(byte_start, 16, sizeof(line)));
		count++;
	}
}

/** \brief Write a formatted string to the console */
void Console::write(const char* format, va_list ap)
{
	size_t size = VSNPRINTF(m_format_buffer, sizeof(m_format_buffer), format, ap);
	m_uart.write(reinterpret_cast<uint8_t*>(m_format_buffer), size);
}

}
