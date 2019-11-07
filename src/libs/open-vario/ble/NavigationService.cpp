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

#include "NavigationService.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
NavigationService::NavigationService()
: OpenVarioBleServiceBase()

, m_navigation_service("Navigation service", "530b9c7a-3185-49f0-9bb5-8e7b88a9df09")

, m_nav_data("Navigation data", "609a0afe-59a2-4837-b4fe-46d2ddfec0dd", 23u, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)

, m_gnss_data()
, m_gnss_event_handler()
{}


/** \brief Initialize the BLE service */
bool NavigationService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_navigation_service.addCharacteristic(m_nav_data);

    return ret;
}

/** \brief Start the BLE service */
bool NavigationService::start()
{
    bool ret = true;

    // Register to GNSS events
    IGnssProvider& gnss = IOpenVarioApp::getInstance().getGnssProvider();
    m_gnss_event_handler = NANO_STL_EVENT_HANDLER_M(NavigationService, onGnssData, const IGnss::NavigationData&);
    ret = ret && gnss.gnssDataEvent().bind(m_gnss_event_handler);

    return ret;
}

/** \brief Update the BLE service characteristics values */
void NavigationService::updateCharacteristicsValues()
{
    uint8_t nav_data[23u] = {0};
    NANO_STL_MEMCPY(&nav_data[0u], &m_gnss_data.satellite_count, sizeof(uint8_t));
    NANO_STL_MEMCPY(&nav_data[1u], &m_gnss_data.latitude, sizeof(double));
    NANO_STL_MEMCPY(&nav_data[9u], &m_gnss_data.longitude, sizeof(double));
    NANO_STL_MEMCPY(&nav_data[17u], &m_gnss_data.speed, sizeof(uint32_t));
    NANO_STL_MEMCPY(&nav_data[21u], &m_gnss_data.track_angle, sizeof(uint16_t));
    m_nav_data.update(nav_data, sizeof(nav_data));
}

/** \brief Called when new GNSS data is available */
void NavigationService::onGnssData(const IGnss::NavigationData& gnss_data)
{
    m_gnss_data = gnss_data;
}

}
