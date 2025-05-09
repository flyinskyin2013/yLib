/*
 * @Author: Sky
 * @Date: 2020-09-09 13:42:25
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-09 14:26:12
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"


#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yUdpSocket)

DEFINE_TEST_CASE_FOR_CLASS_INFO(yAbstractSocket)
// TEST_CASE( "Test yAbstractSocket apis" , "[yAbstractSocket_Apis]" ){

//     SECTION("yAbstractSocket test") {
//         yLib::yAbstractSocket _ab_socket;
//     }

// }


using namespace yLib;
TEST_CASE( "Test yUdpClient apis" , "[yUdpClient_Apis]" ){

    SECTION("yUdpClient test") {
        
        yUdpSocket udp_client0;
        yUdpSocket udp_client1;
        yUdpSocket udp_client2(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        yUdpSocket udp_client3(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        char recv_msg_buff[100];
        std::string svr_ip;
        uint64_t svr_port;

        REQUIRE(0 == udp_client0.bind("", 12345));
        REQUIRE(0 == udp_client1.bind("", 12346));
        //udp_client2 not bind, system automatically choose port.

        // check bind failed.
        REQUIRE(-1 == udp_client3.bind("", 12346));

        std::string msg0 = "I am client0";
        REQUIRE((int64_t)msg0.length() == udp_client0.sendto(msg0.c_str(), msg0.length(), "127.0.0.1", 12355));

        std::string msg1 = "I am client1";
        REQUIRE((int64_t)msg0.length() == udp_client1.sendto(msg1.c_str(), msg1.length(), "127.0.0.1", 12355));

        std::string msg2 = "I am client2";
        REQUIRE((int64_t)msg0.length() == udp_client2.sendto(msg2.c_str(), msg2.length(), "127.0.0.1", 12355));


        memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        std::string msg_base_compare = "server: your ip " + std::string("127.0.0.1") + " your port " + std::to_string(12345);
        REQUIRE((int64_t)msg_base_compare.length() == udp_client0.recvfrom(recv_msg_buff, 100, svr_ip, svr_port));
        REQUIRE(12355 == svr_port);
        std::string svr_ip_str = svr_ip;
        REQUIRE_THAT(svr_ip_str.c_str(), Catch::Equals ( "127.0.0.1" ));
        yLib::yLog::I("client0: recv: %s,  ip is %s,  port is %d\n", recv_msg_buff , svr_ip.c_str(), svr_port);
        REQUIRE_THAT( recv_msg_buff, Catch::Equals ( msg_base_compare ));


        memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        REQUIRE((int64_t)msg_base_compare.length() == udp_client1.recvfrom(recv_msg_buff, 100, svr_ip, svr_port));
        REQUIRE(12355 == svr_port);
        svr_ip_str = svr_ip;
        REQUIRE_THAT(svr_ip_str, Catch::Equals ( "127.0.0.1" ));
        yLib::yLog::I("client1: recv: %s,  ip is %s,  port is %d\n", recv_msg_buff , svr_ip.c_str(), svr_port);

        msg_base_compare = "server: your ip " + std::string("127.0.0.1") + " your port " + std::to_string(12346);
        REQUIRE_THAT( recv_msg_buff, Catch::Equals ( msg_base_compare ));

        memset(recv_msg_buff, 0, sizeof(recv_msg_buff));
        REQUIRE((int64_t)msg_base_compare.length() == udp_client2.recvfrom(recv_msg_buff, 100, svr_ip, svr_port));
        REQUIRE(12355 == svr_port);
        svr_ip_str = svr_ip;
        REQUIRE_THAT(svr_ip_str, Catch::Equals ( "127.0.0.1" ));
        yLib::yLog::I("client2: recv: %s,  ip is %s,  port is %d\n", recv_msg_buff , svr_ip.c_str(), svr_port);
    }
}
