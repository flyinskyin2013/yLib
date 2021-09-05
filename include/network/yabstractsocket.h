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
        virtual ~yAbstractSocket() noexcept;

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