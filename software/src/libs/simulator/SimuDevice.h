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

#ifndef SIMUDEVICE_H
#define SIMUDEVICE_H

#include <string>

namespace open_vario
{


/** \brief Base class for all simulated devices */
class SimuDevice
{
    public:

        /** \brief Constructor */
        SimuDevice(const std::string& name);


        /** \brief Get the name of the device */
        const std::string& getName() const { return m_name; };

    private:

        /** \brief Name */
        const std::string m_name;

};

}

#endif // SIMUDEVICE_H
