

#ifndef OV_BAROMETRIC_ALTIMETER_H
#define OV_BAROMETRIC_ALTIMETER_H

#include "i_barometric_altimeter.h"
#include "i_barometric_sensor.h"

namespace ov
{

/** @brief Interface for barometric altimeter implementations */
class barometric_altimeter : public i_barometric_altimeter
{
  public:
    /** @brief Constructor */
    barometric_altimeter(i_barometric_sensor& barometric_sensor);

    /** @brief Initialize the barometric altimeter */
    bool init();

    /** @brief Set the references for the altitude computation */
    void set_references(const int16_t ref_temperature, const uint32_t ref_pressure, const int32_t ref_altitude) override;

    /** @brief Get the barometric altimeter data */
    data get_data() override;

  private:
    /** @brief Barometric sensor */
    i_barometric_sensor& m_barometric_sensor;
    /** @brief Reference temperature */
    double m_ref_temp;
    /** @brief Reference pressure */
    double m_ref_pressure;
    /** @brief Reference altitude */
    double m_ref_alti;
    /** @brief Sensor data */
    data m_data;
};

} // namespace ov

#endif // OV_BAROMETRIC_ALTIMETER_H
