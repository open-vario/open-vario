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

#include "SimuGnss.h"

using namespace std;

namespace open_vario
{


/** \brief Constructor */
SimuGnss::SimuGnss(ISimulator& simulator, const std::string& name)
: SimuDevice(simulator, type(), name)
, m_nav_data()
, m_running(false)
{}


/** \brief Configure the GNSS */
bool SimuGnss::configure()
{
    return true;
}

/** \brief Start the GNSS */
bool SimuGnss::start()
{
    m_running = true;
    return true;
}

/** \brief Stop the GNSS */
bool SimuGnss::stop()
{
    m_running = false;
    return true;
}

/** \brief Read the current navigation data */
bool SimuGnss::readData(NavigationData& nav_data)
{
    nav_data = m_nav_data;
    return true;
}


}
