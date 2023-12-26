
#ifndef OV_I_DISPLAY_H
#define OV_I_DISPLAY_H

#include <cstddef>
#include <cstdint>

namespace ov
{

/** @brief Interface for displays (LCD, OLED...) implementations */
class i_display
{
  public:
    /** @brief Destructor */
    virtual ~i_display() { }

    /** @brief Get the display width in pixels */
    virtual size_t get_width() = 0;

    /** @brief Get the display heigth in pixels */
    virtual size_t get_heigth() = 0;

    /** @brief Get the frame buffer of the display */
    virtual uint8_t* get_frame_buffer() = 0;

    /** @brief Reset the display */
    virtual bool reset() = 0;

    /** @brief Turn ON the display */
    virtual bool turn_on() = 0;

    /** @brief Turn OFF the display */
    virtual bool turn_off() = 0;

    /** @brief Refresh the display contents */
    virtual bool refresh() = 0;
};

} // namespace ov

#endif // OV_I_DISPLAY_H
