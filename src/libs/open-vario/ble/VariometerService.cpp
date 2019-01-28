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
: m_variometer_service("Variometer service", {0xaeu, 0x28u, 0x3au, 0xc8u, 0x78u, 0x6fu, 0x42u, 0xefu, 0xb6u, 0x94u, 0xb7u, 0xfau, 0xf4u, 0x92u, 0xcau, 0xe9u})

, m_vario("Vario", {0x77u, 0x08u, 0x15u, 0x7cu, 0x13u, 0x2fu, 0x4du, 0x21u, 0xa1u, 0xd9u, 0xc9u, 0x76u, 0x87u, 0x32u, 0xb4u, 0xe9u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_min_vario("Min vario", {0x4eu, 0x14u, 0x92u, 0x74u, 0x22u, 0xf9u, 0x41u, 0x85u, 0xb7u, 0x93u, 0x59u, 0xa7u, 0xb9u, 0xe8u, 0x2du, 0xb8u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_max_vario("Max vario", {0xbdu, 0x98u, 0xcdu, 0x90u, 0x4du, 0x67u, 0x4bu, 0x55u, 0x90u, 0x0au, 0x20u, 0x4au, 0x75u, 0x32u, 0x6bu, 0x01u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_acceleration("Acceleration", {0x9eu, 0x13u, 0xb1, 0x5fu, 0x35u, 0x82u, 0x43u, 0x3bu, 0x90u, 0x34u, 0x55u, 0xacu, 0x88u, 0x81u, 0xeeu, 0x4fu}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_max_acceleration("Max acceleration", {0x12u, 0x03u, 0x8fu, 0xb7u, 0x12u, 0xf6u, 0x49u, 0xc5u, 0x89u, 0x31u, 0xcau, 0xe4u, 0xafu, 0x25u, 0x01u, 0x9cu}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)

, m_variometer_event_handler()
, m_variometer_values()
{}


/** \brief Initialize the BLE service */
bool VariometerService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_variometer_service.addCharacteristic(m_vario);
    ret = ret && m_variometer_service.addCharacteristic(m_min_vario);
    ret = ret && m_variometer_service.addCharacteristic(m_max_vario);
    ret = ret && m_variometer_service.addCharacteristic(m_acceleration);
    ret = ret && m_variometer_service.addCharacteristic(m_max_acceleration);

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
    m_min_vario.update(m_variometer_values.min_vario);
    m_max_vario.update(m_variometer_values.max_vario);
    m_acceleration.update(0u);
    m_max_acceleration.update(0u);
}

/** \brief Called when new variometer values have been computed */
void VariometerService::onVariometerValues(const VariometerValues& vario_values)
{
    NANO_STL_MEMCPY(&m_variometer_values, &vario_values, sizeof(VariometerValues));
}

}
