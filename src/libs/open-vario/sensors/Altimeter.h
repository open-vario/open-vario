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

#ifndef ALTIMETER_H
#define ALTIMETER_H

#include "IAltimeter.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "SensorFilter.h"
#include "IBarometricAltimeter.h"
#include "EventHelper.h"

namespace open_vario
{


/** \brief Altimeter */
class Altimeter : public IAltimeter
{
    public:

        /** \brief Constructor */
        Altimeter(ConfigManager& config_manager, IBarometricAltimeter& barometric_altimeter);


        /** \brief Initialize the altimeter */
        bool init();

        /** \brief Start the altimeter */
        bool start();

        /** \brief Compute altitude and get current sensor raw values */
        bool compute(int32_t& raw_altitude, uint32_t& raw_pressure, int16_t& raw_temperature); 

        /** \brief Set a reference altitude (1 = 0.1m) */
        virtual bool setReferenceAltitude(const int32_t ref_altitude);

        /** \brief Set the altitude 1 (1 = 0.1m) */
        virtual bool setAlti1(const int32_t alti_1);

        /** \brief Set the altitude 2 (1 = 0.1m) */
        virtual bool setAlti2(const int32_t alti_2);

        /** \brief Set the altitude 3 (1 = 0.1m) */
        virtual bool setAlti3(const int32_t alti_3);

        /** \brief Set the altitude 4 (1 = 0.1m) */
        virtual bool setAlti4(const int32_t alti_4);

        /** \brief Get the current altimeter values */
        virtual bool getValues(AltimeterValues& values);

        /** \brief Event triggered on new altimeter values */
        virtual nano_stl::IEvent<const AltimeterValues&>& altimeterValuesEvent() { return m_altimeter_values_event; }


    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Barometric altimeter */
        IBarometricAltimeter& m_barometric_altimeter;


        /** \brief Mutex to protect acces to altimeter sensor */
        Mutex m_alti_mutex;

        
        /** \brief Altimeter output values */
        AltimeterValues m_alti_values;

        /** \brief Filter max depth */
        #define ALTI_FILTER_MAX_DEPTH 10u

        /** \brief Filter to compute altimeter value */
        SensorFilter<int32_t, ALTI_FILTER_MAX_DEPTH> m_alti_filter;


        /** \brief Configuration values */
        ConfigValueGroup<8u> m_config_values;
        /** \brief Configuration value : reference altitude */
        ConfigValue<int32_t> m_config_ref_alti;
        /** \brief Configuration value : reference temperature */
        ConfigValue<int16_t> m_config_ref_temp;
        /** \brief Configuration value : reference pressure */
        ConfigValue<uint32_t> m_config_ref_pres;
        /** \brief Configuration value : raw value filter depth */
        MinMaxConfigValue<uint8_t> m_config_filter_depth;
        /** \brief Configuration value : offset for alti 1 */
        ConfigValue<int32_t> m_config_offset_alti1;
        /** \brief Configuration value : offset for alti 2 */
        ConfigValue<int32_t> m_config_offset_alti2;
        /** \brief Configuration value : offset for alti 3 */
        ConfigValue<int32_t> m_config_offset_alti3;
        /** \brief Configuration value : offset for alti 4 */
        ConfigValue<int32_t> m_config_offset_alti4;


        /** \brief Indicate if the altimeter is started */
        bool m_started;


        /** \brief Offset for altitude 1 */
        int32_t m_offset_alti_1;

        /** \brief Offset for altitude 2 */
        int32_t m_offset_alti_2;

        /** \brief Offset for altitude 3 */
        int32_t m_offset_alti_3;

        /** \brief Offset for altitude 4 */
        int32_t m_offset_alti_4;


        /** \brief Event triggered on new altimeter values */
        nano_stl::EventHelper<3u, const AltimeterValues&> m_altimeter_values_event;


};

}

#endif // ALTIMETER_H
