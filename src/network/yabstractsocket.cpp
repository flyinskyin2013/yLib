/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:39
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-16 16:33:05
 * @Description: 
 */
#include "network/yabstractsocket.h"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <unistd.h>
#include <arpa/inet.h>


#ifdef __cplusplus
}
#endif //__cplusplus

#include "ylib.hpp"


yLib::yAbstractSocket::yAbstractSocket(/* args */)
{
}

yLib::yAbstractSocket::~yAbstractSocket()
{
}
/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yLib::yAbstractSocket::close(int64_t fd_){

    if (socket_is_valid()){

        return ::close(fd_);
    }

    return -1;
}

std::string yLib::yAbstractSocket::get_ip_from_binary(uint64_t ip_){
    
    struct in_addr _tmp_in_addr;

    _tmp_in_addr.s_addr = ip_;
    return ::inet_ntoa(_tmp_in_addr);
}


uint64_t yLib::yAbstractSocket::get_port_from_binary(uint64_t port_){

    return ::ntohs(port_);
}

int64_t yLib::yAbstractSocket::translate_ip_to_binary(const std::string &ip_){

    struct in_addr _sin_addr;
    if (0 == ::inet_aton(ip_.c_str(), &_sin_addr)){//invalid ip

        yLib::yLog::I("yAbstractSocket: Invalid ip_. please check.");
        return -1;
    }

    return _sin_addr.s_addr;
}

uint64_t yLib::yAbstractSocket::translate_port_to_binary(uint64_t port_){

    return ::htons(port_);
}