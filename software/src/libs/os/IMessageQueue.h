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

#ifndef IMESSAGEQUEUE_H
#define IMESSAGEQUEUE_H

#include <cstdint>
#include <cstddef>

namespace open_vario
{


/** \brief Interface for all message queues implementations */
template <typename MsgType>
class IMessageQueue
{
    public:

        /** \brief Post a message into the message queue */
        virtual bool post(const MsgType& msg, const bool from_isr) = 0;

        /** \brief Wait for a message in the message queue  */
        virtual bool wait(MsgType& msg, const uint32_t timeout) = 0;
};


}

#endif // IMESSAGEQUEUE_H
