/*
 * @Author: Sky
 * @Date: 2020-09-09 13:42:25
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-09 15:28:24
 * @Description: 
 */
#include "yUdpServer.h"

#include <iostream>

int main(int argc, char * argv[]){

    yUdpServer udp_server;

    udp_server.bind("0.0.0.0", 12355);

    char read_buffer[100];
    ::memset(read_buffer, 0, 100);
    uint64_t client_ip;
    uint64_t client_port;
    udp_server.recvfrom(read_buffer, 100, client_ip, client_port);
    std::cout<<"udpserver recv: "<<read_buffer<<std::endl;
    std::string msg_base = "server: your ip " + yUdpServer::get_ip_str(client_ip) + " your port " + std::to_string(yUdpServer::get_port(client_port));
    std::cout<<"udpserver recv: "<<msg_base<<std::endl;
    udp_server.sendto(msg_base.c_str(), msg_base.length(), client_ip, client_port);

    udp_server.recvfrom(read_buffer, 100, client_ip, client_port);
    std::cout<<"udpserver recv: "<<read_buffer<<std::endl;
    msg_base = "server: your ip " + yUdpServer::get_ip_str(client_ip) + " your port " + std::to_string(yUdpServer::get_port(client_port));
    std::cout<<"udpserver recv: "<<msg_base<<std::endl;
    udp_server.sendto(msg_base.c_str(), msg_base.length(), client_ip, client_port);

    udp_server.recvfrom(read_buffer, 100, client_ip, client_port);
    std::cout<<"udpserver recv: "<<read_buffer<<std::endl;
    msg_base = "server: your ip " + yUdpServer::get_ip_str(client_ip) + " your port " + std::to_string(yUdpServer::get_port(client_port));
    std::cout<<"udpserver recv: "<<msg_base<<std::endl;
    udp_server.sendto(msg_base.c_str(), msg_base.length(), client_ip, client_port);
    return 0;
}