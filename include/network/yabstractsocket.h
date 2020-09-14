/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:28
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 17:47:58
 * @Description: 
 */

#ifndef __YLIB_NETWORK_YABSTRACT_SOCKET_H__
#define __YLIB_NETWORK_YABSTRACT_SOCKET_H__

#include <string>
#include <cstdint>

namespace yLib{
    
    class yAbstractSocket 
    {
    private:

    protected:
        /* data */
        yAbstractSocket(/* args */);
    public:
        ~yAbstractSocket();

        /**
         * @description: 
         * @param {type} 
         * @return {type} 
         */
        int64_t close(int64_t fd_);
        /**
         * @description: Check socket()
         * @param {type} 
         * @return {type} 
         */
        
        inline bool socket_is_valid(void);

        static std::string get_ip_str(uint64_t ip_);
        static uint64_t get_port(uint64_t port_);

        /**
         * @description: Check socket(), bind(), listen(), accept() and so on.
         * @param {type} 
         * @return {type} 
         */
        virtual inline bool socket_is_ready(void) = 0;

        /**
         * @description: tcp server & udp server
         * @param {type} 
         * @return {type} 
         */
        virtual int8_t bind(const std::string & ip_, int32_t port_) = 0;

        
    protected:
        int64_t socket_fd;
        bool is_bind_success;
    };

}

#endif //__YLIB_NETWORK_YABSTRACT_SOCKET_H__