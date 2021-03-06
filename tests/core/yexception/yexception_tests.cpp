/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 11:09:50
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-26 15:56:13
 * @FilePath: \yLib\tests\yexception\yexception_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"


TEST_CASE( "Test yException apis" , "[yException_Apis]" ){

    SECTION("test constructor and what() ") {

        yLib::yException _exp0;
        yLib::yException _exp1("exception 1 msg");
        
        REQUIRE_THAT( _exp0.what(), Catch::Equals ( "Basic exception of yLib." ));
        REQUIRE_THAT( _exp1.what(), Catch::Equals ( "exception 1 msg" ));
    }
}