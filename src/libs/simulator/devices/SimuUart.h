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

#ifndef SIMUUART_H
#define SIMUUART_H

#include "SimuDevice.h"
#include "IUart.h"


namespace open_vario
{


/** \brief Simulated UART */
class SimuUart : public SimuDevice, public IUart
{
    public:

        /** \brief Constructor */
        SimuUart(ISimulator& simulator, const std::string& name);

        /** \brief Get the type of the device */
        static const std::string type() { return "uart"; }


        /** \brief Configure the UART */
        virtual bool configure();

        /** \brief Write data through the UART */
        virtual bool write(const uint8_t* const data, const uint32_t size);

        /** \brief Read data through the UART */
        virtual bool read(uint8_t* const data, const uint32_t size, const uint32_t timeout);


    private:


};

}

#endif // SIMUUART_H
