
#ifndef OV_I_HMI_MANAGER_H
#define OV_I_HMI_MANAGER_H

#include "hmi.h"

namespace ov
{

/** @brief Interface for the HMI manager implementation */
class i_hmi_manager
{
  public:
    /** @brief Destructor */
    virtual ~i_hmi_manager() { }

    /** @brief Set the next screen */
    virtual void set_next_screen(hmi_screen screen) = 0;

    /** @brief Turn the display ON/OFF */
    virtual void set_display(bool on) = 0;
};

} // namespace ov

#endif // OV_I_HMI_MANAGER_H
