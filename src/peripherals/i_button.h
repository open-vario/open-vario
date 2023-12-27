
#ifndef OV_I_BUTTON_H
#define OV_I_BUTTON_H

namespace ov
{

/** @brief Interface for buttons implementations */
class i_button
{
  public:
    /** @brief Destructor */
    virtual ~i_button() { }

    /** @brief Indicate if the button is in pushed state */
    virtual bool is_pushed() = 0;
};

} // namespace ov

#endif // OV_I_BUTTON_H
