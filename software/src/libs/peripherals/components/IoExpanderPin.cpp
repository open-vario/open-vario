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

#include "IoExpanderPin.h"

namespace open_vario
{


/** \brief Constructor */
IoExpanderPin::IoExpanderPin(IIOExpander& io_expander, const uint8_t pin, const bool input)
: m_io_expander(io_expander)
, m_pin(pin)
, m_input(input)
{}


/** \brief Configure the pin */
bool IoExpanderPin::configure()
{
    return m_io_expander.configurePin(m_pin, m_input);
}

/** \brief Indicate if the pin level is low */
bool IoExpanderPin::isLow()
{
    return !isHigh();
}

/** \brief Indicate if the pin level is high */
bool IoExpanderPin::isHigh()
{
    return getLevel();
}

/** \brief Get the pin level */
IIoPin::Level IoExpanderPin::getLevel()
{
    return m_io_expander.getLevel(m_pin);
}

/** \brief Set the pin to low level */
void IoExpanderPin::setLow()
{
    setLevel(IIoPin::LOW);
}

/** \brief Set the pin to high level */
void IoExpanderPin::setHigh()
{
    setLevel(IIoPin::HIGH);
}

/** \brief Set the pin to a specified level */
void IoExpanderPin::setLevel(const IIoPin::Level level)
{
    m_io_expander.setLevel(m_pin, level);
}


}
