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

#ifndef UBLOXM8_H
#define UBLOXM8_H

#include "NmeaGnss.h"

namespace open_vario
{


/** \brief UBlox M8 GNSS module */
class UBloxM8 : public NmeaGnss
{
    public:

        /** \brief Constructor */
        UBloxM8(IUart& uart, ITask& rx_task);


        /** \brief Configure the GNSS */
        virtual bool configure();

        /** \brief Start the GNSS */
        virtual bool start();

        /** \brief Stop the GNSS */
        virtual bool stop();


    private:

};

}

#endif // UBLOXM8_H
