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

#ifndef SENSORFILTER_H
#define SENSORFILTER_H

#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "MeanFilter.h"

#include <limits>

namespace open_vario
{


/** \brief Sensor filter for raw sensor values */
template <typename ValueType, uint8_t FILTER_DEPTH, typename AccumulatorType = int32_t>
class SensorFilter : public IConfigValueListener
{
    public:


        /** \brief Sensor value */
        struct SensorValues
        {
            /** \brief Constructor */
            SensorValues(ValueType& _current, ValueType& _min, ValueType& _max)
            : current(_current)
            , min(_min)
            , max(_max)
            {}

            /** \brief Copy constructor */
            SensorValues(const SensorValues& copy)
            : current(copy.current)
            , min(copy.min)
            , max(copy.max)
            {}

            /** \brief Current value */
            ValueType& current;
            /** \brief Min value */
            ValueType& min;
            /** \brief Max value */
            ValueType& max;
        };


        /** \brief Constructor */
        SensorFilter(ConfigManager& config_manager, const uint16_t sensor_config_group,  const uint16_t sensor_config_value, SensorValues values)
        : m_config_manager(config_manager)
        , m_config_group(sensor_config_group)
        , m_config_value(sensor_config_value)
        , m_filter()
        , m_filter_depth()
        , m_values(values)
        {}


        /** \brief Get the sensor values */
        const SensorValues& values() const { return m_values; }


        /** \brief Initialize the sensor */
        bool init()
        {
            // Load configuration value
            bool ret = m_config_manager.getConfigValue<uint8_t>(m_config_group, m_config_value, m_filter_depth);
            if (ret)
            {
                // Register configuration value listener
                ret = m_config_manager.registerConfigValueListener(m_config_group, m_config_value, *this);
                if (ret)
                {
                    // Configure the filter
                    m_filter.setFilterDepth(m_filter_depth);

                    // Initialize min and max values
                    m_values.min = std::numeric_limits<ValueType>::max();
                    m_values.max = std::numeric_limits<ValueType>::min();
                }
            }

            return ret;
        }

        /** \brief Compute sensor value from raw sensor value */
        void compute(const ValueType raw_value)
        {
            // Check if filter depth has changed
            if (m_filter_depth != m_filter.getFilterDepth())
            {
                // Update filter depth
                m_filter.setFilterDepth(m_filter_depth);
            }

            // Compute filtered value
            m_values.current = m_filter.compute(raw_value);

            // Update min and max values
            if (m_values.current < m_values.min)
            {
                m_values.min = m_values.current;
            }
            if (m_values.current > m_values.max)
            {
                m_values.max = m_values.current;
            }
        }

        /** \brief Called when a configuration value has been modified */
        virtual void onConfigValueChange(const IConfigValue& config_value)
        {
            m_filter_depth = config_value.value<uint8_t>();
        }


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Configuration group */
        const uint16_t m_config_group;

        /** \brief Configuration value */
        const uint16_t m_config_value;


        /** \brief Filter to compute sensor value */
        MeanFilter<ValueType, FILTER_DEPTH, AccumulatorType> m_filter;

        /** \brief Filter depth */
        uint8_t m_filter_depth;

        /** \brief Sensor values */
        SensorValues m_values;
};

}

#endif // SENSORFILTER_H
