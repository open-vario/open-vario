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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "IUdpSocket.h"

#include <Windows.h>


namespace open_vario
{


/** \brief UDP socket implementation for Windows */
class UdpSocket : public IUdpSocket
{
    public:

        /** \brief Constructor */
        UdpSocket();
        

        /** \brief Open the socket */
        virtual bool open();

        /** \brief Close the socket */
        virtual bool close();

        /** \brief Assign a name to an unnamed socket and establish a connection to a socket. */
        virtual bool bindAddr(const IpEndPoint& local_address);

        /** \brief Receive a datagram and stores the source address. */
        virtual bool receiveFrom(void* data, const size_t size, size_t& received, IpEndPoint& distant_address, const uint32_t timeout);

        /** \brief Send data to a specified address. */
        virtual bool sendTo(const void* data, const size_t size, const IpEndPoint& distant_address);


    private:

        /** \brief Underlying socket */
        SOCKET m_socket;

};

}

#endif // UDPSOCKET_H
