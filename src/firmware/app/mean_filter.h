
#ifndef OV_MEAN_FILTER_H
#define OV_MEAN_FILTER_H

#include "circular_buffer.h"

namespace ov
{

/** @brief Mean filter with parametrizable depth */
template <typename T, typename S, size_t MAX_DEPTH>
class mean_filter
{
  public:
    /** @brief Constructor */
    mean_filter() : m_values(), m_sum{}, m_mean{} { }

    /** @brief Set a new depth */
    bool set_depth(size_t new_depth)
    {
        bool ret = m_values.set_depth(new_depth);
        if (ret)
        {
            // Clear filter
            m_sum = 0;
        }

        return ret;
    }

    /** @brief Get the depth */
    size_t get_depth() const { return m_values.get_depth(); }

    /** @brief Add value to the filter */
    T add_value(const T& val)
    {
        // Compute mean
        m_sum += static_cast<S>(val);
        m_mean = static_cast<T>(m_sum / static_cast<S>(m_values.get_depth()));

        m_sum -= static_cast<S>(m_values.get_oldest_value());
        m_values.add_value(val);

        return m_mean;
    }

    /** @brief Get the current mean value */
    T get_value() const { return m_mean; }

  private:
    /** @brief Values */
    circular_buffer<T, MAX_DEPTH> m_values;
    /** @brief Intermediate sum */
    S m_sum;
    /** @brief Mean value */
    T m_mean;
};

} // namespace ov

#endif // OV_MEAN_FILTER_H
