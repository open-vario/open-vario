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

, m_altitudes("Altitudes", "f033de08-eda3-46a2-9918-19e123297152", 10, IBleCharacteristic::PROP_READ | IBleCharacteristic::PROP_NOTIFY)
, m_command("Command", "b176dd1b-d98e-4707-b51d-d0e31223f776", true, IBleCharacteristic::PROP_WRITE)

, m_altimeter_event_handler()
, m_altimeter_values()
{}


/** \brief Initialize the BLE service */
bool AltimeterService::init()
{
    bool ret = true;

    // Fill BLE service with characteristics
    ret = ret && m_altimeter_service.addCharacteristic(m_altitudes);
    ret = ret && m_altimeter_service.addCharacteristic(m_command);

    return ret;
}

/** \brief Start the BLE service */
bool AltimeterService::start()
{
    bool ret = true;

    // Register to characteristics events
    ret = ret && m_command.registerListener(*this);
    if (ret)
    {
        // Register to altimeter events
        IAltimeter& altimeter = IOpenVarioApp::getInstance().getAltimeter();
        m_altimeter_event_handler = NANO_STL_EVENT_HANDLER_M(AltimeterService, onAltimeterValues, const AltimeterValues&);
        ret = ret && altimeter.altimeterValuesEvent().bind(m_altimeter_event_handler);
    }

    return ret;
}

/** \brief Update the BLE service characteristics values */
void AltimeterService::updateCharacteristicsValues()
{
    const uint16_t main_alti = static_cast<int16_t>(m_altimeter_values.main_alti / 10);
    const uint16_t alti_1 = static_cast<int16_t>(m_altimeter_values.alti_1 / 10);
    const uint16_t alti_2 = static_cast<int16_t>(m_altimeter_values.alti_2 / 10);
    const uint16_t alti_3 = static_cast<int16_t>(m_altimeter_values.alti_3 / 10);
    const uint16_t alti_4 = static_cast<int16_t>(m_altimeter_values.alti_4 / 10);
    const uint8_t altitudes[] = {
                                    static_cast<uint8_t>(main_alti & 0xFFu),
                                    static_cast<uint8_t>((main_alti >> 8u) & 0xFFu),
                                    static_cast<uint8_t>(alti_1 & 0xFFu),
                                    static_cast<uint8_t>((alti_1 >> 8u) & 0xFFu),
                                    static_cast<uint8_t>(alti_2 & 0xFFu),
                                    static_cast<uint8_t>((alti_2 >> 8u) & 0xFFu),
                                    static_cast<uint8_t>(alti_3 & 0xFFu),
                                    static_cast<uint8_t>((alti_3 >> 8u) & 0xFFu),
                                    static_cast<uint8_t>(alti_4 & 0xFFu),
                                    static_cast<uint8_t>((alti_4 >> 8u) & 0xFFu)
                                };
    m_altitudes.update(altitudes, sizeof(altitudes));
}

/** \brief Called when new altimeter values have been computed */
void AltimeterService::onAltimeterValues(const AltimeterValues& alti_values)
{
    NANO_STL_MEMCPY(&m_altimeter_values, &alti_values, sizeof(AltimeterValues));
}

/** \brief Called when the characteristic's value has changed */
void AltimeterService::onValueChanged(IBleCharacteristic& characteristic, const bool from_stack, const void* new_value, const uint16_t new_value_size)
{
    // Only handle modifications mades by the client
    if (from_stack)
    {
        IAltimeter& altimeter = IOpenVarioApp::getInstance().getAltimeter();
        const uint32_t value = *reinterpret_cast<const uint32_t*>(new_value);
        const int32_t altitude = static_cast<int32_t>(static_cast<int16_t>(value & 0xFFFFu)) * 10;
        const uint16_t command = static_cast<uint16_t>((value >> 8u) & 0xFFFFu);
        switch (command)
        {
            case CMD_SET_MAIN_ALTI:
            {
                altimeter.setReferenceAltitude(altitude);
                break;
            }

            case CMD_SET_ALTI_1:
            {
                altimeter.setAlti1(altitude);
                break;
            }

            case CMD_SET_ALTI_2:
            {
                altimeter.setAlti2(altitude);
                break;
            }

            case CMD_SET_ALTI_3:
            {
                altimeter.setAlti3(altitude);
                break;
            }

            case CMD_SET_ALTI_4:
            {
                altimeter.setAlti4(altitude);
                break;
            }

            default:
            {
                // Should not happen => ignore
                break;
            }
        }
    }
}

}
