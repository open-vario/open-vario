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

#ifndef SIMUGNSS_H
#define SIMUGNSS_H

#include "SimuDevice.h"
#include "IGnss.h"


namespace open_vario
{


/** \brief Simulated GNSS device */
class SimuGnss : public SimuDevice, public IGnss
{
    public:

        /** \brief Constructor */
        SimuGnss(ISimulator& simulator, const std::string& name);

        /** \brief Get the type of the device */
        static const std::string type() { return "gnss"; }


        /** \brief Configure the GNSS */
        virtual bool configure();

        /** \brief Start the GNSS */
        virtual bool start();

        /** \brief Stop the GNSS */
        virtual bool stop();

        /** \brief Read the current navigation data */
        virtual bool readData(NavigationData& nav_data);


    private:

        /** \brief Current navigation data */
        NavigationData m_nav_data;

        /** \brief Indicate if the GNSS device is running */
        bool m_running;
};

}

#endif // SIMUGNSS_H
