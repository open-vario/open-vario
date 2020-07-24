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

#ifndef SENSORSMANAGER_H
#define SENSORSMANAGER_H

#include "Timer.h"
#include "Semaphore.h"
#include "TaskHelper.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "Altimeter.h"
#include "Barometer.h"
#include "Thermometer.h"
#include "Variometer.h"
#include "IGnssProvider.h"

namespace open_vario
{


/** \brief Sensors manager */
class SensorsManager : public ITimerListener
{
    public:

        /** \brief Constructor */
        SensorsManager(ConfigManager& config_manager, Altimeter& altimeter, Barometer& barometer, Thermometer& thermometer, Variometer& variometer, IGnssProvider& gnss);


        /** \brief Initialize the sensors manager */
        bool init();

        /** \brief Start the sensors manager */
        bool start();

        /** \brief Start the sensors notifications */
        bool startNotifications();


        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer);

    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Altimeter */
        Altimeter& m_altimeter;

        /** \brief Barometer */
        Barometer& m_barometer;

        /** \brief Thermometer */
        Thermometer& m_thermometer;

        /** \brief Variometer */
        Variometer& m_variometer;

        /** \brief GNSS */
        IGnssProvider& m_gnss;


        /** \brief Configuration values */
        ConfigValueGroup<2u> m_config_values;
        /** \brief Configuration value : acquisition period */
        MinMaxConfigValue<uint16_t> m_config_acq_period;
        /** \brief Configuration value : auto calibration with GNSS altitude */
        ConfigValue<bool> m_config_gnss_auto_calib;


        /** \brief Acquisition period in milliseconds */
        uint16_t m_acq_period;

        /** \brief Sensor task */
        TaskHelper<2048u> m_task;

        /** \brief Timer to start periodic sensor acquisitions */
        Timer m_acq_timer;

        /** \brief Semaphore to trigger the start of sensor acquisitions */
        Semaphore m_acq_trigger_sem;


        /** \brief Indicate if auto calibration with GNSS altitude is enabled */
        bool m_gnss_auto_calib;


        /** \brief Sensors manager's task method */
        void task(void* const param);
};

}

#endif // SENSORSMANAGER_H
