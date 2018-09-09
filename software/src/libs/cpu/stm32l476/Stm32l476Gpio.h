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

#ifndef STM32L476GPIO_H
#define STM32L476GPIO_H

#include "IInputPin.h"
#include "IOutputPin.h"

namespace open_vario
{



/** \brief STM32L476 GPIO */
class Stm32l476Gpio : public IInputPin, public IOutputPin
{
    public:


        /** \brief IO ports */
        enum Port
        {
            /** \brief Port A */
            PORT_A = 0u,
            /** \brief Port B */
            PORT_B = 1u,
            /** \brief Port C */
            PORT_C = 2u,
            /** \brief Port D */
            PORT_D = 3u,

            /** \brief Max port value (do not use) */
            PORT_MAX
        };

        /** \brief IO mode */
        enum Mode
        {
            /** \brief Input */
            MODE_INPUT = 0u,
            /** \brief Output */
            MODE_OUTPUT = 1u,
            /** \brief Alternate function */
            MODE_AF = 2u,
            /** \brief Analog */
            MODE_ANALOG = 3u,

            /** \brief Max mode value (do not use) */
            MODE_MAX
        };

        /** \brief IO configuration (bitfield) */
        enum Configuration
        {
            /** \brief No special configuration */
            CONFIG_NONE = 0u,
            /** \brief Open drain */
            CONFIG_OPEN_DRAIN = 1u,
            /** \brief Pull up */
            CONFIG_PULL_UP = 2u,
            /** \brief Pull down */
            CONFIG_PULL_DOWN = 4u
        };

        /** \brief IO speed */
        enum Speed
        {
            /** \brief Low */
            SPEED_LOW = 0u,
            /** \brief Medium */
            SPEED_MEDIUM = 1u,
            /** \brief High */
            SPEED_HIGH = 2u,
            /** \brief Very high */
            SPEED_VERY_HIGH = 3u,

            /** \brief Max speed value (do not use) */
            SPEED_MAX
        };

        /** \brief Maximum number of pins per port */
        static const uint8_t MAX_PIN_PER_PORT = 16u;

        /** \brief Maximum number of alternate functions per pin */
        static const uint8_t MAX_AF_PER_PIN = 16u;


        /** \brief Constructor */
        Stm32l476Gpio(const Port port, const uint8_t pin, const Mode mode, const uint8_t alternate_function, const uint8_t configuration, const Speed speed);

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


        /** \brief Port */
        const Port m_port;
        
        /** \brief Pin */
        const uint8_t m_pin;
        
        /** \brief Mode */
        const Mode m_mode;
        
        /** \brief Alternate function */
        const uint8_t m_alternate_function;

        /** \brief Configuration */
        const uint8_t m_configuration;
        
        /** \brief Speed */
        const Speed m_speed;
};

}

#endif // STM32L476GPIO_H
