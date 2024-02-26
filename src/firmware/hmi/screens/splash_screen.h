/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_SPLASH_SCREEN_H
#define OV_SPLASH_SCREEN_H

#include "base_screen.h"

namespace ov
{

/** @brief Splash screen */
class splash_screen : public base_screen
{
  public:
    /** @brief Constructor */
    splash_screen(i_hmi_manager& hmi_manager);

  private:
    /** @brief Name label */
    YACSWL_label_t m_name_label;
    /** @brief Version label */
    YACSWL_label_t m_version_label;
    /** @brief Progress bar */
    YACSWL_progress_bar_t m_progress_bar;
    /** @brief Version string */
    char m_version_string[16u];

    /** @brief Initialize the screen */
    void on_init(YACSGL_frame_t& frame) override;

    /** @brief Refresh the contents of the screen */
    void on_refresh(YACSGL_frame_t& frame) override;
};

} // namespace ov

#endif // OV_SPLASH_SCREEN_H
