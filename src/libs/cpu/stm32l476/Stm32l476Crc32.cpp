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

#include "Stm32l476Crc32.h"

#include "stm32l476xx.h"


namespace open_vario
{


/** \brief Constructor */
Stm32l476Crc32::Stm32l476Crc32()
{}


/** \brief Configure the CRC-32 driver */
bool Stm32l476Crc32::configure()
{
    // Enable clock
    RCC->AHB1ENR |= (1u << 12u);

    // Configure CRC-32 (Ethernet) polynomial: 0x04C11DB7
    CRC->CR = 0u;
    CRC->INIT = 0xFFFFFFFFu;
    CRC->POL = 0x04C11DB7u;

    // Reset computation
    reset();

    return true;
}

/** \brief Reset the CRC-32 computation */
void Stm32l476Crc32::reset()
{
    CRC->CR |= (1u << 0u);
    while ((CRC->CR & (1u << 0u)) != 0u)
    {}
}

/** \brief Update the CRC-32 computation with a data buffer */
uint32_t Stm32l476Crc32::update(const void* buffer, const size_t size)
{
    const uint32_t* u32_buffer = reinterpret_cast<const uint32_t*>(buffer);
    size_t u32_size = size / sizeof(uint32_t);
    size_t u8_size = size;
    while (u32_size != 0u)
    {
        CRC->DR = *u32_buffer;
        u32_buffer++;
        u32_size--;
        u8_size -= sizeof(uint32_t);
    }

    const uint8_t* u8_buffer = reinterpret_cast<const uint8_t*>(u32_buffer);
    volatile uint8_t* crc_dr = reinterpret_cast<volatile uint8_t*>(&(CRC->DR));
    while (u8_size != 0u)
    {
        *crc_dr = *u8_buffer;
        u8_buffer++;
        u8_size--;
    }

    return CRC->DR;
}

/** \brief Get the current value of the CRC-32 computation */
uint32_t Stm32l476Crc32::value() const
{
    return CRC->DR;
}

}
