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
 * @Date: 2020-09-08 10:27:22
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:58:48
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YUDP_SOCKET_H__
#define __YLIB_NETWORK_YUDP_SOCKET_H__


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>

#ifdef __cplusplus
}
#endif //__cplusplus

#include <iostream>
#include <cstring>


#include "yabstractsocket.h"


namespace yLib{


    typedef void (*RecvFromCB)(uint64_t ip_, uint64_t port_);

    class yUdpSocket : public yAbstractSocket
    {
    public:
        yUdpSocket(void) noexcept;
        yUdpSocket(int domain, int type, int protocol) noexcept;
        ~yUdpSocket() noexcept;

        /**
         * @description: clean all flag to default
         */
        void clean_to_default(void) noexcept override;   

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int64_t sendto(const void * buffer, uint64_t size_to_send, std::string ip, uint64_t port,int flags = 0) noexcept;

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int64_t recvfrom(void * buffer, uint64_t size_to_read, std::string &client_ip, uint64_t &client_port, int flags = 0) noexcept;

        YLIB_DECLARE_CLASSINFO_CONTENT(yUdpSocket);
    };


}




#endif //__YLIB_NETWORK_YUDP_SOCKET_H__