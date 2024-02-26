/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_USB_CDC_H
#define OV_I_USB_CDC_H

#include "i_serial.h"

namespace ov
{

/** @brief Interface for USB CDC drivers implementations */
class i_usb_cdc : public i_serial
{
  public:
    /** @brief Destructor */
    virtual ~i_usb_cdc() { }

    /** @brief Listener to USB CDC events */
    class i_listener
    {
      public:
        /** @brief Destructor */
        virtual ~i_listener() { }

        /** @brief Called when USB cable has been plugged and CDC link is ready */
        virtual void on_cdc_link_up() = 0;

        /** @brief Called when USB cable has been unplugged and/or CDC link is down */
        virtual void on_cdc_link_down() = 0;
    };

    /** @brief Register a listener to USB CDC events */
    virtual void register_listener(i_listener& listener) = 0;

    /** @brief Indicate if the USB CDC link is up */
    virtual bool is_link_up() = 0;
};

} // namespace ov

#endif // OV_I_USB_CDC_H
