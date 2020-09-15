/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 15:58:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-05-13 16:28:25
 * @FilePath: \yLib\tests\yhttp\yhttp_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"


TEST_CASE( "Test yHttp apis" , "[yHttp_Apis]" ){

    SECTION("tmp test") {

        yLib::yHttp http;

        yLib::yHttpRequestParam req_p;
        yLib::yHttpResponseInfo res_p;
        
        req_p.protocol_type = "http";
        req_p.port = 80;
        req_p.host = "www.baidu.com";

        http.yHttpGet(req_p, res_p);

        yLib::yLog::I("Recv data size %d", res_p.response_data_buf.size());
        res_p.response_data_buf.push_back(0x00);
        yLib::yLog::I("Recv data is %s", &res_p.response_data_buf[0]);
        
        // REQUIRE_THAT( _exp0.what(), Catch::Equals ( "Basic exception of yLib." ));
        // REQUIRE_THAT( _exp1.what(), Catch::Equals ( "exception 1 msg" ));
    }
}