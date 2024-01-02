
#ifndef OV_BLE_SCREEN_H
#define OV_BLE_SCREEN_H

#include "base_screen.h"

namespace ov
{

// Forward declaration
class i_ble_stack;

/** @brief BLE screen */
class ble_screen : public base_screen
{
  public:
    /** @brief Constructor */
    ble_screen(i_hmi_manager& hmi_manager, i_ble_stack& ble_stack);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief BLE stack */
    i_ble_stack& m_ble_stack;
    /** @brief BLE label */
    YACSWL_label_t m_ble_label;
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

#endif // OV_BLE_SCREEN_H
