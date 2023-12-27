
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
#if 0
    /** @brief Dashboard */
    dashboard,
    /** @brief Infos */
    infos,
    /** @brief Screen OFF */
    screen_off,
#endif
    /** @brief Number of screens */
    count
};

} // namespace ov

#endif // OV_HMI_H
