
#ifndef OV_HMI_H
#define OV_HMI_H

namespace ov
{

/** @brief Buttons */
enum class button
{
    /** @brief 'Previous' */
    previous,
    /** @brief 'Next' */
    next,
    /** @brief 'Select' */
    select,
    /** @brief Number of buttons */
    count
};

/** @brief Button events */
enum class button_event
{
    /** @brief Button pushed down */
    down,
    /** @brief Short push */
    short_push,
    /** @brief Long push */
    long_push,
    /** @brief Button released */
    up
};

/** @brief HMI screens */
enum class hmi_screen
{
    /** @brief Splash screen */
    splash,
    /** @brief Dashboard 1 */
    dashboard1,
    /** @brief Dashboard 2 */
    dashboard2,
    /** @brief Dashboard 3 */
    dashboard3,
    /** @brief BLE */
    ble,
    /** @brief GNSS */
    gnss,
    /** @brief Settings */
    settings,
    /** @brief Settings - Display */
    settings_display,
    /** @brief Settings - Exit */
    settings_exit,
    /** @brief Number of screens */
    count
};

} // namespace ov

#endif // OV_HMI_H
