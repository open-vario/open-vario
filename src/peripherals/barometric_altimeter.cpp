
#include "barometric_altimeter.h"

#include <cmath>

namespace ov
{

/** @brief Constructor */
barometric_altimeter::barometric_altimeter(i_barometric_sensor& barometric_sensor)
    : m_barometric_sensor(barometric_sensor), m_ref_temp(288.), m_ref_pressure(1013.), m_ref_alti(0.), m_data{}
{
    // Default reference :
    // - 15°C = 288°K
    // - 1013mbar
    // - 0m
}

/** @brief Initialize the barometric altimeter */
bool barometric_altimeter::init()
{
    // Nothing to do for now
    return true;
}

/** @brief Set the references for the altitude computation */
void barometric_altimeter::set_references(const int16_t ref_temperature, const uint32_t ref_pressure, const int32_t ref_altitude)
{
    // Convert units for internal computation
    m_ref_temp     = static_cast<double>(ref_temperature + 2730u) / 10.;
    m_ref_pressure = static_cast<double>(ref_pressure) / 100.;
    m_ref_alti     = static_cast<double>(ref_altitude) / 10.;
}

/** @brief Get the barometric altimeter data */
i_barometric_altimeter::data barometric_altimeter::get_data()
{
    // Read sensor data
    auto sensor_data = m_barometric_sensor.get_data();

    // Compute new altitude :
    //
    // dA = A - Aref = Tref / DT * [1 - (P / Pref) ^ ((R * DT) / (M * g))]
    //
    // With:
    // - R = 8.314 J.K^-1.mol^-1
    // - DT = 0.0065 K/m
    // - g = 9.807 m.s^-2
    // - M = 0.02896 kg.mol^-1
    const double P  = static_cast<double>(sensor_data.pressure) / 100.;
    double       dA = (m_ref_temp / 0.0065) * (1 - pow(P / m_ref_pressure, 0.190));
    double       A  = dA + m_ref_alti;

    // Convert computed value
    m_data.altitude = static_cast<int32_t>(A * 10.);

    // Save sensor data
    m_data.pressure    = sensor_data.pressure;
    m_data.temperature = sensor_data.temperature;
    m_data.is_valid    = sensor_data.is_valid;

    return m_data;
}

} // namespace ov
