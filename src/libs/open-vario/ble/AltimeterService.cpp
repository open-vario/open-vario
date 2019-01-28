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

#include "AltimeterService.h"
#include "IOpenVarioApp.h"

namespace open_vario
{


/** \brief Constructor */
AltimeterService::AltimeterService()
: m_altimeter_service("Altimeter service", {0x51u, 0x6cu, 0x57u, 0x37u, 0x82u, 0x50u, 0x49u, 0x3bu, 0xbbu, 0x95u, 0xb2u, 0xa1u, 0x6fu, 0x65u, 0x11u, 0x0eu})

, m_main_alti("Main altitude", {0xf0u, 0x33u, 0xdeu, 0x08u, 0xedu, 0xa3u, 0x46u, 0xa2u, 0x99u, 0x18u, 0x19u, 0xe1u, 0x23u, 0x29u, 0x71u, 0x52u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_min_alti("Min altitude", {0x87u, 0x65u, 0xecu, 0x19u, 0xd2u, 0xedu, 0x4cu, 0x9eu, 0x8au, 0xc7u, 0x1fu, 0x0cu, 0x51u, 0x6au, 0x2du, 0x6bu}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_max_alti("Max altitude", {0x06u, 0x71u, 0x63u, 0xe5u, 0xb0u, 0x97u, 0x4bu, 0xd5u, 0x81u, 0xa6u, 0x3fu, 0xbfu, 0x4du, 0xbfu, 0xb6u, 0xafu}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_1("Altitude 1", {0xb1u, 0x76u, 0xddu, 0x1bu, 0xd9u, 0x8eu, 0x47u, 0x07u, 0xb5u, 0x1du, 0xd0u, 0xe3u, 0x12u, 0x23u, 0xf7u, 0x76u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_2("Altitude 2", {0xe4u, 0xc5u, 0x4eu, 0xc3u, 0xe4u, 0xb3u, 0x43u, 0xa3u, 0x9eu, 0xb0u, 0x97u, 0x90u, 0x61u, 0x5fu, 0x68u, 0xc3u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_3("Altitude 3", {0x2au, 0x09u, 0x34u, 0xe3u, 0x71u, 0x27u, 0x46u, 0xc0u, 0x90u, 0xa9u, 0xd6u, 0xa5u, 0xcbu, 0xb5u, 0x1fu, 0xa6u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_4("Altitude 4", {0x80u, 0xb8u, 0x1bu, 0x29u, 0x79u, 0x1au, 0x4bu, 0x98u, 0xbcu, 0x76u, 0xc6u, 0xa8u, 0x55u, 0x39u, 0xb8u, 0x44u}, true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)

, m_altimeter_event_handler()
, m_altimeter_values()
{}


/** \brief Initialize the BLE service */
bool AltimeterService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_altimeter_service.addCharacteristic(m_main_alti);
    ret = ret && m_altimeter_service.addCharacteristic(m_min_alti);
    ret = ret && m_altimeter_service.addCharacteristic(m_max_alti);
    ret = ret && m_altimeter_service.addCharacteristic(m_alti_1);
    ret = ret && m_altimeter_service.addCharacteristic(m_alti_2);
    ret = ret && m_altimeter_service.addCharacteristic(m_alti_3);
    ret = ret && m_altimeter_service.addCharacteristic(m_alti_4);

    return ret;
}

/** \brief Start the BLE service */
bool AltimeterService::start()
{
    bool ret = true;

    // Register to altimeter events
    IAltimeter& altimeter = IOpenVarioApp::getInstance().getAltimeter();
    m_altimeter_event_handler = NANO_STL_EVENT_HANDLER_M(AltimeterService, onAltimeterValues, const AltimeterValues&);
    ret = ret && altimeter.altimeterValuesEvent().bind(m_altimeter_event_handler);

    return ret;
}

/** \brief Update the BLE service characteristics values */
void AltimeterService::updateCharacteristicsValues()
{
    m_main_alti.update(static_cast<int16_t>(m_altimeter_values.main_alti / 10));
    m_min_alti.update(static_cast<int16_t>(m_altimeter_values.min_alti / 10));
    m_max_alti.update(static_cast<int16_t>(m_altimeter_values.max_alti / 10));
    m_alti_1.update(static_cast<int16_t>(m_altimeter_values.alti_1 / 10));
    m_alti_2.update(static_cast<int16_t>(m_altimeter_values.alti_2 / 10));
    m_alti_3.update(static_cast<int16_t>(m_altimeter_values.alti_3 / 10));
    m_alti_4.update(static_cast<int16_t>(m_altimeter_values.alti_4 / 10));
}

/** \brief Called when new altimeter values have been computed */
void AltimeterService::onAltimeterValues(const AltimeterValues& alti_values)
{
    NANO_STL_MEMCPY(&m_altimeter_values, &alti_values, sizeof(AltimeterValues));
}

}
