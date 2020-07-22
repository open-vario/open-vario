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

#ifndef MESSAGEQUEUEBASE_H
#define MESSAGEQUEUEBASE_H

#include "IMessageQueue.h"

#include "nano_os_api.h"

namespace open_vario
{


/** \brief Base class for message queue implementation for Nano-OS */
template <typename MsgType>
class MessageQueueBase : public IMessageQueue<MsgType>
{
    public:


        /** \brief Constructor */
        MessageQueueBase()
        : m_mailbox()
        {}

        /** \brief Get the mailbox */
        nano_os_mailbox_t& mailbox() { return m_mailbox; }


        /** \brief Post a message into the message queue */
        virtual bool post(const MsgType& msg, const bool from_isr)
        {
            nano_os_error_t err;

            if (from_isr)
            {
                err = NANO_OS_MAILBOX_PostFromIsr(&m_mailbox, &msg);
            }
            else
            {
                err = NANO_OS_MAILBOX_Post(&m_mailbox, &msg);
            }

            return (err == NOS_ERR_SUCCESS);
        }

        /** \brief Wait for a message in the message queue  */
        virtual bool wait(MsgType& msg, const uint32_t timeout)
        {
            const nano_os_error_t err = NANO_OS_MAILBOX_Wait(&m_mailbox, &msg, NANO_OS_MS_TO_TICKS(timeout));
            return (err == NOS_ERR_SUCCESS);
        }


    private:

        /** \brief Underlying Nano-OS mailbox */
        nano_os_mailbox_t m_mailbox;

};


}

#endif // MESSAGEQUEUEBASE_H
