
#include "ble_characteristic.h"

namespace ov
{

/** @brief Constructor */
ble_characteristic_base::ble_characteristic_base(
    i_ble_service& service, const char* name, const ble_uuid& uuid, size_t size, uint8_t properties)
    : m_service(service),
      m_name(name),
      m_uuid(uuid),
      m_handle(0u),
      m_properties(properties),
      m_size(size),
      m_app_handler(),
      m_stack_handler()
{
}

/** @brief Update the value of the characteristic from application */
bool ble_characteristic_base::update_value_from_app(const void* value, size_t new_size)
{
    bool ret = false;
    if (!m_stack_handler.is_null())
    {
        ret = m_stack_handler.invoke(*this, static_cast<const void*&&>(value), static_cast<size_t&&>(new_size));
    }
    return ret;
}

/** @brief Update the value of the characteristic from stack */
bool ble_characteristic_base::update_value_from_stack(const void* value, size_t new_size)
{
    bool ret = false;
    if (!m_app_handler.is_null())
    {
        ret = m_app_handler.invoke(*this, static_cast<const void*&&>(value), static_cast<size_t&&>(new_size));
    }
    return ret;
}

} // namespace ov
