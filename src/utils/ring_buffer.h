
#ifndef OV_RING_BUFFER_H
#define OV_RING_BUFFER_H

#include <cstdint>

namespace ov
{

/** @brief Ring buffer */
template <typename T, size_t SIZE>
class ring_buffer
{
  public:
    /** @brief Constructor */
    ring_buffer() : m_buffer(), m_read_index(0u), m_write_index(0u) { }

    /** @brief Copy constructor */
    ring_buffer(const ring_buffer& copy) = delete;
    /** @brief Move constructor */
    ring_buffer(ring_buffer&& move) = delete;
    /** @brief Copy operator */
    ring_buffer& operator=(ring_buffer& copy) = delete;

    /** @brief Clear the contents of the ring buffer */
    void clear()
    {
        m_read_index  = 0u;
        m_write_index = 0u;
    }

    /** @brief Indicate if the buffer is empty */
    bool is_empty() const { return (m_read_index == m_write_index); }

    /** @brief Read a value from the buffer */
    bool read(T& val)
    {
        bool ret = false;

        // Check if a value is available
        if (!is_empty())
        {
            // Read value
            val = m_buffer[m_read_index];

            // Compute next index
            m_read_index = get_next_index(m_read_index);

            ret = true;
        }

        return ret;
    }

    /** @brief Write a value in the buffer */
    bool write(const T& val)
    {
        bool ret = false;

        // Compute next index
        size_t next_index = get_next_index(m_write_index);

        // Add value to the buffer
        if (next_index != m_read_index)
        {
            m_buffer[m_write_index] = val;
            m_write_index           = next_index;
            ret                     = true;
        }

        return ret;
    }

  private:
    /** @brief Buffer */
    T m_buffer[SIZE];
    /** @brief Read index */
    size_t m_read_index;
    /** @brief Write index */
    size_t m_write_index;

    /** @brief Get the next index */
    uint32_t get_next_index(uint32_t index)
    {
        size_t next_index = index + 1u;
        if (next_index == SIZE)
        {
            next_index = 0u;
        }
        return next_index;
    }
};

} // namespace ov

#endif // OV_RING_BUFFER_H
