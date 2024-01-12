
#ifndef OV_FLIGHT_SCREEN_H
#define OV_FLIGHT_SCREEN_H

#include "base_screen.h"

namespace ov
{

// Forward declaration
class i_flight_recorder;

/** @brief Flight screen */
class flight_screen : public base_screen
{
  public:
    /** @brief Constructor */
    flight_screen(i_hmi_manager& hmi_manager, i_flight_recorder& recorder);

    /** @brief Button event */
    void event(button bt, button_event bt_event) override;

  private:
    /** @brief Flight recorder */
    i_flight_recorder& m_recorder;
    /** @brief Flight label */
    YACSWL_label_t m_flight_label;
    /** @brief Flight status label */
    YACSWL_label_t m_flight_status_label;
    /** @brief Flight start label */
    YACSWL_label_t m_flight_start_label;
    /** @brief Status label string */
    char m_flight_status_string[18u];
    /** @brief Flight start string 1 */
    static constexpr const char* m_flight_start_string1 = "Press SELECT";
    /** @brief Flight start string 2 */
    static constexpr const char* m_flight_start_string2 = "to start!";

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_FLIGHT_SCREEN_H
