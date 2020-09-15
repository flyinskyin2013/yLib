/*
 * @Author: Sky
 * @Date: 2020-09-14 11:14:59
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 17:34:30
 * @Description: 
 */

#include "yTcpServer.h"

#include <iostream>

bool g_is_tcpserver_continue = true;
int g_client_array[3] = {0, 0, 0};
int g_client_array_idx = 0;


void test_OnClientConnectCB(uint64_t client_fd_, const std::string &client_ip_, uint64_t client_port_){

    g_client_array[g_client_array_idx] = client_fd_;
    g_client_array_idx++;
    std::cout<<"client connect, ip "<< client_ip_<< ", port " <<client_port_<<std::endl;
}
void test_OnClientDisconnectCB(uint64_t client_fd_, const std::string &client_ip_, uint64_t client_port_){

    std::cout<<"client disconnect, ip "<< client_ip_<< ", port " <<client_port_<<std::endl;
}


int main(int argc, char * argv[]){

    yTcpServer tcp_server;

    char recv_msg_buff[100];
    uint64_t svr_ip;
    uint64_t svr_port;

    tcp_server.bind("127.0.0.1", 12356);

    tcp_server.start_epoll_thread(test_OnClientConnectCB, test_OnClientDisconnectCB);
    g_is_tcpserver_continue = true;
    while(g_is_tcpserver_continue){

        if (3 == g_client_array_idx){
            std::cout<<"Server read: ======="<<std::endl;
            ::memset(recv_msg_buff, 0, 100);
            tcp_server.read(recv_msg_buff, 100, g_client_array[0]);
            std::string _msg = "tcpserver recv: " + std::string(recv_msg_buff);
            std::cout << _msg << std::endl;
            tcp_server.write(_msg.c_str(), _msg.length(), g_client_array[0]);

            ::memset(recv_msg_buff, 0, 100);
            tcp_server.read(recv_msg_buff, 100, g_client_array[1]);
            _msg = "tcpserver recv: " + std::string(recv_msg_buff);
            std::cout << _msg << std::endl;
            tcp_server.write(_msg.c_str(), _msg.length(), g_client_array[1]);

            ::memset(recv_msg_buff, 0, 100);
            tcp_server.read(recv_msg_buff, 100, g_client_array[2]);
            _msg = "tcpserver recv: " + std::string(recv_msg_buff);
            std::cout << _msg << std::endl;
            tcp_server.write(_msg.c_str(), _msg.length(), g_client_array[2]);
            break;
        }
        else{

            sleep(1);
        }
    }

    return 0;
}