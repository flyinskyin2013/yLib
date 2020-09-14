/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:39
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:15:15
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


using namespace yLib;

yAbstractSocket::yAbstractSocket(/* args */)
{
}

yAbstractSocket::~yAbstractSocket()
{
}
/**
 * @description: 
 * @param {type} 
 * @return {type} 
 */
int64_t yAbstractSocket::close(int64_t fd_){

    if (socket_is_valid()){

        return ::close(fd_);
    }

    return -1;
}
/**
 * @description: Check socket()
 * @param {type} 
 * @return {type} 
 */
inline bool yAbstractSocket::socket_is_valid(void){

    return (0 > socket_fd)?false:true;
}

std::string yAbstractSocket::get_ip_str(uint64_t ip_){
    
    struct in_addr _tmp_in_addr;

    _tmp_in_addr.s_addr = ip_;
    return ::inet_ntoa(_tmp_in_addr);
}


uint64_t yAbstractSocket::get_port(uint64_t port_){

    return ::ntohs(port_);
}