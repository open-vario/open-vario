/*
Copyright(c) 2017 Cedric Jimenez

This file is part of Open-Vario.

Open-Vario is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Open-Vario is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Open-Vario.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "IMessageQueue.h"

#include <queue>

namespace open_vario
{


/** \brief Message queue implementation for Windows */
template <typename MsgType, uint32_t MAX_MSG_COUNT>
class MessageQueue : public IMessageQueue<MsgType>
{
    public:


        /** \brief Constructor */
        MessageQueue()
        : m_max_msg_count(MAX_MSG_COUNT)
        , m_cs()
        , m_cond()
        , m_queue()
        {
            InitializeCriticalSection(&m_cs);
            InitializeConditionVariable(&m_cond);
        }


        /** \brief Post a message into the message queue */
        virtual bool post(const MsgType& msg, const bool from_isr)
        {
            (void)from_isr;
            
            bool ret = false;

            EnterCriticalSection(&m_cs);
            if (m_queue.size() != m_max_msg_count)
            {
                m_queue.push(msg);
                WakeConditionVariable(&m_cond);
                ret = true;
            }
            LeaveCriticalSection(&m_cs);

            return ret;
        }

        /** \brief Wait for a message in the message queue  */
        virtual bool wait(MsgType& msg, const uint32_t timeout)
        {
            bool ret = true;

            EnterCriticalSection(&m_cs);
            if (m_queue.size() == 0)
            {
                const BOOL success = SleepConditionVariableCS(&m_cond, &m_cs, timeout);
                ret = (success != 0);
            }
            if (ret && (m_queue.size() != 0))
            {
                msg = m_queue.front();
                m_queue.pop();
            }
            else
            {
                ret = false;
            }

            LeaveCriticalSection(&m_cs);

            return ret;
        }


    private:

        /** \brief Maximum message count */
        const uint32_t m_max_msg_count;

        /** \brief Underlying Windows critical section */
        CRITICAL_SECTION m_cs;

        /** \brief Underlying Windows condition variable */
        CONDITION_VARIABLE m_cond;

        /** \brief Message queue */
        std::queue<MsgType> m_queue;
};


}

#endif // MESSAGEQUEUE_H
