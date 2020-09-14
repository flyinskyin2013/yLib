/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:55
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:17:41
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YTCP_SOCKET_H__
#define __YLIB_NETWORK_YTCP_SOCKET_H__


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>

//for connect() select
#include <sys/select.h>
#include <sys/time.h>

#ifdef __cplusplus
}
#endif //__cplusplus

#include <iostream>
#include <cstring>

#include "yabstractsocket.h"


namespace yLib{

    class yTcpSocket : public yAbstractSocket
    {
    private:
        /* data */
        struct sockaddr_in client_socket_addr;//ipv4
    public:
        yTcpSocket(/* args */);
        yTcpSocket(int socket_flags_, bool is_block_ = true);
        ~yTcpSocket();


        /**
         * @description: Check socket(), bind(), listen(), accept() and so on.
         * @param {type} 
         * @return {type} 
        */
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
        int8_t connect(const std::string & ip_, int32_t port_);
        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         * =-1: a error occurred.
         * =-2: no data to read.The file descriptor fd refers to a socket and has been marked nonblocking (O_NONBLOCK)
         * = 0: server maybe close.
         * > 0: read n bytes.
         */
        int64_t read(void *buffer_, size_t count_);
        /**
         * @description: 
         * @param {type} 
         * @return {type}
         *  =-1: a error occurred.
         *  >=0: write n bytes.
         */
        int64_t write(const void *buffer_, size_t count_);
    };

}



#endif //__YLIB_NETWORK_YTCP_SOCKET_H__