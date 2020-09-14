/*
 * @Author: Sky
 * @Date: 2020-09-08 14:52:59
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:24:50
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YUDP_SERVER_H__
#define __YLIB_NETWORK_YUDP_SERVER_H__



#include <iostream>
#include <cstring>
#include <vector>

#include "yabstractsocketserver.h"

namespace yLib{

    class yUdpServer : public yAbstractSocketServer
    {
    private:
        std::vector<struct sockaddr_in> client_socket_addr_vec; //ipv4
    public:
        yUdpServer(void);
        yUdpServer(uint64_t max_client_num_, int socket_flags_ = IPPROTO_UDP, bool is_block_ = true);
        ~yUdpServer();

        /**
         * @description: Check socket(), bind(), listen(), accept() and so on.
         * @param {type} 
         * @return {type} 
        */
        inline bool socket_is_ready(void);

        /**
         * @description: bind ip and port
         * @param {type} 
         * @return {type} 
         */
        int8_t bind(const std::string & ip_, int32_t port_);


        /**
         * @description: 
         * @param {type} 
         * client_ip_: The sendto client ip(binary value).
         * @return {type} 
         */ 
        int64_t sendto(const void * buffer_, uint64_t size_to_send_, uint64_t client_ip_ = 0, uint64_t client_port_ = 0, int flags_ = 0);

        /**
         * @description: 
         * @param {type} 
         * client_ip_: The current receive client ip(binary value).
         * @return {type} 
         */
        int64_t recvfrom(void * buffer_, uint64_t size_to_read_, uint64_t &client_ip_, uint64_t &client_port_, int flags_ = 0);
    };

}
#endif //__YLIB_NETWORK_YUDP_SERVER_H__