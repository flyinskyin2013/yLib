/*
 * @Author: Sky
 * @Date: 2020-09-14 11:14:49
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-14 16:53:21
 * @Description: 
 */

#include "yTcpSocket.h"

#include <iostream>

int main(int argc, char * argv[]){

    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction( SIGPIPE, &sa, 0);
    

    yTcpSocket tcp_client0;
    yTcpSocket tcp_client1;
    yTcpSocket tcp_client2(IPPROTO_TCP, false);

    char recv_msg_buff[100];
    uint64_t svr_ip;
    uint64_t svr_port;

    tcp_client0.bind("", 12345);
    tcp_client1.bind("", 12346);
    //tcp_client2 not bind, system automatically choose port.

    if (0 > tcp_client0.connect("127.0.0.1", 12356) ){

        std::cout<<"client0: connect failed."<<std::endl; 
    }
    if (0 > tcp_client1.connect("127.0.0.1", 12356)){

        std::cout<<"client1: connect failed."<<std::endl; 
    }
    if (0 > tcp_client2.connect("127.0.0.1", 12356)){

        std::cout<<"client2: connect failed."<<std::endl; 
    }

    std::string msg0 = "I am client0";
    tcp_client0.write(msg0.c_str(), msg0.length());

    std::string msg1 = "I am client1";
    tcp_client1.write(msg1.c_str(), msg1.length());

    std::string msg2 = "I am client2";
    tcp_client2.write(msg2.c_str(), msg2.length());

    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
    tcp_client2.read(recv_msg_buff, 100);
    std::cout<<"client2: recv: "<<recv_msg_buff<<std::endl; 


    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));

    tcp_client0.read(recv_msg_buff, 100);
    std::cout<<"client0: recv: "<<recv_msg_buff<<std::endl; 

    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
    tcp_client1.read(recv_msg_buff, 100);
    std::cout<<"client1: recv: "<<recv_msg_buff<<std::endl; 

    memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
    tcp_client2.read(recv_msg_buff, 100);
    std::cout<<"client2: recv: "<<recv_msg_buff<<std::endl; 
    return 0;
}