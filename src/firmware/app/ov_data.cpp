
#include "ov_data.h"
#include "lock_guard.h"
#include "mutex.h"

namespace ov
{
namespace data
{

/** @brief Sensor data */
static ov_data s_data;

/** @brief Mutex to protect concurrent access to sensor data */
static mutex s_mutex;

// Getters

/** @brief Get the sensor data */
ov_data get()
{
    lock_guard<mutex> lock(s_mutex);
    return s_data;
}

/** @brief Get the GNSS data */
i_gnss::data get_gnss()
{
    lock_guard<mutex> lock(s_mutex);
    return s_data.gnss;
}

// Setters

/** @brief Set the GNSS data */
void set_gnss(const i_gnss::data& data)
{
    lock_guard<mutex> lock(s_mutex);
    s_data.gnss = data;
}

/** @brief Invalidate GNSS data */
void invalidate_gnss()
{
    lock_guard<mutex> lock(s_mutex);
    s_data.gnss = {};
}

} // namespace data
} // namespace ov
