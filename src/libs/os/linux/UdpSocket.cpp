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

#include "UdpSocket.h"
#include "nano-stl-conf.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


/** \brief Invalid socket file descriptor */
#define INVALID_SOCKET -1

namespace open_vario
{


/** \brief Constructor */
UdpSocket::UdpSocket()
: m_socket(INVALID_SOCKET)
{}        

/** \brief Open the socket */
bool UdpSocket::open()
{
    bool ret = false;

    if (m_socket == INVALID_SOCKET)
    {
        m_socket = socket(static_cast<int>(AF_INET), static_cast<int>(SOCK_DGRAM), IPPROTO_UDP);
        if (m_socket != INVALID_SOCKET)
        {
            ret = true;
        }
    }

    return ret;
}

/** \brief Close the socket */
bool UdpSocket::close()
{
    bool ret = false;

    if (m_socket != INVALID_SOCKET)
    {
        int call_ret = ::close(m_socket);
        if (call_ret == 0)
        {
            m_socket = INVALID_SOCKET;
            ret = true;
        }
    }

    return ret;
}

/** \brief Assign a name to an unnamed socket and establish a connection to a socket. */
bool UdpSocket::bindAddr(const IpEndPoint& local_address)
{
    bool ret = false;

    if (m_socket != INVALID_SOCKET)
    {
        struct sockaddr_in addr = { 0 };
        addr.sin_family = PF_INET;
        addr.sin_port = htons(local_address.port);
        if (local_address.address[0] == '*')
        {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else
        {
            addr.sin_addr.s_addr = inet_addr(local_address.address);
        }
        const int call_ret = ::bind(m_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
        if (call_ret == 0)
        {
            ret = true;
        }
    }

    return ret;
}

/** \brief Receive a datagram and stores the source address. */
bool UdpSocket::receiveFrom(void* data, const size_t size, size_t& received, IpEndPoint& distant_address, const uint32_t timeout)
{
    bool ret = false;

    if ((m_socket != INVALID_SOCKET) &&
        (data != nullptr) &&
        (size != 0))
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(m_socket, &readfds);
        struct timeval tv;
        tv.tv_sec = timeout / 1000u;
        tv.tv_usec = (timeout - 1000u * tv.tv_sec) * 1000u;

        int call_ret = select(m_socket + 1, &readfds, nullptr, nullptr, &tv);
        if( call_ret > 0)
        {
            struct sockaddr_in remoteAddr;
            socklen_t addrLen = sizeof(remoteAddr);

            call_ret = ::recvfrom(m_socket, reinterpret_cast<char *>(data), static_cast<int>(size), 0, reinterpret_cast<struct sockaddr *>(&remoteAddr), &addrLen);
            if (call_ret > 0)
            {
                // Data has been received
                received = static_cast<uint32_t>(call_ret);
                distant_address.port = ntohs(remoteAddr.sin_port);
                (void)NANO_STL_STRNCPY(distant_address.address, inet_ntoa(remoteAddr.sin_addr), IpEndPoint::IP_ADDRESS_MAX_SIZE);
                ret = true;
            }
        }
    }

    return ret;
}

/** \brief Send data to a specified address. */
bool UdpSocket::sendTo(const void* data, const size_t size, const IpEndPoint& distant_address)
{
    bool ret = false;

    if ((m_socket != INVALID_SOCKET) &&
        (data != nullptr) &&
        (size != 0))
    {
        struct sockaddr_in remoteAddr;
        remoteAddr.sin_family = AF_INET;
        remoteAddr.sin_port = htons(distant_address.port);
        remoteAddr.sin_addr.s_addr = inet_addr(distant_address.address);

        int call_ret = ::sendto(m_socket, reinterpret_cast<const char *>(data), static_cast<int>(size), 0, reinterpret_cast<struct sockaddr *>(&remoteAddr), sizeof(struct sockaddr_in));
        if (call_ret == static_cast<int>(size))
        {
            // Data sent
            ret = true;
        }
    }

    return ret;
}


}
