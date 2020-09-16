/*
 * @Author: Sky
 * @Date: 2020-09-14 11:14:59
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-16 11:53:26
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"

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

        char recv_msg_buff[100];
        uint64_t svr_ip;
        uint64_t svr_port;

        REQUIRE(0 == tcp_server.bind("127.0.0.1", 12356));

        REQUIRE(0 == tcp_server.start_epoll_thread(test_OnClientConnectCB, test_OnClientDisconnectCB));
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

                REQUIRE(recv_msg_str.length() == tcp_server.read(recv_msg_buff, 100, g_client_array[0]));
                std::string _msg = "tcpserver recv: " + std::string(recv_msg_buff);
                yLib::yLog::I(_msg);
                
                REQUIRE_THAT( recv_msg_str, Catch::Equals ( recv_msg_buff ));//verify client0 write

                REQUIRE(_msg.length() == tcp_server.write(_msg.c_str(), _msg.length(), g_client_array[0]));

//===================================================================

                ::memset(recv_msg_buff, 0, 100);
                recv_msg_str = "I am client1";

                REQUIRE(recv_msg_str.length() == tcp_server.read(recv_msg_buff, 100, g_client_array[1]));
                _msg = "tcpserver recv: " + std::string(recv_msg_buff);
                yLib::yLog::I(_msg);
                
                REQUIRE_THAT( recv_msg_str, Catch::Equals ( recv_msg_buff ));//verify client1 write

                REQUIRE(_msg.length() == tcp_server.write(_msg.c_str(), _msg.length(), g_client_array[1]));

//===================================================================

                ::memset(recv_msg_buff, 0, 100);
                recv_msg_str = "I am client2";
                
                REQUIRE(recv_msg_str.length() == tcp_server.read(recv_msg_buff, 100, g_client_array[2]));
                _msg = "tcpserver recv: " + std::string(recv_msg_buff);
                yLib::yLog::I(_msg);

                REQUIRE_THAT( recv_msg_str, Catch::Equals ( recv_msg_buff ));//verify client2 write

                REQUIRE(_msg.length() == tcp_server.write(_msg.c_str(), _msg.length(), g_client_array[2]));
                break;
            }
            else{

                sleep(1);
            }
        }
    }
    
}

