/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#include "ov_app.h"

/** @brief Open Vario application */
static ov::ov_app ov_app;

/** @brief Entry point */
int main()
{
    // Initialize application
    if (ov_app.init())
    {
        // Start application
        ov_app.start();
    }

    // Shall never happen
    return 0;
}
