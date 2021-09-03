/*
 * @Author: Sky
 * @Date: 2020-09-08 10:49:42
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:58:24
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YTCP_SERVER_H__
#define __YLIB_NETWORK_YTCP_SERVER_H__

#include "yabstractsocket.h"

#include <utility>
#include <string>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#include <cstring>

namespace yLib{

    typedef void (*OnClientConnectCB)(uint64_t client_fd, const std::string &client_ip, uint64_t client_port);
    typedef void (*OnClientDisconnectCB)(uint64_t client_fd, const std::string &client_ip, uint64_t client_port);

    class yTcpServer:public yAbstractSocket
    {
    private:
        std::unique_ptr<std::thread> epoll_thread_ptr;

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        void epoll_thread_context(OnClientConnectCB con_cb, OnClientDisconnectCB discon_cb) noexcept;
        bool epoll_thread_is_continue;

        bool is_listen_success = false;

        int max_listen_num = 0;


        /**
         * @description: yTcpServer don't manage the client
         * @param {type} 
         * @return {type} 
         * =-1: a error occurred.
         * =-2: no data to read.The file descriptor fd refers to a socket and has been marked nonblocking (O_NONBLOCK)
         * = 0: server maybe close.
         * > 0: read n bytes.
         */
        int64_t read(void *buffer, size_t count) const noexcept override{return -1;}

        /**
         * @description: yTcpServer don't manage the client
         * @param {type} 
         * @return {type}
         *  =-1: a error occurred.
         *  >=0: write n bytes.
         */
        int64_t write(const void *buffer, size_t count) const noexcept override{return -1;}  

    public:
        yTcpServer(void) noexcept;
        yTcpServer(int domain, int type, int protocol) noexcept;
        ~yTcpServer() noexcept;

        /**
         * @description: Tcpserver don't need to connect(). we set the is_connect_success to true.
         * @param {type} 
         * @return {type} 
         */
        int8_t connect(const std::string & ip, int32_t port, int domain = AF_INET) noexcept override;

        /**
         * @description: clean all flag to default
         */
        void clean_to_default(void) noexcept override;        

        /**
         * @description: Check socket()/bind()/listen() and so on.
         * @param {type} 
         * @return {type} 
         */
        inline bool socket_is_ready(void) const noexcept override;


        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int8_t start_epoll_thread(OnClientConnectCB con_cb, \
            OnClientDisconnectCB discon_cb, \
            uint64_t max_listen_num = 5) noexcept;

        YLIB_DECLARE_CLASSINFO_CONTENT(yTcpServer);
    };

}


#endif //__YLIB_NETWORK_YTCP_SERVER_H__