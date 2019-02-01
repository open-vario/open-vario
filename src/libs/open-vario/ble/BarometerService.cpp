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

#include "BarometerService.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
BarometerService::BarometerService()
: OpenVarioBleServiceBase()

, m_barometer_service("Barometer service", "d29a5ba1-e46c-4e2c-a1b7-05f21091a216")

, m_pressure("Pressure", "a59b4f7f-47ec-4515-b561-497209d3e8f2", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_temperature("Temperature", "88db8fd5-8362-429b-bfc8-c74aa6c2de44", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)

, m_barometer_event_handler()
, m_thermometer_event_handler()

, m_barometer_values()
, m_thermometer_values()
{}


/** \brief Initialize the BLE service */
bool BarometerService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_barometer_service.addCharacteristic(m_pressure);
    ret = ret && m_barometer_service.addCharacteristic(m_temperature);
 
    return ret;
}

/** \brief Start the BLE service */
bool BarometerService::start()
{
    bool ret = true;

    // Register to barometer and thermometer events
    IBarometer& barometer = IOpenVarioApp::getInstance().getBarometer();
    m_barometer_event_handler = NANO_STL_EVENT_HANDLER_M(BarometerService, onBarometerValues, const BarometerValues&);
    ret = ret && barometer.barometerValuesEvent().bind(m_barometer_event_handler);

    IThermometer& thermometer = IOpenVarioApp::getInstance().getThermometer();
    m_thermometer_event_handler = NANO_STL_EVENT_HANDLER_M(BarometerService, onThermometerValues, const ThermometerValues&);
    ret = ret && thermometer.thermometerValuesEvent().bind(m_thermometer_event_handler);

    return ret;
}

/** \brief Update the BLE service characteristics values */
void BarometerService::updateCharacteristicsValues()
{
    m_pressure.update(m_barometer_values.pressure);
    m_temperature.update(m_thermometer_values.temperature);
}

/** \brief Called when new barometer values have been computed */
void BarometerService::onBarometerValues(const BarometerValues& baro_values)
{
    NANO_STL_MEMCPY(&m_barometer_values, &baro_values, sizeof(BarometerValues));
}

/** \brief Called when new thermometer values have been computed */
void BarometerService::onThermometerValues(const ThermometerValues& temp_values)
{
    NANO_STL_MEMCPY(&m_thermometer_values, &temp_values, sizeof(ThermometerValues));
}

}
