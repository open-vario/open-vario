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

#include "MessageQueueBase.h"


namespace open_vario
{


/** \brief Message queue implementation for Nano-OS */
template <typename MsgType, uint32_t MAX_MSG_COUNT>
class MessageQueue : public MessageQueueBase<MsgType>
{
    public:


        /** \brief Constructor */
        MessageQueue()
        : MessageQueueBase<MsgType>()
        {
            NANO_OS_MAILBOX_Create(&MessageQueueBase<MsgType>::mailbox(), MAX_MSG_COUNT, sizeof(MsgType), m_mailbox_buffer, sizeof(m_mailbox_buffer), QT_PRIORITY);
        }


    private:

        /** \brief Buffer for the mailbox */
        uint8_t m_mailbox_buffer[NANO_OS_MAILBOX_SIZE(sizeof(MsgType), MAX_MSG_COUNT)];
};


}

#endif // MESSAGEQUEUE_H
