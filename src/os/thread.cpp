
#include "thread.h"

namespace ov
{
/** @brief Default constructor */
base_thread::base_thread() : m_thread_data(), m_thread(nullptr) { }

/** @brief Constructor */
base_thread::base_thread(thread_func func, const char* name, uint32_t priority, void* param, uint8_t* stack, uint32_t stack_size)
    : m_thread_data(), m_thread(nullptr), m_func(), m_param(nullptr)
{
    if (!start(func, name, priority, param, stack, stack_size))
    {
        while (true)
            ;
    }
}

/** @brief Destructor */
base_thread::~base_thread()
{
    if (m_thread)
    {
        vTaskDelete(m_thread);
    }
}

/** @brief Start the thread */
bool base_thread::start(thread_func func, const char* name, uint32_t priority, void* param, uint8_t* stack, uint32_t stack_size)
{
    bool ret = false;

    if (!m_thread)
    {
        m_func   = func;
        m_param  = param;
        m_thread = xTaskCreateStatic(reinterpret_cast<TaskFunction_t>(&base_thread::start_func),
                                     name,
                                     stack_size / sizeof(StackType_t),
                                     this,
                                     priority,
                                     reinterpret_cast<StackType_t*>(stack),
                                     &m_thread_data);
        if (m_thread)
        {
            ret = true;
        }
    }

    return ret;
}

/** @brief Start function for the thread */
void base_thread::start_func(base_thread* thread)
{
    if (thread)
    {
        thread->m_func.invoke(static_cast<void*&&>(thread->m_param));
    }
}

namespace this_thread
{

/** @brief Suspend current thread for the specified amount of milliseconds */
void sleep_for(uint32_t duration_ms)
{
    vTaskDelay(duration_ms / portTICK_PERIOD_MS);
}

} // namespace this_thread

} // namespace ov
