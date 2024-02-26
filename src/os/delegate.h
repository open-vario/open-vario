/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_DELEGATE_H
#define OV_DELEGATE_H

namespace ov
{

/** @brief Delegate to target a method of an object */
template <typename ReturnType, typename... Args>
class delegate
{
  public:
    /** @brief Create a delegate */
    template <typename ObjType, ReturnType (ObjType::*MethodType)(Args...)>
    static delegate create(ObjType& obj)
    {
        delegate<ReturnType, Args...> deleg(&obj, &call_method<ObjType, MethodType>);
        return deleg;
    }

    /** @brief Constructor */
    delegate() : m_obj(nullptr), m_method(nullptr) { }

    /** @brief Copy constructor */
    delegate(const delegate& copy) : m_obj(copy.m_obj), m_method(copy.m_method) { }

    /** @brief Move constructor */
    delegate(delegate&& move) : m_obj(move.m_obj), m_method(move.m_method) { move.reset(); }

    /** @brief Invoke the target function or method */
    ReturnType invoke(Args&&... a) const
    {
        if (is_null())
        {
            while (true)
                ;
        }

        return (*m_method)(m_obj, static_cast<Args&&>(a)...);
    }

    /** @brief Call operator to invoke the target function or method */
    ReturnType operator()(Args&&... a) const
    {
        if (is_null())
        {
            while (true)
                ;
        }

        return (*m_method)(m_obj, static_cast<Args&&>(a)...);
    }

    /** @brief Indicate if the delegate points to a valid target function or method */
    bool is_null() const { return ((m_obj == nullptr) || (m_method == nullptr)); }

    /** @brief Make the delegate invalid */
    void reset()
    {
        m_obj    = nullptr;
        m_method = nullptr;
    }

    /** @brief Comparison operator */
    bool operator==(const delegate<ReturnType, Args...>& compare_delegate) const
    {
        const bool ret = ((m_obj == compare_delegate.m_obj) && (m_method == compare_delegate.m_method));
        return ret;
    }

    /** @brief Comparison operator */
    bool operator!=(const delegate<ReturnType, Args...>& compare_delegate) const
    {
        const bool ret = !((m_obj == compare_delegate.m_obj) && (m_method == compare_delegate.m_method));
        return ret;
    }

    /** @brief Assignment operator */
    delegate<ReturnType, Args...>& operator=(const delegate<ReturnType, Args...>& copy)
    {
        m_obj    = copy.m_obj;
        m_method = copy.m_method;
        return (*this);
    }

  private:
    /** @brief Caller method type */
    typedef ReturnType (*fp_call_t)(void* const obj, Args...);

    /** @brief Object to call */
    void* m_obj;

    /** @brief Method to call */
    fp_call_t m_method;

    /** @brief Constructor */
    delegate(void* const obj, const fp_call_t method) : m_obj(obj), m_method(method) { }

    /** @brief Caller method */
    template <typename ObjType, ReturnType (ObjType::*MethodType)(Args...)>
    static ReturnType call_method(void* const obj, Args... params)
    {
        ObjType* const obj_call = static_cast<ObjType*>(obj);
        return (obj_call->*MethodType)(static_cast<Args&&>(params)...);
    }
};

} // namespace ov

#endif // OV_DELEGATE_H
