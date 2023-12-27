
#ifndef OV_I_BOARD_H
#define OV_I_BOARD_H

#include "i_button.h"
#include "i_display.h"
#include "i_serial.h"
#include "i_storage_memory.h"

namespace ov
{

/** @brief Interface for boards implementations */
class i_board
{
  public:
    /** @brief Destructor */
    virtual ~i_board() { }

    /** @brief Get the debug serial port */
    virtual i_serial& get_debug_port() = 0;

    /** @brief Get the storage memory */
    virtual i_storage_memory& get_storage_memory() = 0;

    /** @brief Get the display */
    virtual i_display& get_display() = 0;

    /** @brief Get the 'Previous' button */
    virtual i_button& get_previous_button() = 0;

    /** @brief Get the 'Next' button */
    virtual i_button& get_next_button() = 0;

    /** @brief Get the 'Select' button */
    virtual i_button& get_select_button() = 0;
};

} // namespace ov

#endif // OV_I_BOARD_H