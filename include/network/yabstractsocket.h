/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:28
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-16 16:38:21
 * @Description: 
 */

#ifndef __YLIB_NETWORK_YABSTRACT_SOCKET_H__
#define __YLIB_NETWORK_YABSTRACT_SOCKET_H__

#include <string>
#include <cstdint>

#include "core/yobject.hpp"

namespace yLib{
    
    class __YLIB_EXPORT__ yAbstractSocket MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:

    protected:
        /* data */
        yAbstractSocket(/* args */);

        static std::string get_ip_from_binary(uint64_t ip_);
        static uint64_t get_port_from_binary(uint64_t port_);

        static int64_t translate_ip_to_binary(const std::string &ip_);
        static uint64_t translate_port_to_binary(uint64_t port_);
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
         * http://isocpp.org/wiki/faq/inline-functions#inline-member-fns
         * Inline funciton defines must put in header-file.
         */
        
        inline bool socket_is_valid(void){return (0 > socket_fd)?false:true;}

        /**
         * @description: Check socket(), bind(), listen(), accept() and cd so on.
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