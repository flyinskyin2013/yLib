/*
 * @Author: Sky
 * @Date: 2020-09-09 13:42:25
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-09 15:57:51
 * @Description: 
 */
#include "yUdpSocket.h"

#include <iostream>

int main(int argc, char * argv[]){

    yUdpSocket udp_client0;
    yUdpSocket udp_client1;
    yUdpSocket udp_client2;

    char recv_msg_buff[100];
    uint64_t svr_ip;
    uint64_t svr_port;

    udp_client0.bind("", 12345);
    udp_client1.bind("", 12346);
    //udp_client2 not bind, system automatically choose port.

    std::string msg0 = "I am client0";
    udp_client0.sendto(msg0.c_str(), msg0.length(), "127.0.0.1", 12355);

    std::string msg1 = "I am client1";
    udp_client1.sendto(msg1.c_str(), msg1.length(), "127.0.0.1", 12355);

    std::string msg2 = "I am client2";
    udp_client2.sendto(msg2.c_str(), msg2.length(), "127.0.0.1", 12355);


    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));

    udp_client0.recvfrom(recv_msg_buff, 100, svr_ip, svr_port);
    std::cout<<"client2: recv: "<<recv_msg_buff<< " ip is " <<yUdpSocket::get_ip_str(svr_ip)<< " port is "<<yUdpSocket::get_port(svr_port)<<std::endl; 

    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
    udp_client1.recvfrom(recv_msg_buff, 100, svr_ip, svr_port);
    std::cout<<"client2: recv: "<<recv_msg_buff<< " ip is " <<yUdpSocket::get_ip_str(svr_ip)<< " port is "<<yUdpSocket::get_port(svr_port)<<std::endl; 

    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
    udp_client2.recvfrom(recv_msg_buff, 100, svr_ip, svr_port);
    std::cout<<"client2: recv: "<<recv_msg_buff<< " ip is " <<yUdpSocket::get_ip_str(svr_ip)<< " port is "<<yUdpSocket::get_port(svr_port)<<std::endl; 
    return 0;
}