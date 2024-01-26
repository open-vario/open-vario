

#ifndef OV_I_ACCELEROMETER_SENSOR_H
#define OV_I_ACCELEROMETER_SENSOR_H

#include <cstdint>

namespace ov
{

/** @brief Interface for accelerometer sensor implementations */
class i_accelerometer_sensor
{
  public:
    /** @brief Accelerometer sensor data */
    struct data
    {
        /** @brief Acceleration on X (1000 = 1g) */
        int16_t x_accel;
        /** @brief Acceleration on Y (1000 = 1g) */
        int16_t y_accel;
        /** @brief Acceleration on Z (1000 = 1g) */
        int16_t z_accel;
        /** @brief Total acceleration */
        int16_t total_accel;
        /** @brief Indicate if the data is valid */
        bool is_valid;
    };

    /** @brief Destructor */
    virtual ~i_accelerometer_sensor() { }

    /** @brief Get the accelerometer sensor data */
    virtual data get_data() = 0;
};

} // namespace ov

#endif // OV_I_ACCELEROMETER_SENSOR_H
