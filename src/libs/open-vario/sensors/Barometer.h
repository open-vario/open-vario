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

#ifndef BAROMETER_H
#define BAROMETER_H

#include "IBarometer.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "MeanFilter.h"

namespace open_vario
{


/** \brief Barometer */
class Barometer : public IBarometer
{
    public:

        /** \brief Constructor */
        Barometer(ConfigManager& config_manager);


        /** \brief Initialize the barometer */
        bool init();

        /** \brief Start the barometer */
        bool start();

        /** \brief Compute pressure value from raw pressure value */
        bool compute(const uint32_t raw_pressure); 

        /** \brief Register a listener for the barometer values */
        virtual bool registerListener(IBarometerListener& listener);


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;


        /** \brief Filter to compute barometer value */
        MeanFilter<uint32_t, 10u> m_baro_filter;

        /** \brief Barometer values */
        BarometerValues m_baro_values;


        /** \brief Configuration values */
        ConfigValueGroup<1u> m_config_values;
        /** \brief Configuration value : raw value filter depth */
        MinMaxConfigValue<uint8_t> m_config_filter_depth;


        /** \brief Indicate if the barometer is started */
        bool m_started;


        /** \brief Listeners */
        nano_stl::StaticVector<IBarometerListener*, 3u> m_listeners;
};

}

#endif // BAROMETER_H
