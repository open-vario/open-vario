
#ifndef I_IO_PIN_H
#define I_IO_PIN_H

namespace ov
{

/** @brief Interface for input/output pins drivers implementations */
class i_io_pin
{
  public:
    /** @brief Pin level */
    typedef bool Level;
    /** @brief High level */
    static const Level HIGH = true;
    /** @brief Low level */
    static const Level LOW = false;
};

} // namespace ov

#endif // I_IO_PIN_H
