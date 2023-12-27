
#ifndef OV_BASE_SCREEN_H
#define OV_BASE_SCREEN_H

#include "i_hmi_screen.h"

#include <YACSWL.h>

namespace ov
{

// Forward declaration
class i_hmi_manager;

/** @brief Base class for screen implementations */
class base_screen : public i_hmi_screen
{
  public:
    /** @brief Constructor */
    base_screen(hmi_screen screen_id, i_hmi_manager& hmi_manager);

    /** @brief Screen identifier */
    hmi_screen get_id() const override { return m_screen_id; }

    /** @brief Initialize the screen */
    void init(YACSGL_frame_t& frame) override;

    /** @brief Button event */
    void event(button, button_event) override { }

    /** @brief Refresh the contents of the screen */
    void refresh(YACSGL_frame_t& frame) override;

    /** @brief Set the night mode */
    void set_night_mode(bool is_on) override;

  protected:
    /** @brief Root widget */
    YACSWL_widget_t m_root_widget;

    /** @brief Initialize the screen */
    virtual void on_init(YACSGL_frame_t& frame) = 0;

    /** @brief Refresh the contents of the screen */
    virtual void on_refresh(YACSGL_frame_t& frame) { (void)frame; }

    /** @brief Switch to the specified screen */
    void switch_to_screen(hmi_screen screen);

    /** @brief Get HMI manager */
    i_hmi_manager& get_hmi() { return m_hmi_manager; }

  private:
    /** @brief Screen identifier */
    const hmi_screen m_screen_id;
    /** @brief HMI manager */
    i_hmi_manager& m_hmi_manager;
};

} // namespace ov

#endif // OV_BASE_SCREEN_H
