
#ifndef OV_USB_SCREEN_H
#define OV_USB_SCREEN_H

#include "base_screen.h"

namespace ov
{

// Forward declaration
class i_usb_cdc;

/** @brief BLE screen */
class usb_screen : public base_screen
{
  public:
    /** @brief Constructor */
    usb_screen(i_hmi_manager& hmi_manager, i_usb_cdc& usb);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief USB link */
    i_usb_cdc& m_usb;
    /** @brief USB label */
    YACSWL_label_t m_usb_label;
    /** @brief Connection status label */
    YACSWL_label_t m_connection_status_label;
    /** @brief Connected string */
    static constexpr const char* m_connected_string = "Connected";
    /** @brief Disconnected string */
    static constexpr const char* m_not_connected_string = "Not connected";

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_USB_SCREEN_H
