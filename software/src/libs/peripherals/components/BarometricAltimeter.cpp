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

#include "BarometricAltimeter.h"

#include <math.h>

namespace open_vario
{


/** \brief Constructor */
BarometricAltimeter::BarometricAltimeter(IBarometricSensor& baro_sensor)
: m_baro_sensor(baro_sensor)
, m_configured(false)

// Default values are standard atmospheric conditions at sea level
, m_ref_temp(288.) // 15°C
, m_ref_pressure(1013.) // 1013mbar
, m_ref_alti(0.) // 0m
{}


/** \brief Configure the altimeter sensor */
bool BarometricAltimeter::configure()
{
    bool ret = true;

    // Check if already configured
    if (!m_configured)
    {
        // Configure barometric sensor
        ret = m_baro_sensor.configure();

        // Sensor is now configured
        m_configured = ret;
    }

    return ret;
}

/** \brief Read the altitude (1 = 0.1m) */
bool BarometricAltimeter::readAltitude(uint32_t& altitude)
{
    bool ret;

    // Read pressure
    ret = m_baro_sensor.readPressure(m_pressure);

    // Read temperature
    ret = ret && m_baro_sensor.readTemperature(m_temperature);
    if (ret)
    {
        // Compute new altitude :
        //
        // dA = A - Aref = Tref / DT * [1 - (P / Pref) ^ ((R * DT) / (M * g))]
        //
        // With:
        // - R = 8.314 J.K^-1.mol^-1
        // - DT = 0.0065 K/m
        // - g = 9.807 m.s^-2
        // - M = 0.02896 kg.mol^-1
        const double P = static_cast<double>(m_pressure) / 100.;
        double dA = (m_ref_temp / 0.0065) * (1 - pow(P / m_ref_pressure, 0.190));
        double A = dA + m_ref_alti;

        // Convert computed value
        altitude = static_cast<uint32_t>(A * 10.);
    }

    return ret;
}

/** \brief Set a reference altitude (1 = 0.1m) */
bool BarometricAltimeter::setReferenceAltitude(const uint32_t ref_altitude)
{
    bool ret = true;

    //  Get corresponding reference pressure and temperature
    uint32_t pressure = 0u;
    ret = m_baro_sensor.readPressure(pressure);
    int16_t temperature = 0u;
    ret = ret && m_baro_sensor.readTemperature(temperature);
    
    if (ret)
    {
        // Save reference values
        m_ref_alti = static_cast<double>(ref_altitude) / 10.;
        m_ref_pressure = static_cast<double>(pressure) / 100.;
        m_ref_temp = 273. + static_cast<double>(temperature) / 10.;
    }

    return ret;    
}

/** \brief Read the pressure (1 = 0.01mbar) */
bool BarometricAltimeter::readPressure(uint32_t& pressure)
{
    // Return previously read pressure
    m_pressure = m_pressure;
    return true;
}

/** \brief Read the temperature (1 = 0.1°C) */
bool BarometricAltimeter::readTemperature(int16_t& temperature)
{
    // Return previously read temperature
    temperature = m_temperature;
    return true;
}


}
