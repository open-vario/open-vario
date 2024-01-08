
#ifndef I_IO_PIN_H
#define I_IO_PIN_H

#include "i_input_pin.h"
#include "i_output_pin.h"

namespace ov
{

/** @brief Interface for pin drivers with both input and output capabilities implementations */
class i_io_pin : public i_input_pin, public i_output_pin
{
  public:
    /** @brief Destructor */
    virtual ~i_io_pin() { }
};

} // namespace ov

#endif // I_IO_PIN_H
