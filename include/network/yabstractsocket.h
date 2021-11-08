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
 * @Date: 2020-09-08 10:26:28
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:56:40
 * @Description: 
 */

#ifndef __YLIB_NETWORK_YABSTRACT_SOCKET_H__
#define __YLIB_NETWORK_YABSTRACT_SOCKET_H__

#include <string>
#include <cstdint>

#include "core/yobject.hpp"


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <sys/socket.h>
#include <sys/types.h>          /* See NOTES */
#include <netinet/in.h>
#include <errno.h>

#ifdef __cplusplus
}
#endif //__cplusplus

namespace yLib{
    
    class __YLIB_CLASS_DECLSPEC__ yAbstractSocket:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
    protected:
        static std::string get_ip_from_binary(uint64_t ip) noexcept;
        static uint64_t get_port_from_binary(uint64_t port) noexcept;

        static int64_t translate_ip_to_binary(const std::string &ip) noexcept;
        static uint64_t translate_port_to_binary(uint64_t port) noexcept;

        void create_socket(int domain, int type, int protocol) noexcept;
    public:

        yAbstractSocket() noexcept;

        ~yAbstractSocket() noexcept;

        // notice: can't give a pointer obj-sub-class of yAbstractSocket to pointer yAbstractSocket
        // Deleting an object through pointer to base invokes undefined behavior unless the destructor in the base class is virtual
        // virtual ~yAbstractSocket() noexcept;

        /**
         * @description: Check socket()/bind()/connect() and so on.
         * @param {type} 
         * @return {type} 
         */
        virtual inline bool socket_is_ready(void) const noexcept;

        /**
         * @description: tcp server & udp server
         * @param {type} 
         * @return {type} 
         */
        virtual int8_t bind(const std::string & ip, int32_t port, int domain = AF_INET) noexcept;

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         * =-1: a error occurred.
         * =-2: no data to read.The file descriptor fd refers to a socket and has been marked nonblocking (O_NONBLOCK)
         * = 0: server maybe close.
         * > 0: read n bytes.
         */
        virtual int64_t read(void *buffer, size_t count) const noexcept;

        /**
         * @description: 
         * @param {type} 
         * @return {type}
         *  =-1: a error occurred.
         *  >=0: write n bytes.
         */
        virtual int64_t write(const void *buffer, size_t count) const noexcept;     

        /**
         * @description: clean all flag to default
         */
        virtual void clean_to_default(void) noexcept;

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        virtual int8_t connect(const std::string & ip, int32_t port, int domain = AF_INET) noexcept;

    protected:
        int64_t socket_fd = -1;
        bool is_bind_success = false;
        bool is_sockfd_valid = false;
        bool is_connect_success = false;

        struct sockaddr_in local_socket_addr;//ipv4


        YLIB_DECLARE_CLASSINFO_CONTENT(yAbstractSocket);
    };

}

#endif //__YLIB_NETWORK_YABSTRACT_SOCKET_H__