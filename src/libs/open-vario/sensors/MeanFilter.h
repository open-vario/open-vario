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

#ifndef MEANFILTER_H
#define MEANFILTER_H

#include <cstdint>

namespace open_vario
{


/** \brief Compute a filtered value which is the mean of a set of raw values */
template <typename ValueType, uint8_t MAX_RAW_VALUES_COUNT, typename AccumulatorType = int32_t>
class MeanFilter
{
    public:

        /** \brief Constructor */
        MeanFilter()
        : m_raw_values()
        , m_filter_depth(MAX_RAW_VALUES_COUNT)
        , m_raw_values_count(0u)
        , m_current_raw_value(0u)
        {}

        /** \brief Get the maximum filter depth */
        uint8_t getMaxFilterDepth() const { return MAX_RAW_VALUES_COUNT; }

        /** \brief Set the filter depth */
        void setFilterDepth(const uint8_t filter_depth)
        {
            if ((filter_depth > 0u) &&
                (filter_depth <= MAX_RAW_VALUES_COUNT))
            {
                // Clear circular buffer
                m_current_raw_value = 0u;
                m_raw_values_count = 0u;

                // Update filter depth
                m_filter_depth = filter_depth;
            }
        }

        /** \brief Add a new raw value and compute the filtered value */
        ValueType compute(const ValueType raw_value)
        {
            // Add the value to the circular buffer
            m_raw_values[m_current_raw_value] = raw_value;
            m_current_raw_value++;
            if (m_current_raw_value == m_filter_depth)
            {
                m_current_raw_value = 0u;
            }
            if (m_raw_values_count != m_filter_depth)
            {
                m_raw_values_count++;
            }

            // Compute the filtered value
            AccumulatorType filtered_value = 0;

            for (uint8_t i = 0; i < m_raw_values_count; i++)
            {
                filtered_value += static_cast<int32_t>(m_raw_values[i]);
            }
            filtered_value /= m_raw_values_count;

            return static_cast<ValueType>(filtered_value);
        }

    private:

        /** \brief Circular buffer for the raw values */
        ValueType m_raw_values[MAX_RAW_VALUES_COUNT];

        /** \brief Maximum number of raw values to take into account */
        uint8_t m_filter_depth;

        /** \brief Current number of raw values */
        uint8_t m_raw_values_count;

        /** \brief Current index for writing the next raw value in the circular buffer */
        uint8_t m_current_raw_value;

};


}

#endif // MEANFILTER_H
