/*
 * @Author: Sky
 * @Date: 2020-09-08 16:00:09
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 18:16:47
 * @Description: 
 */
#ifndef __YLIB_NETWORK_Y_ABSTRACT_SOCKET_SERVER_H__
#define __YLIB_NETWORK_Y_ABSTRACT_SOCKET_SERVER_H__

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <netinet/in.h> 

#ifdef __cplusplus
}
#endif //__cplusplus

#include "yabstractsocket.h"

namespace yLib{

    class yAbstractSocketServer:public yAbstractSocket
    {
    private:
        /* data */
    public:
        yAbstractSocketServer(/* args */);
        ~yAbstractSocketServer();

    protected:
        uint64_t max_client_num;
        struct sockaddr_in server_socket_addr; //ipv4

    };

}

#endif //__YLIB_NETWORK_Y_ABSTRACT_SOCKET_SERVER_H__