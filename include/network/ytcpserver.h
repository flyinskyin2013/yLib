/*
 * @Author: Sky
 * @Date: 2020-09-08 10:49:42
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:22:00
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YTCP_SERVER_H__
#define __YLIB_NETWORK_YTCP_SERVER_H__

#include "yabstractsocketserver.h"

#include <utility>
#include <string>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include <cstring>

namespace yLib{

    typedef void (*OnClientConnectCB)(uint64_t client_fd_, const std::string &client_ip_, uint64_t client_port_);
    typedef void (*OnClientDisconnectCB)(uint64_t client_fd_, const std::string &client_ip_, uint64_t client_port_);

    class yTcpServer:public yAbstractSocketServer
    {
    private:
        /* data */
        typedef std::unordered_map<uint64_t, struct sockaddr_in> yTcpServerSocketMap;
        std::unordered_map<uint64_t, struct sockaddr_in> client_info_map;
        std::unique_ptr<std::thread> epoll_thread_ptr;

        std::vector<uint32_t> used_client_id_bitmap;
        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int64_t get_available_client_id(void);
        
        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        bool set_available_client_id(uint64_t client_id_);

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        void epoll_thread_context(OnClientConnectCB con_cb_, OnClientDisconnectCB discon_cb_);
        bool epoll_thread_is_continue;

    public:
        yTcpServer(void);
        yTcpServer(uint64_t max_client_num_, int socket_flags_ = IPPROTO_TCP, bool is_block_ = true);
        ~yTcpServer();

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
         * @return {type} 
         */
        int8_t start_epoll_thread(OnClientConnectCB con_cb_, \
            OnClientDisconnectCB discon_cb_, \
            uint64_t max_listen_num = 5);


        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         * =-1: a error occurred.
         * =-2: no data to read.The file descriptor fd refers to a socket and has been marked nonblocking (O_NONBLOCK)
         * = 0: client maybe close.
         * > 0: read n bytes.
         */
        int64_t read(void *buffer_, size_t count_, uint64_t client_fd_ = 0); 
        
        
        /**
         * @description: 
         * @param {type} 
         * @return {type}
         *  =-1: a error occurred.
         *  >=0: write n bytes.
         */
        int64_t write(const void *buffer_, size_t count_, uint64_t client_fd_ = 0);
    };

}


#endif //__YLIB_NETWORK_YTCP_SERVER_H__