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

#ifndef VARIOMETER_H
#define VARIOMETER_H

#include "IVariometer.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "SensorFilter.h"

namespace open_vario
{


/** \brief Variometer */
class Variometer : public IVariometer
{
    public:

        /** \brief Constructor */
        Variometer(ConfigManager& config_manager);


        /** \brief Initialize the variometer */
        bool init();

        /** \brief Start the variometer */
        bool start();

        /** \brief Compute vario value from raw altitude value */
        bool compute(const int32_t raw_altitude, int16_t& raw_vario); 

        /** \brief Register a listener for the variometer values */
        virtual bool registerListener(IVariometerListener& listener);


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;


        /** \brief Variometer values */
        VariometerValues m_vario_values;

        /** \brief Filter max depth */
        #define VARIO_FILTER_MAX_DEPTH 50u

        /** \brief Filter to compute variometer value */
        SensorFilter<int16_t, VARIO_FILTER_MAX_DEPTH> m_vario_filter;


        /** \brief Configuration values */
        ConfigValueGroup<1u> m_config_values;
        /** \brief Configuration value : raw value filter depth */
        MinMaxConfigValue<uint8_t> m_config_filter_depth;


        /** \brief Indicate if the variometer is started */
        bool m_started;


        /** \brief Previous raw altitude value */
        int32_t m_previous_raw_altitude;
        /** \brief Previous timestamp */
        uint32_t m_previous_timestamp;


        /** \brief Listeners */
        nano_stl::StaticVector<IVariometerListener*, 3u> m_listeners;

};

}

#endif // VARIOMETER_H
