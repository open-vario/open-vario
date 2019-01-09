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
#include "SensorFilter.h"
#include "EventHelper.h"

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

        /** \brief Event triggered on new barometer values */
        virtual nano_stl::IEvent<const BarometerValues&>& barometerValuesEvent() { return m_barometer_values_event; }


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;


        /** \brief Barometer values */
        BarometerValues m_baro_values;

        /** \brief Filter max depth */
        #define BARO_FILTER_MAX_DEPTH 10u

        /** \brief Filter to compute barometer values */
        SensorFilter<uint32_t, BARO_FILTER_MAX_DEPTH> m_baro_filter;


        /** \brief Configuration values */
        ConfigValueGroup<1u> m_config_values;
        /** \brief Configuration value : raw value filter depth */
        MinMaxConfigValue<uint8_t> m_config_filter_depth;


        /** \brief Indicate if the barometer is started */
        bool m_started;

        
        /** \brief Event triggered on new barometer values */
        nano_stl::EventHelper<3u, const BarometerValues&> m_barometer_values_event;
};

}

#endif // BAROMETER_H
