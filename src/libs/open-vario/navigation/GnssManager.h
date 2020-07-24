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

#ifndef GNSSMANAGER_H
#define GNSSMANAGER_H

#include "Timer.h"
#include "IGnssProvider.h"
#include "Semaphore.h"
#include "TaskHelper.h"
#include "ConfigManager.h"
#include "MinMaxConfigValue.h"
#include "Barometer.h"
#include "Thermometer.h"
#include "Variometer.h"
#include "FaultManager.h"
#include "EventHelper.h"

namespace open_vario
{


/** \brief Gnss manager */
class GnssManager : public ITimerListener, public IGnssProvider
{
    public:

        /** \brief Constructor */
        GnssManager(ConfigManager& config_manager, FaultManager& fault_manager, IGnss& gnss);


        /** \brief Initialize the GNSS manager */
        bool init();

        /** \brief Start the GNSS manager */
        bool start();

        /** \brief Start the GNSS notifications */
        bool startNotifications();


        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer);

        /** \brief Get the current navigation data */
        virtual bool getData(IGnss::NavigationData& nav_data);

        /** \brief Event triggered on new GNSS data */
        virtual nano_stl::IEvent<const IGnss::NavigationData&>& gnssDataEvent() { return m_gnss_data_event; }

    private:

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Fault manager */
        FaultManager& m_fault_manager;

        /** \brief GNSS */
        IGnss& m_gnss;


        /** \brief Configuration values */
        ConfigValueGroup<3u> m_config_values;
        /** \brief Configuration value : GNSS enabled flag */
        ConfigValue<bool> m_config_enabled;
        /** \brief Configuration value : acquisition period */
        MinMaxConfigValue<uint16_t> m_config_acq_period;


        /** \brief Indicate if the GNSS is enabled */
        bool m_enabled;

        /** \brief Acquisition period in milliseconds */
        uint16_t m_acq_period;

        /** \brief GNSS manager task */
        TaskHelper<2048u> m_task;

        /** \brief Timer to start periodic navigation data acquisitions */
        Timer m_acq_timer;

        /** \brief Semaphore to trigger the start of sensor acquisitions */
        Semaphore m_acq_trigger_sem;


        /** \brief Event triggered on new GNSS data */
        nano_stl::EventHelper<3u, const IGnss::NavigationData&> m_gnss_data_event;



        /** \brief GNSS manager's task method */
        void task(void* const param);
};

}

#endif // GNSSMANAGER_H
