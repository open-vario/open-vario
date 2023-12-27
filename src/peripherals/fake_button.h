
#ifndef OV_FAKE_BUTTON_H
#define OV_FAKE_BUTTON_H

#include "i_button.h"

namespace ov
{

/** @brief Fake button implementation */
class fake_button : public i_button
{
  public:
    /** @brief Constructor */
    fake_button() : m_is_pushed(false) { }

    /** @brief Indicate if the button is in pushed state */
    bool is_pushed() override { return m_is_pushed; }

    /** @brief Set the button state */
    void set_state(bool is_pushed) { m_is_pushed = is_pushed; }

  private:
    /** @brief Pushed state */
    bool m_is_pushed;
};

} // namespace ov

#endif // OV_FAKE_BUTTON_H
