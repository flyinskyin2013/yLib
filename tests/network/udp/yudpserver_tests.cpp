/*
 * @Author: Sky
 * @Date: 2020-09-09 13:42:25
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-30 18:03:17
 * @Description: 
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

using namespace yLib;
TEST_CASE( "Test yUdpServer apis" , "[yUdpServer_Apis]" ){

    SECTION("yUdpServer test") {

        yUdpSocket udp_server;
        REQUIRE(0 == udp_server.bind("0.0.0.0", 12355));

        char read_buffer[100];
        std::string client_ip;
        uint64_t client_port;
        
        //===================================================================

        ::memset(read_buffer, 0, 100);
        REQUIRE(::strlen("I am client0") == udp_server.recvfrom(read_buffer, 100, client_ip, client_port));

        yLib::yLog::I("udpserver recv: %s\n", read_buffer);
        REQUIRE_THAT( read_buffer, Catch::Equals ( "I am client0" ));//verify client0 sendto


        std::string msg_base = "server: your ip " + client_ip + " your port " + std::to_string(client_port);
        std::string msg_base_compare = "";
        yLib::yLog::I("udpserver recv: %s\n", msg_base.c_str());
        msg_base_compare = "server: your ip " + std::string("127.0.0.1") + " your port " + std::to_string(12345);
        REQUIRE_THAT( msg_base, Catch::Equals ( msg_base_compare ));//verify client0 sendto's ip and port

        REQUIRE((int64_t)msg_base.length() == udp_server.sendto(msg_base.c_str(), msg_base.length(), client_ip, client_port));//sendto msg back to client0


        //===================================================================

        ::memset(read_buffer, 0, 100);
        REQUIRE(::strlen("I am client1") == udp_server.recvfrom(read_buffer, 100, client_ip, client_port));
        yLib::yLog::I("udpserver recv: %s\n", read_buffer);
        REQUIRE_THAT( read_buffer, Catch::Equals ( "I am client1" ));//verify client1 sendto

        msg_base = "server: your ip " + client_ip + " your port " + std::to_string(client_port);
        yLib::yLog::I("udpserver recv: %s\n", msg_base.c_str());
        msg_base_compare = "server: your ip " + std::string("127.0.0.1") + " your port " + std::to_string(12346);
        REQUIRE_THAT( msg_base, Catch::Equals ( msg_base_compare ));//verify client1 sendto's ip and port


        REQUIRE((int64_t)msg_base.length() == udp_server.sendto(msg_base.c_str(), msg_base.length(), client_ip, client_port));//sendto msg back to client1

        //===================================================================
        
        REQUIRE(::strlen("I am client2") == udp_server.recvfrom(read_buffer, 100, client_ip, client_port));
        yLib::yLog::I("udpserver recv: %s\n", read_buffer);
        REQUIRE_THAT( read_buffer, Catch::Equals ( "I am client2" ));//verify client2 sendto

        msg_base = "server: your ip " + client_ip + " your port " + std::to_string(client_port);
        yLib::yLog::I("udpserver recv: %s\n", msg_base.c_str());

        //we can't verify client2's port . It's random.
        REQUIRE_THAT("127.0.0.1", Catch::Equals (client_ip));

        REQUIRE((int64_t)msg_base.length() == udp_server.sendto(msg_base.c_str(), msg_base.length(), client_ip, client_port));
        
        // REQUIRE_THAT( _exp0.what(), Catch::Equals ( "Basic exception of yLib." ));
        // REQUIRE_THAT( _exp1.what(), Catch::Equals ( "exception 1 msg" ));
    }
}
