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
: m_barometer_service("Barometer service", {0xd2u, 0x9au, 0x5bu, 0xa1u, 0xe4u, 0x6cu, 0x4eu, 0x2cu, 0xa1u, 0xb7u, 0x05u, 0xf2u, 0x10u, 0x91u, 0xa2u, 0x16u})

, m_pressure("Pressure", {0xa5u, 0x9bu, 0x4fu, 0x7fu, 0x47u, 0xecu, 0x45u, 0x15u, 0xb5u, 0x61u, 0x49u, 0x72u, 0x09u, 0xd3u, 0xe8u, 0xf2u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_temperature("Temperature", {0x88u, 0xdbu, 0x8fu, 0xd5u, 0x83u, 0x62u, 0x42u, 0x9bu, 0xbfu, 0xc8u, 0xc7u, 0x4au, 0xa6u, 0xc2u, 0xdeu, 0x44u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_min_temperature("Min temperature", {0x88u, 0x1au, 0x6du, 0x3au, 0x83u, 0xacu, 0x47u, 0xc6u, 0x87u, 0xdau, 0x3du, 0x56u, 0x47u, 0x0fu, 0xfcu, 0xd4u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_max_temperature("Max temperature", {0x72u, 0x48u, 0xa6u, 0x5cu, 0xd4u, 0x83u, 0x42u, 0x5fu, 0xabu, 0x8bu, 0x91u, 0x7du, 0x01u, 0x0eu, 0x90u, 0xc5u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)

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
    ret = ret && m_barometer_service.addCharacteristic(m_min_temperature);
    ret = ret && m_barometer_service.addCharacteristic(m_max_temperature);

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
    if (ret)
    {
        // Register to characteristics change of values
        ret = ret && m_min_temperature.registerListener(*this);
        ret = ret && m_max_temperature.registerListener(*this);
    }

    return ret;
}

/** \brief Update the BLE service characteristics values */
void BarometerService::updateCharacteristicsValues()
{
    m_pressure.update(m_barometer_values.pressure);
    m_temperature.update(m_thermometer_values.temperature);
    m_min_temperature.update(m_thermometer_values.min_temperature);
    m_max_temperature.update(m_thermometer_values.max_temperature);
}

/** \brief Called when the characteristic's value has changed */
void BarometerService::onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size)
{
    // Only handle changes made from the stack side (ie. by the client)
    if (from_stack)
    {
        if ((&characteristic == static_cast<IBleCharacteristic*>(&m_min_temperature)) ||
            (&characteristic == static_cast<IBleCharacteristic*>(&m_max_temperature)))
        {
            // Reset min and max temperature
            IThermometer& thermometer = IOpenVarioApp::getInstance().getThermometer();
            thermometer.resetMinMax();
        }
        else
        {
            // Ignore
        }
    }
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
