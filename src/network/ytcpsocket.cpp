/*
 * @Author: Sky
 * @Date: 2020-09-08 10:27:12
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:57:33
 * @Description: 
 */
#include "network/ytcpsocket.h"

#include "ylib.hpp"

using namespace yLib;

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yTcpSocket)

yTcpSocket::yTcpSocket(/* args */) noexcept
{

    clean_to_default();

    create_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

yTcpSocket::yTcpSocket(int domain, int type, int protocol) noexcept{

    clean_to_default();
    
    create_socket(domain, type, protocol);
}

yTcpSocket::~yTcpSocket() noexcept{}
