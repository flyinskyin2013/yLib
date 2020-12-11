/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:39
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-11 13:39:41
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

    // In "in.h" file, Tere are two definations of noths/ntohl and htons/htonl .
    // If enable macro __OPTIMIZE__(have -Ox param for gcc ), noths/ntohl and htons/htonl are defined as macro.
    // If disable macro __OPTIMIZE__,(don't have -Ox param for gcc ) noths/ntohl and htons/htonl are defined as function.
    // so we can't call noths/ntohl and htons/htonl using '::', when we enable macro '__OPTIMIZE__'(have -Ox param for gcc )
    // return ::ntohs(port_);
    return ntohs(port_);
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

    // In "in.h" file, Tere are two definations of noths/ntohl and htons/htonl .
    // If enable macro __OPTIMIZE__(have -Ox param for gcc ), noths/ntohl and htons/htonl are defined as macro.
    // If disable macro __OPTIMIZE__,(don't have -Ox param for gcc ) noths/ntohl and htons/htonl are defined as function.
    // so we can't call noths/ntohl and htons/htonl using '::', when we enable macro '__OPTIMIZE__'(have -Ox param for gcc )
    // return ::ntohs(port_);
    return htons(port_);
}