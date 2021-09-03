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