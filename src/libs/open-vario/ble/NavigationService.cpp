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

, m_speed("Speed", "609a0afe-59a2-4837-b4fe-46d2ddfec0dd", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_latitude("Latitude", "c75a49f0-cfe0-4d93-8109-3dbc588c2243", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_longitude("Longitude", "3692fbda-6a27-4422-a307-5f852658cae0", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_track_angle("Track angle", "c6502b8c-5aae-489f-bb23-04eabc389f58", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
{}


/** \brief Initialize the BLE service */
bool NavigationService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_navigation_service.addCharacteristic(m_speed);
    ret = ret && m_navigation_service.addCharacteristic(m_latitude);
    ret = ret && m_navigation_service.addCharacteristic(m_longitude);
    ret = ret && m_navigation_service.addCharacteristic(m_track_angle);

    return ret;
}

/** \brief Start the BLE service */
bool NavigationService::start()
{
    bool ret = true;

    // TODO : register to GNSS values

    return ret;
}

/** \brief Update the BLE service characteristics values */
void NavigationService::updateCharacteristicsValues()
{
    m_speed.update(0u);
    m_latitude.update(0.);
    m_longitude.update(0.);
    m_track_angle.update(0u);
}

}
