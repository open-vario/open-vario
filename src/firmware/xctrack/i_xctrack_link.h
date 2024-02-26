/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_I_XCTRACK_LINK_H
#define OV_I_XCTRACK_LINK_H

#include "i_usb_cdc.h"

namespace ov
{

/** @brief Interface for the link with the XCTrack application */
class i_xctrack_link
{
  public:
    /** @brief Destructor */
    virtual ~i_xctrack_link() { }

    /** @brief Get the USB CDC link */
    virtual i_usb_cdc& get_usb() = 0;

    /** @brief Indicate if the link is active */
    virtual bool is_active() const = 0;

    /** @brief Change the active status of the link */
    virtual void set_active(bool is_active) = 0;
};

} // namespace ov

#endif // OV_I_XCTRACK_LINK_H
