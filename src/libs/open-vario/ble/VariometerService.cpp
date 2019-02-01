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

#include "VariometerService.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
VariometerService::VariometerService()
: OpenVarioBleServiceBase()
, m_variometer_service("Variometer service", "ae283ac8-786f-42ef-b694-b7faf492cae9")

, m_vario("Vario", "7708157c-132f-4d21-a1d9-c9768732b4e9", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_acceleration("Acceleration", "9e13b15f-3582-433b-9034-55ac8881ee4f", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)

, m_variometer_event_handler()
, m_variometer_values()
{}


/** \brief Initialize the BLE service */
bool VariometerService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_variometer_service.addCharacteristic(m_vario);
    ret = ret && m_variometer_service.addCharacteristic(m_acceleration);

    return ret;
}

/** \brief Start the BLE service */
bool VariometerService::start()
{
    bool ret = true;

    // Register to variometer events
    IVariometer& variometer = IOpenVarioApp::getInstance().getVariometer();
    m_variometer_event_handler = NANO_STL_EVENT_HANDLER_M(VariometerService, onVariometerValues, const VariometerValues&);
    ret = ret && variometer.variometerValuesEvent().bind(m_variometer_event_handler);

    return ret;
}

/** \brief Update the BLE service characteristics values */
void VariometerService::updateCharacteristicsValues()
{
    m_vario.update(m_variometer_values.vario);
    m_acceleration.update(0u);
}

/** \brief Called when new variometer values have been computed */
void VariometerService::onVariometerValues(const VariometerValues& vario_values)
{
    NANO_STL_MEMCPY(&m_variometer_values, &vario_values, sizeof(VariometerValues));
}

}
