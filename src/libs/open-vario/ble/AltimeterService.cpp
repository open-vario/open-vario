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
: OpenVarioBleServiceBase()

, m_altimeter_service("Altimeter service", "516c5737-8250-493b-bb95-b2a16f65110e")

, m_main_alti("Main altitude", "f033de08-eda3-46a2-9918-19e123297152", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_1("Altitude 1", "b176dd1b-d98e-4707-b51d-d0e31223f776", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_2("Altitude 2", "e4c54ec3-e4b3-43a3-9eb0-9790615f68c3", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_3("Altitude 3", "2a0934e3-7127-46c0-90a9-d6a5cbb51fa6", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)
, m_alti_4("Altitude 4", "80b81b29-791a-4b98-bc76-c6a85539b844", true, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY | IBleCharacteristic::PROP_WRITE)

, m_altimeter_event_handler()
, m_altimeter_values()
{}


/** \brief Initialize the BLE service */
bool AltimeterService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_altimeter_service.addCharacteristic(m_main_alti);
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
