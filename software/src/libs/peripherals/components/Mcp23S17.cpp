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

#include "Mcp23S17.h"

namespace open_vario
{


/** \brief Constructor */
Mcp23S17::Mcp23S17(ISpi& spi, const uint8_t chip_select)
: m_spi(spi)
, m_chip_select(chip_select)
, m_mutex()
, m_iodirA(0xFFu)
, m_iodirB(0xFFu)
, m_olatA(0x00u)
, m_olatB(0x00u)
{}


/** \brief Configure the I/O expander */
bool Mcp23S17::configure()
{
    // Nothing to do, keep default reset values
    return true;
}

/** \brief Configure the mode of an I/O */
bool Mcp23S17::configurePin(const uint8_t pin, const bool input)
{
    uint8_t reg;
    uint8_t pin_mask;
    uint8_t* reg_value;

    if (pin < 8u)
    {
        reg = IODIRA;
        reg_value = &m_iodirA;
        pin_mask = (1u << pin);
    }
    else
    {
        reg = IODIRB;
        reg_value = &m_iodirB;
        pin_mask = (1u << (pin - 8u));
    }
    if (input)
    {
        (*reg_value) |= pin_mask;
    }
    else
    {
        (*reg_value) &= ~pin_mask;
    }

    return writeRegister(reg, (*reg_value));
}

/** \brief Set the level of an I/O */
void Mcp23S17::setLevel(const uint8_t pin, const IIoPin::Level level)
{
    uint8_t reg;
    uint8_t pin_mask;
    uint8_t* reg_value;
    
    if (pin < 8u)
    {
        reg = OLATA;
        reg_value = &m_olatA;
        pin_mask = (1u << pin);
    }
    else
    {
        reg = OLATB;
        reg_value = &m_olatB;
        pin_mask = (1u << (pin - 8u));
    }

    m_mutex.lock();

    if (level == IIoPin::HIGH)
    {
        (*reg_value) |= pin_mask;
    }
    else
    {
        (*reg_value) &= ~pin_mask;
    }

    writeRegister(reg, (*reg_value));

    m_mutex.unlock();

    return;
}

/** \brief Get the level of an I/O */
IIoPin::Level Mcp23S17::getLevel(const uint8_t pin)
{
    uint8_t reg;
    uint8_t pin_mask;
    uint8_t reg_value;
    
    if (pin < 8u)
    {
        reg = GPIOA;
        pin_mask = (1u << pin);
    }
    else
    {
        reg = GPIOB;
        pin_mask = (1u << (pin - 8u));
    }

    readRegister(reg, reg_value);

    IIoPin::Level level;
    if ((reg_value & pin_mask) != 0u)
    {
        level = IIoPin::HIGH;
    }
    else
    {
        level = IIoPin::LOW;
    }

    return level;
}

/** \brief Write a value into a register */
bool Mcp23S17::writeRegister(const uint8_t reg, const uint8_t value)
{
    const uint8_t cmd[] = {0x40u, reg, value};

    ISpi::XFer spi_xfer;
    spi_xfer.write_data = cmd;
    spi_xfer.size = sizeof(cmd);
    spi_xfer.cs = m_chip_select;
    const bool ret = m_spi.xfer(spi_xfer);

    return ret;
}

/** \brief Read a value from a register */
bool Mcp23S17::readRegister(const uint8_t reg, uint8_t& value)
{
    const uint8_t cmd[] = {0x41u, reg, 0x00u};
    uint8_t resp[sizeof(cmd)];

    ISpi::XFer spi_xfer;
    spi_xfer.write_data = cmd;
    spi_xfer.read_data = resp;
    spi_xfer.size = sizeof(cmd);
    spi_xfer.cs = m_chip_select;
    const bool ret = m_spi.xfer(spi_xfer);
    if (ret)
    {
        value = resp[2u];
    }

    return ret;
}

}
