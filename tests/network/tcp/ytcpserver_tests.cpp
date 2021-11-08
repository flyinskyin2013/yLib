/*
 * @Author: Sky
 * @Date: 2020-09-14 11:14:59
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-09 17:07:31
 * @Description: 
 */

#include "catch2/catch.hpp"
#define YLIB_ENABLE_UNIT_TEST
#include "ylib.hpp"

#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yTcpServer)

using namespace yLib;


bool g_is_tcpserver_continue = true;
int g_client_array[3] = {0, 0, 0};
int g_client_port_array[3] = {0, 0, 0};
std::string g_client_ip_array[3] = {"", "", ""};
int g_client_array_idx = 0;


void test_OnClientConnectCB(uint64_t client_fd_, const std::string &client_ip_, uint64_t client_port_){

    g_client_array[g_client_array_idx] = client_fd_;
    g_client_port_array[g_client_array_idx] = client_port_;
    g_client_ip_array[g_client_array_idx] = client_ip_;
    
    g_client_array_idx++;
    std::cout<<"client connect, ip "<< client_ip_<< ", port " <<client_port_<<std::endl;
}
void test_OnClientDisconnectCB(uint64_t client_fd_, const std::string &client_ip_, uint64_t client_port_){

    std::cout<<"client disconnect, ip "<< client_ip_<< ", port " <<client_port_<<std::endl;
}



TEST_CASE( "Test yTcpServer apis" , "[yTcpServer_Apis]" ){

    SECTION("yTcpServer test") {

        yTcpServer tcp_server;

        REQUIRE(-1 == tcp_server.read(nullptr, 0));
        REQUIRE(-1 == tcp_server.write(nullptr, 0));

        // check yTcpServer(xxx,xxx,xxx)
        yTcpServer tcp_server1(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        // check yTcpServer(xxx,xxx,xxx) fail-mode
        yTcpServer tcp_server2(1, 2, 1999);

        char recv_msg_buff[100];
        uint64_t svr_ip;
        uint64_t svr_port;

        REQUIRE(0 == tcp_server.bind("127.0.0.1", 12356));

        REQUIRE(0 == tcp_server.start_epoll_thread(test_OnClientConnectCB, test_OnClientDisconnectCB));

        // check connect do nothing
        REQUIRE(tcp_server.connect("", 123) == 0);

        // check tcpserver is ready
        REQUIRE(tcp_server.socket_is_ready() == true);

        g_is_tcpserver_continue = true;
        while(g_is_tcpserver_continue){

            if (3 == g_client_array_idx){

                //================================check ip and port ===============
                REQUIRE(12345 == g_client_port_array[0]);
                REQUIRE_THAT( "127.0.0.1", Catch::Equals ( g_client_ip_array[0] ));

                REQUIRE(12346 == g_client_port_array[1]);
                REQUIRE_THAT( "127.0.0.1", Catch::Equals ( g_client_ip_array[1] ));

                //REQUIRE(12345 == g_client_port_array[0]);//we can't verify client2's port . It's random.
                REQUIRE_THAT( "127.0.0.1", Catch::Equals ( g_client_ip_array[2] ));


                //=================================================================
                std::cout<<"Server read: ======="<<std::endl;
                ::memset(recv_msg_buff, 0, 100);
                std::string recv_msg_str = "I am client0";

                REQUIRE(recv_msg_str.length() == read(g_client_array[0], recv_msg_buff, 100));
                std::string _msg = "tcpserver recv: " + std::string(recv_msg_buff);
                yLib::yLog::I(_msg);
                
                REQUIRE_THAT( recv_msg_str, Catch::Equals ( recv_msg_buff ));//verify client0 write

                REQUIRE(_msg.length() == write(g_client_array[0], _msg.c_str(), _msg.length()));

//===================================================================

                ::memset(recv_msg_buff, 0, 100);
                recv_msg_str = "I am client1";

                REQUIRE(recv_msg_str.length() == read(g_client_array[1], recv_msg_buff, 100));
                _msg = "tcpserver recv: " + std::string(recv_msg_buff);
                yLib::yLog::I(_msg);
                
                REQUIRE_THAT( recv_msg_str, Catch::Equals ( recv_msg_buff ));//verify client1 write

                REQUIRE(_msg.length() == write(g_client_array[1], _msg.c_str(), _msg.length()));

//===================================================================

                ::memset(recv_msg_buff, 0, 100);
                recv_msg_str = "I am client2";
                
                REQUIRE(recv_msg_str.length() == read(g_client_array[2], recv_msg_buff, 100));
                _msg = "tcpserver recv: " + std::string(recv_msg_buff);
                yLib::yLog::I(_msg);

                REQUIRE_THAT( recv_msg_str, Catch::Equals ( recv_msg_buff ));//verify client2 write

                REQUIRE(_msg.length() == write(g_client_array[2], _msg.c_str(), _msg.length()));
                
                sleep(10);//sleep for client close tcp-socket
                for(int _i = 0; _i < 3; _i++)
                    ::close(g_client_array[_i]);
                    
                break;
            }
            else{

                sleep(1);
            }
        }


    }
    
}

