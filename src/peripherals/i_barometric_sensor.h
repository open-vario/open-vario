

#ifndef OV_I_BAROMETRIC_SENSOR_H
#define OV_I_BAROMETRIC_SENSOR_H

#include <cstdint>

namespace ov
{

/** @brief Interface for barometric sensor implementations */
class i_barometric_sensor
{
  public:
    /** @brief Barometric sensor data */
    struct data
    {
        /** @brief Pressure (1 = 0.01mbar) */
        int32_t pressure;
        /** @brief Temperature (1 = 0.1°C) */
        int16_t temperature;
        /** @brief Indicate if the data is valid */
        bool is_valid;
    };

    /** @brief Destructor */
    virtual ~i_barometric_sensor() { }

    /** @brief Get the barometric sensor data */
    virtual data get_data() = 0;
};

} // namespace ov

#endif // OV_I_BAROMETRIC_SENSOR_H
