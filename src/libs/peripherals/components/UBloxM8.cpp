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

#include "UBloxM8.h"


namespace open_vario
{


/** \brief Constructor */
UBloxM8::UBloxM8(IUart& uart, ITask& rx_task)
: NmeaGnss(uart, rx_task)
{}

/** \brief Configure the GNSS */
bool UBloxM8::configure()
{
    // TODO : configure NMEA messages and periodicity
    return true;
}

/** \brief Start the GNSS */
bool UBloxM8::start()
{
    // Start NMEA message reception
    return startRx();
}

/** \brief Stop the GNSS */
bool UBloxM8::stop()
{
    // TODO : stop the GNSS
    return true;
}


}
