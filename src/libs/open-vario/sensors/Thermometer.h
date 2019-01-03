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

#ifndef THERMOMETER_H
#define THERMOMETER_H

#include "IThermometer.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "SensorFilter.h"

namespace open_vario
{


/** \brief Thermometer */
class Thermometer : public IThermometer
{
    public:

        /** \brief Constructor */
        Thermometer(ConfigManager& config_manager);


        /** \brief Initialize the thermometer */
        bool init();

        /** \brief Start the thermometer */
        bool start();

        /** \brief Compute temperature value from raw temperature value */
        bool compute(const int16_t raw_temperature); 

        /** \brief Register a listener for the thermometer values */
        virtual bool registerListener(IThermometerListener& listener);


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;


        /** \brief Thermometer values */
        ThermometerValues m_temp_values;

        /** \brief Filter max depth */
        #define THERMO_FILTER_MAX_DEPTH 10u

        /** \brief Filter to compute thermometer value */
        SensorFilter<int16_t, THERMO_FILTER_MAX_DEPTH> m_temp_filter;


        /** \brief Configuration values */
        ConfigValueGroup<1u> m_config_values;
        /** \brief Configuration value : raw value filter depth */
        MinMaxConfigValue<uint8_t> m_config_filter_depth;


        /** \brief Indicate if the thermometer is started */
        bool m_started;


        /** \brief Listeners */
        nano_stl::StaticVector<IThermometerListener*, 3u> m_listeners;
};

}

#endif // THERMOMETER_H
