
#ifndef OV_DASHBOARD2_SCREEN_H
#define OV_DASHBOARD2_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Dashboard 2 screen */
class dashboard2_screen : public base_screen
{
  public:
    /** @brief Constructor */
    dashboard2_screen(i_hmi_manager& hmi_manager);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Sink rate label */
    YACSWL_label_t m_sink_rate_label;
    /** @brief Glide ratio label */
    YACSWL_label_t m_glide_ratio_label;
    /** @brief Speed label */
    YACSWL_label_t m_speed_label;
    /** @brief Glide ratio string */
    char m_glide_ratio_string[10u];
    /** @brief Speed string */
    char m_speed_string[16u];
    /** @brief Sink rate string */
    char m_sink_rate_string[14u];

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_DASHBOARD2_SCREEN_H
