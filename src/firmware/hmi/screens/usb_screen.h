/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_USB_SCREEN_H
#define OV_USB_SCREEN_H

#include "base_screen.h"

namespace ov
{

// Forward declaration
class i_xctrack_link;

/** @brief BLE screen */
class usb_screen : public base_screen
{
  public:
    /** @brief Constructor */
    usb_screen(i_hmi_manager& hmi_manager, i_xctrack_link& xctrack_link);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief XCTrack link */
    i_xctrack_link& m_xctrack;
    /** @brief USB label */
    YACSWL_label_t m_usb_label;
    /** @brief Connection status label */
    YACSWL_label_t m_connection_status_label;
    /** @brief Mode label */
    YACSWL_label_t m_mode_label;
    /** @brief Connected string */
    static constexpr const char* m_connected_string = "Connected";
    /** @brief Disconnected string */
    static constexpr const char* m_not_connected_string = "Not connected";
    /** @brief XCTrack string */
    static constexpr const char* m_xctrack_string = "XCTrack";
    /** @brief OV ToolBox string */
    static constexpr const char* m_ovtoolbox_string = "OV ToolBox";

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_USB_SCREEN_H
