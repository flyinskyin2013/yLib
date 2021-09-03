/*
 * @Author: Sky
 * @Date: 2020-09-14 11:14:49
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 13:59:36
 * @Description: 
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yTcpSocket)

using namespace yLib;
TEST_CASE( "Test yTcpServer apis" , "[yTcpServer_Apis]" ){

    SECTION("yTcpServer test") {

        struct sigaction sa;
        sa.sa_handler = SIG_IGN;
        sigaction( SIGPIPE, &sa, 0);
        

        yLib::yTcpSocket tcp_client0;
        yTcpSocket tcp_client1;
        yTcpSocket tcp_client2(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        char recv_msg_buff[100];
        uint64_t svr_ip;
        uint64_t svr_port;

        REQUIRE(0 == tcp_client0.bind("", 12345));
        REQUIRE(0 == tcp_client1.bind("", 12346));
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
        REQUIRE(msg0.length() == tcp_client0.write(msg0.c_str(), msg0.length()));

        std::string msg1 = "I am client1";
        REQUIRE(msg1.length() == tcp_client1.write(msg1.c_str(), msg1.length()));

        std::string msg2 = "I am client2";
        REQUIRE(msg2.length() == tcp_client2.write(msg2.c_str(), msg2.length()));


        ::memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        
        std::string _svr_write_msg;
//===================================================================

        memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        _svr_write_msg = "tcpserver recv: " + std::string("I am client0");

        REQUIRE(_svr_write_msg.length() == tcp_client0.read(recv_msg_buff, 100));
        REQUIRE_THAT( _svr_write_msg, Catch::Equals ( recv_msg_buff ));

        yLib::yLog::I("client0: recv: %s", recv_msg_buff); 
//===================================================================
        memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        _svr_write_msg = "tcpserver recv: " + std::string("I am client1");

        REQUIRE(_svr_write_msg.length() == tcp_client1.read(recv_msg_buff, 100));
        REQUIRE_THAT( _svr_write_msg, Catch::Equals ( recv_msg_buff ));
        
        yLib::yLog::I("client1: recv: %s", recv_msg_buff); 
//===================================================================
        memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        _svr_write_msg = "tcpserver recv: " + std::string("I am client2");

        REQUIRE(_svr_write_msg.length() == tcp_client2.read(recv_msg_buff, 100));
        REQUIRE_THAT( _svr_write_msg, Catch::Equals ( recv_msg_buff ));

        yLib::yLog::I("client2: recv: %s", recv_msg_buff);      
    }
    
    
}
