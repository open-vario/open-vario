/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_LOCK_GUARD_H
#define OV_LOCK_GUARD_H

namespace ov
{

/** @brief Lock guard for mutexes */
template <typename MutexType>
class lock_guard
{
  public:
    /** @brief Constructor */
    lock_guard(MutexType& mutex) : m_mutex(mutex) { m_mutex.lock(); }
    /** @brief Copy constructor */
    lock_guard(const lock_guard& copy) = delete;
    /** @brief Move constructor */
    lock_guard(lock_guard&& move) = delete;

    /** @brief Destructor */
    virtual ~lock_guard() { m_mutex.unlock(); }

    /** @brief Copy operator */
    lock_guard& operator=(lock_guard& copy) = delete;

  private:
    /** @brief Associated mutex */
    MutexType& m_mutex;
};

} // namespace ov

#endif // OV_LOCK_GUARD_H
