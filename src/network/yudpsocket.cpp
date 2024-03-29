/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Author: Sky
 * @Date: 2020-09-08 10:27:33
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:57:51
 * @Description: 
 */

#include "network/yudpsocket.h"

#include "ylib.hpp"

using namespace yLib;

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yUdpSocket)

yUdpSocket::yUdpSocket(int domain, int type, int protocol) noexcept
{

    clean_to_default();
    create_socket(domain, type, protocol);
}

yUdpSocket::yUdpSocket() noexcept{

    clean_to_default();

    create_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

yUdpSocket::~yUdpSocket() noexcept
{
    clean_to_default();
}

void yUdpSocket::clean_to_default(void) noexcept {

    is_sockfd_valid = false;

    // In udp socket, the bind() can be called or ben't called. it's flexible.
    // In yUdpSocket, we don't need to call connect
    is_bind_success = true;

    // In udp socket, the connect() can be called or ben't called. it's flexible.
    // In yUdpSocket, we don't need to call connect
    is_connect_success = true;

    socket_fd = -1; 

}

int64_t yUdpSocket::sendto(const void * buffer_, uint64_t size_to_send_, std::string ip_, uint64_t port_, int flags_) noexcept{

    if (!socket_is_ready()){

        LOGE("yUdpSocket")<<"yUdpSocket: socket is not ready. please check.";
        return -1;
    }

    struct sockaddr_in _send_addr;
    ::memset(&_send_addr, 0, sizeof(_send_addr));

    int64_t _binary_ip = 0;
    if (0 > (_binary_ip = translate_ip_to_binary(ip_))){//invalid ip

        LOGE("yUdpSocket")<<"yUdpSocket: Invalid ip_. please check.";
        return -1;
    }

    
    _send_addr.sin_addr.s_addr = _binary_ip;
    _send_addr.sin_family = AF_INET;
	_send_addr.sin_port = translate_port_to_binary(port_);

    ssize_t _complete_send_len = 0;
    if ( 0 > (_complete_send_len = ::sendto(socket_fd, buffer_, size_to_send_, 0, reinterpret_cast<const struct sockaddr *>(&_send_addr), sizeof(struct sockaddr_in))) ){

        if (errno == EAGAIN || errno == EWOULDBLOCK){
            /*
                EAGAIN or EWOULDBLOCK
                        The  socket  is  marked  nonblocking and the requested operation
                        would block.  POSIX.1-2001 allows either error  to  be  returned
                        for  this case, and does not require these constants to have the
                        same value, so a portable application should check for both pos‐
                        sibilities.

            */
            return 0;
        }        
        LOGE("yUdpSocket")<<"yUdpSocket: send data failed. errno is "<<errno;
        return -1;
    }

    return _complete_send_len;
}

int64_t yUdpSocket::recvfrom(void * buffer_, uint64_t size_to_read_,  std::string &client_ip_, uint64_t &client_port_, int flags_) noexcept{
    

    if (!socket_is_ready()){

        LOGE("yUdpSocket")<<"yUdpSocket: socket is not ready. please check.";
        return -1;
    }

    ssize_t _complete_read_len = 0;
    struct sockaddr_in _recv_addr;
    socklen_t _sockaddr_in_len = sizeof(_recv_addr);
    ::memset(&_recv_addr, 0, sizeof(_recv_addr));

    if ( 0 > (_complete_read_len = ::recvfrom(socket_fd, buffer_, size_to_read_, flags_, \
        reinterpret_cast<struct sockaddr *>(&_recv_addr), &_sockaddr_in_len)) ){

        if (errno == EAGAIN || errno == EWOULDBLOCK){
            /*
                EAGAIN or EWOULDBLOCK
                        The  socket  is  marked  nonblocking and the requested operation
                        would block.  POSIX.1-2001 allows either error  to  be  returned
                        for  this case, and does not require these constants to have the
                        same value, so a portable application should check for both pos‐
                        sibilities.

            */
            return 0;
        }       
        LOGE("yUdpSocket")<<"yUdpSocket: recvfrom failed. errno is "<< errno;
        return -1;
    }

    client_ip_ = get_ip_from_binary(_recv_addr.sin_addr.s_addr);
    client_port_ = get_port_from_binary(_recv_addr.sin_port);

    return _complete_read_len;
}
