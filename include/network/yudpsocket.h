/*
 * @Author: Sky
 * @Date: 2020-09-08 10:27:22
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:17:59
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
    private:
        /* data */
        struct sockaddr_in client_socket_addr;//ipv4

    private:


    public:
        yUdpSocket(void);
        yUdpSocket(int socket_flags_, bool is_block_ = true);
        ~yUdpSocket();

        inline bool socket_is_ready(void);

        /**
         * @description: bind a port for send
         * @param {type} 
         * @return {type} 
         */
        int8_t bind(const std::string & ip_, int32_t port_);

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int64_t sendto(const void * buffer_, uint64_t size_to_send_, std::string ip_, uint64_t port_,int flags_ = 0);

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int64_t recvfrom(void * buffer_, uint64_t size_to_read_, uint64_t &client_ip_, uint64_t &client_port_, int flags_ = 0);
    };


}




#endif //__YLIB_NETWORK_YUDP_SOCKET_H__