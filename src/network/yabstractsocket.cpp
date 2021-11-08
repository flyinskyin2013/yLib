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
 * @Date: 2020-09-08 10:26:39
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:57:09
 * @Description: 
 */
#include "network/yabstractsocket.h"



#include "ylib.hpp"


YLIB_IMPLEMENT_CLASSINFO_CONTENT(yAbstractSocket)


yLib::yAbstractSocket::yAbstractSocket(/* args */) noexcept
{
    clean_to_default();
}

yLib::yAbstractSocket::~yAbstractSocket() noexcept
{
    if (is_sockfd_valid){
        
        ::close(socket_fd);
    }

    clean_to_default();
}


inline bool yLib::yAbstractSocket::socket_is_ready(void) const noexcept{

    return (is_sockfd_valid && is_bind_success && is_connect_success);
}


std::string yLib::yAbstractSocket::get_ip_from_binary(uint64_t ip) noexcept{
    
    struct in_addr _tmp_in_addr;

    _tmp_in_addr.s_addr = ip;
    return ::inet_ntoa(_tmp_in_addr);
}


uint64_t yLib::yAbstractSocket::get_port_from_binary(uint64_t port) noexcept{

    // In "in.h" file, Tere are two definations of noths/ntohl and htons/htonl .
    // If enable macro __OPTIMIZE__(have -Ox param for gcc ), noths/ntohl and htons/htonl are defined as macro.
    // If disable macro __OPTIMIZE__,(don't have -Ox param for gcc ) noths/ntohl and htons/htonl are defined as function.
    // so we can't call noths/ntohl and htons/htonl using '::', when we enable macro '__OPTIMIZE__'(have -Ox param for gcc )
    // return ::ntohs(port_);
    return ntohs(port);
}

int64_t yLib::yAbstractSocket::translate_ip_to_binary(const std::string &ip) noexcept{

    struct in_addr _sin_addr;
    if (0 == ::inet_aton(ip.c_str(), &_sin_addr)){//invalid ip

        yLib::yLog::E("yAbstractSocket: Invalid ip = %s. please check.", ip.c_str());
        return -1;
    }

    return _sin_addr.s_addr;
}

uint64_t yLib::yAbstractSocket::translate_port_to_binary(uint64_t port) noexcept{

    // In "in.h" file, Tere are two definations of noths/ntohl and htons/htonl .
    // If enable macro __OPTIMIZE__(have -Ox param for gcc ), noths/ntohl and htons/htonl are defined as macro.
    // If disable macro __OPTIMIZE__,(don't have -Ox param for gcc ) noths/ntohl and htons/htonl are defined as function.
    // so we can't call noths/ntohl and htons/htonl using '::', when we enable macro '__OPTIMIZE__'(have -Ox param for gcc )
    // return ::ntohs(port_);
    return htons(port);
}


int8_t yLib::yAbstractSocket::bind(const std::string & ip, int32_t port, int domain) noexcept{

    if (!is_sockfd_valid){

        yLib::yLog::E("yAbstractSocket: socket is invalid. please check.");
        return -1;
    }

	::memset(static_cast<void *>(&local_socket_addr), 0, sizeof(local_socket_addr));

    if (ip != "")
        local_socket_addr.sin_addr.s_addr = translate_ip_to_binary(ip);
    else
        local_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        
	local_socket_addr.sin_family = domain;
	local_socket_addr.sin_port = translate_port_to_binary(port);

    if (0 > ::bind(socket_fd, reinterpret_cast<const struct sockaddr *>(&local_socket_addr), sizeof(local_socket_addr)) ){

        is_bind_success = false;
        yLib::yLog::E("yAbstractSocket socket bind failed. errno is %d", errno);
        return -1;
    }

    is_bind_success = true;

    return 0;
}


/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yLib::yAbstractSocket::read(void *buffer, size_t count) const noexcept{

    ssize_t _read_len = 0;

    if ( 0 > (_read_len = ::read(socket_fd, buffer, count))){

        if (errno == EAGAIN || errno == EWOULDBLOCK){

            return -2;//no data to read
        }

        yLib::yLog::E("yAbstractSocket: read data failed. errno is %d", errno);
        return -1;
    }

    return _read_len;
}

/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yLib::yAbstractSocket::write(const void *buffer, size_t count) const noexcept{

    ssize_t _write_len = 0;

    if ( 0 > (_write_len = ::write(socket_fd, buffer, count))){

        if (errno == EAGAIN || errno == EWOULDBLOCK){

            return 0;//no data to write
        }

        yLib::yLog::E("yAbstractSocket: write data failed. errno is %d", errno);
        return -1;
    }

    return _write_len;
}

void yLib::yAbstractSocket::clean_to_default(void) noexcept{

    is_sockfd_valid = false;
    is_bind_success = false;
    is_connect_success = false;
    socket_fd = -1;
}


/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int8_t yLib::yAbstractSocket::connect(const std::string & ip, int32_t port, int domain) noexcept{
    
    struct sockaddr_in _server_socket_addr;
    socklen_t _server_socket_addr_len = sizeof(_server_socket_addr);
    ::memset(&_server_socket_addr, 0, sizeof(_server_socket_addr));

    // In "in.h" file, Tere are two definations of noths/ntohl and htons/htonl .
    // If enable macro __OPTIMIZE__(have -Ox param for gcc ), noths/ntohl and htons/htonl are defined as macro.
    // If disable macro __OPTIMIZE__,(don't have -Ox param for gcc ) noths/ntohl and htons/htonl are defined as function.
    // so we can't call noths/ntohl and htons/htonl using '::', when we enable macro '__OPTIMIZE__'(have -Ox param for gcc )
    // call ::ntohs(port_);
    _server_socket_addr.sin_port = translate_port_to_binary(port);
    _server_socket_addr.sin_addr.s_addr = translate_ip_to_binary(ip);
    _server_socket_addr.sin_family = domain;

    if ( 0 > ::connect(socket_fd, reinterpret_cast<const sockaddr *>(&_server_socket_addr), _server_socket_addr_len)){

        if (EINPROGRESS == errno){//

            /*
            EINPROGRESS
              The socket is nonblocking and the connection cannot be completed
              immediately.  It is possible to select(2) or poll(2) for comple‐
              tion by selecting the socket for writing.  After select(2) indi‐
              cates writability, use getsockopt(2) to read the SO_ERROR option
              at  level  SOL_SOCKET  to  determine whether connect() completed
              successfully (SO_ERROR is zero) or unsuccessfully  (SO_ERROR  is
              one  of the usual error codes listed here, explaining the reason
              for the failure).
            */
           while(1){

                fd_set _client_fd_set;
                FD_ZERO(&_client_fd_set);
                FD_SET(socket_fd, &_client_fd_set);
                struct timeval _wait_time;
                _wait_time.tv_usec = 1000*1000; //wait for 1s
                int _select_ret = select(socket_fd + 1, NULL, &_client_fd_set, NULL, &_wait_time);
                if (_select_ret > 0){
                    
                    int _opt_val = 0;
                    socklen_t _opt_len = sizeof(_opt_val);

                    if ( 0 > getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &_opt_val, &_opt_len)){

                        yLib::yLog::E("yAbstractSocket: connect failed. In EINPROGRESS state. getsockopt error, errno is %d.", errno);
                    }

                    if (0 == _opt_val){//determine whether connect() completed successfully (SO_ERROR is zero)
                     
                        return 0;
                    }
                    else
                    {
                        yLib::yLog::E("yAbstractSocket: connect failed. In EINPROGRESS state. in select/getsockopt: socket is not ready.");
                        break;
                    }
                    

                }
                else{//timeout 0 ==_select_ret; occur error 0 > _select_ret

                    yLib::yLog::E("yAbstractSocket: connect failed. In EINPROGRESS state. select timeout or error.");
                    if (0 > _select_ret){

                        yLib::yLog::E("yAbstractSocket: connect failed. In EINPROGRESS state. select error, errno is %d.", errno);
                    }

                    break;
                }
           }

        }

        yLib::yLog::E("yAbstractSocket: connect failed. errno is %d", errno);
        return -1;
    }

    is_connect_success = true;
    return 0;
}

void yLib::yAbstractSocket::create_socket(int domain, int type, int protocol) noexcept{

    socket_fd = -1;
    
    if ( 0 > (socket_fd = ::socket(domain, type, protocol)) ){

        yLib::yLog::E("yAbstractSocket create socket failed. errno is %d\n", errno);
        is_sockfd_valid = false;

        return;
    }

    is_sockfd_valid = true;
}

