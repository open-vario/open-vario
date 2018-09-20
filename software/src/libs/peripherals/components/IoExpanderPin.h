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

#ifndef IOEXPANDERPIN_H
#define IOEXPANDERPIN_H

#include "IIOExpander.h"
#include "IInputPin.h"
#include "IOutputPin.h"

namespace open_vario
{


/** \brief I/O pin driven by an I/O expander */
class IoExpanderPin : public IInputPin, public IOutputPin
{
    public:

        /** \brief Constructor */
        IoExpanderPin(IIOExpander& io_expander, const uint8_t pin, const bool input);


        /** \brief Configure the pin */
        virtual bool configure();

        /** \brief Indicate if the pin level is low */
        virtual bool isLow();

        /** \brief Indicate if the pin level is high */
        virtual bool isHigh();

        /** \brief Get the pin level */
        virtual Level getLevel();

        /** \brief Set the pin to low level */
        virtual void setLow();

        /** \brief Set the pin to high level */
        virtual void setHigh();

        /** \brief Set the pin to a specified level */
        virtual void setLevel(const Level level);


    private:

        /** \brief I/O expander */
        IIOExpander& m_io_expander;

        /** \brief Pin number */
        const uint8_t m_pin;

        /** \brief Indicate if the pin must be configured as an input */
        const bool m_input;

};

}

#endif // IOEXPANDERPIN_H
