
#ifndef OV_SETTINGS_SCREEN_H
#define OV_SETTINGS_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Settings screen */
class settings_screen : public base_screen
{
  public:
    /** @brief Constructor */
    settings_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Settings label */
    YACSWL_label_t m_settings_label;

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_SETTINGS_SCREEN_H
