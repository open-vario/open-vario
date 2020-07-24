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

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "IRtc.h"
#include "IGnss.h"
#include "IEvent.h"
#include "Delegate.h"
#include "ConfigManager.h"
#include "ConfigValueGroup.h"
#include "MinMaxConfigValue.h"
#include "Timer.h"

namespace open_vario
{


/** \brief Date and time manager */
class TimeManager : public IConfigValueListener, public ITimerListener
{
    public:

        /** \brief Constructor */
        TimeManager(IRtc& rtc, ConfigManager& config_manager);

        /** \brief Initiatialize the date and time manager */
        bool init();

        /** \brief Start the date and time manager */
        bool start();

        /** \brief Set the system's date and time */
        bool setDateTime(const IRtc::DateTime& utc_date_time, const int32_t time_zone);

        /** \brief Get the system's date and time */
        bool getDateTime(IRtc::DateTime& date_time);



        /** \brief Called when a configuration value has been modified */
        virtual void onConfigValueChange(const IConfigValue& config_value) override;

        /** \brief Method which will be called when the timer elapsed */
        virtual void timerElapsed(ITimer& timer) override;

        

    private:

        /** \brief RTC peripheral */
        IRtc& m_rtc;

        /** \brief Configuration manager */
        ConfigManager& m_config_manager;

        /** \brief Configuration values */
        ConfigValueGroup<2u> m_config_values;
        /** \brief Configuration value : time zone */
        MinMaxConfigValue<int32_t> m_config_time_zone;
        /** \brief Configuration value : gnss date time synchro */
        ConfigValue<bool> m_config_gnss_synchro;

        /** \brief System's time zone (offset in seconds to add to UTC date and time) */
        int32_t m_time_zone;

        /** \brief Indicate if date and time must be automatically synchronized with GNSS */
        bool m_gnss_synchro;

        /** \brief Date and time GNSS synchro timer */
        Timer m_gnss_synchro_timer;

        /** \­brief GNSS date and time (UTC) */
        IRtc::DateTime m_gnss_date_time;

        /** \brief Event handler to receive GNSS notifications */
        nano_stl::IEvent<const IGnss::NavigationData&>::EventHandlerM m_gnss_evt_handler;


        /** \brief Called when new GNSS values have been computed */
        void onGnssValues(const IGnss::NavigationData& nav_data);

        /** \brief Get the local date and time corresponding to an UTC date and time */
        void toLocalDateTime(const IRtc::DateTime& utc_date_time, IRtc::DateTime& local_date_time);
};

}

#endif // TIMEMANAGER_H
