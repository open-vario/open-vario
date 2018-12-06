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

#include <pthread.h>
#include <time.h>
#include <queue>

namespace open_vario
{


/** \brief Message queue implementation for Linux */
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
            pthread_mutex_init(&m_cs, NULL);
            pthread_cond_init(&m_cond, NULL);
        }


        /** \brief Post a message into the message queue */
        virtual bool post(const MsgType& msg, const bool from_isr)
        {
            (void)from_isr;
            
            bool ret = false;

            pthread_mutex_lock(&m_cs);
            if (m_queue.size() != m_max_msg_count)
            {
                m_queue.push(msg);
                pthread_cond_signal(&m_cond);
                ret = true;
            }
            pthread_mutex_unlock(&m_cs);

            return ret;
        }

        /** \brief Wait for a message in the message queue  */
        virtual bool wait(MsgType& msg, const uint32_t timeout)
        {
            bool ret = true;

            struct timespec abstimeout;
            clock_gettime(CLOCK_REALTIME, &abstimeout);
            abstimeout.tv_sec += static_cast<time_t>(timeout / 1000u);
            abstimeout.tv_nsec += static_cast<long>((timeout % 1000u) * 1000000u);
            abstimeout.tv_sec += abstimeout.tv_nsec / 1000000;
            abstimeout.tv_nsec = abstimeout.tv_nsec % 1000000;

            pthread_mutex_lock(&m_cs);
            if (m_queue.size() == 0)
            {
                const int success = (pthread_cond_timedwait(&m_cond, &m_cs, &abstimeout) == 0);
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

            pthread_mutex_unlock(&m_cs);

            return ret;
        }


    private:

        /** \brief Maximum message count */
        const uint32_t m_max_msg_count;

        /** \brief Underlying POSIC mutex */
        pthread_mutex_t m_cs;

        /** \brief Underlying Windows condition variable */
        pthread_cond_t m_cond;

        /** \brief Message queue */
        std::queue<MsgType> m_queue;
};


}

#endif // MESSAGEQUEUE_H
