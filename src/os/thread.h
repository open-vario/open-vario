/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_THREAD_H
#define OV_THREAD_H

#include "FreeRTOS.h"
#include "task.h"

#include "delegate.h"

namespace ov
{

/** @brief Thread function */
using thread_func = delegate<void, void*>;

/** @brief C++ wrapper for threads */
class base_thread
{
  public:
    /** @brief Default constructor */
    base_thread();
    /** @brief Constructor */
    base_thread(thread_func func, const char* name, uint32_t priority, void* param, uint8_t* stack, uint32_t stack_size);
    /** @brief Copy constructor */
    base_thread(const base_thread& copy) = delete;
    /** @brief Move constructor */
    base_thread(base_thread&& move) = delete;

    /** @brief Destructor */
    virtual ~base_thread();

    /** @brief Copy operator */
    base_thread& operator=(base_thread& copy) = delete;

    /** @brief Start the thread */
    bool start(thread_func func, const char* name, uint32_t priority, void* param, uint8_t* stack, uint32_t stack_size);

  private:
    /** @brief FreeRTOS thread data */
    StaticTask_t m_thread_data;
    /** @brief FreeRTOS thread handle */
    TaskHandle_t m_thread;
    /** @brief Delegate for the thread function */
    thread_func m_func;
    /** @brief Thread's parameter */
    void* m_param;

    /** @brief Start function for the thread */
    static void start_func(base_thread* thread);
};

/** @brief C++ wrapper for threads with stack static allocation */
template <uint32_t STACK_SIZE_IN_BYTES>
class thread : public base_thread
{
  public:
    /** @brief Default constructor */
    thread() : base_thread() { }
    /** @brief Constructor */
    thread(thread_func func, const char* name, uint32_t priority, void* param)
        : base_thread(func, name, priority, param, m_stack, sizeof(m_stack))
    {
    }

    /** @brief Copy constructor */
    thread(const thread& copy) = delete;
    /** @brief Move constructor */
    thread(thread&& move) = delete;

    /** @brief Destructor */
    virtual ~thread() = default;

    /** @brief Copy operator */
    thread& operator=(thread& copy) = delete;

    /** @brief Start the thread */
    bool start(thread_func func, const char* name, uint32_t priority, void* param)
    {
        return base_thread::start(func, name, priority, param, m_stack, sizeof(m_stack));
    }

    /** @brief Suspend current thread for the specified amount of milliseconds */
    static void sleep_for(uint32_t duration_ms);

  private:
    /** @brief Thread's stack */
    uint8_t m_stack[STACK_SIZE_IN_BYTES];
};

namespace this_thread
{
/** @brief Suspend current thread for the specified amount of milliseconds */
void sleep_for(uint32_t duration_ms);
} // namespace this_thread

} // namespace ov

#endif // OV_THREAD_H
