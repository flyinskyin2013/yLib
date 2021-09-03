/*
 * @Author: Sky
 * @Date: 2020-09-08 10:26:55
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:58:32
 * @Description: 
 */
#ifndef __YLIB_NETWORK_YTCP_SOCKET_H__
#define __YLIB_NETWORK_YTCP_SOCKET_H__


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>

//for connect() select
#include <sys/select.h>
#include <sys/time.h>

#ifdef __cplusplus
}
#endif //__cplusplus

#include <iostream>
#include <cstring>

#include "yabstractsocket.h"


namespace yLib{

    class yTcpSocket : public yAbstractSocket
    {
    public:
        yTcpSocket(/* args */) noexcept;
        yTcpSocket(int domain, int type, int protocol) noexcept;
        ~yTcpSocket() noexcept;

        YLIB_DECLARE_CLASSINFO_CONTENT(yTcpSocket);
    };

}



#endif //__YLIB_NETWORK_YTCP_SOCKET_H__