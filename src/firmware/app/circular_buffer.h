
#ifndef OV_CIRCULAR_BUFFER_H
#define OV_CIRCULAR_BUFFER_H

#include <cstddef>

namespace ov
{

/** @brief Circular buffer with parametrizable depth */
template <typename T, size_t MAX_DEPTH>
class circular_buffer
{
  public:
    /** @brief Constructor */
    circular_buffer() : m_values{}, m_depth(MAX_DEPTH), m_oldest(1u), m_current(0u) { }

    /** @brief Set a new depth */
    bool set_depth(size_t new_depth)
    {
        bool ret = false;

        // Check new depth
        if ((new_depth > 1u) && (new_depth <= MAX_DEPTH))
        {
            // Save depth
            m_depth = new_depth;

            // Reset buffer
            m_oldest  = 1u;
            m_current = 0u;
        }

        return ret;
    }

    /** @brief Get the depth */
    size_t get_depth() const { return m_depth; }

    /** @brief Get the oldest value in the buffer */
    T get_oldest_value() const { return m_values[m_oldest]; }

    /** @brief Get the current value in the buffer */
    T get_value() const { return m_values[m_current]; }

    /** @brief Add value to the buffer */
    void add_value(const T& val)
    {
        // Compute mean
        m_values[m_current] = val;

        // Update indices
        m_current++;
        if (m_current == m_depth)
        {
            m_current = 0u;
        }
        m_oldest++;
        if (m_oldest == m_depth)
        {
            m_oldest = 0u;
        }
    }

    /** @brief Get the sum of the values contained in the buffer */
    T sum()
    {
        T s{};
        for (size_t i = 0; i < m_depth; i++)
        {
            s += m_values[i];
        }
        return s;
    }

  private:
    /** @brief Values */
    T m_values[MAX_DEPTH];
    /** @brief Depth */
    size_t m_depth;
    /** @brief Oldest value */
    size_t m_oldest;
    /** @brief Current value */
    size_t m_current;
};

} // namespace ov

#endif // OV_CIRCULAR_BUFFER_H
