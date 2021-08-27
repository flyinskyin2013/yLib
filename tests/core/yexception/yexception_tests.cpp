/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 11:09:50
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 10:11:41
 * @FilePath: \yLib\tests\yexception\yexception_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yException)

TEST_CASE( "Test yException apis" , "[yException_Apis]" ){

    SECTION("test constructor and what() ") {

        yLib::yException _exp0;
        yLib::yException _exp1("exception 1 msg");
        
        REQUIRE_THAT( _exp0.what(), Catch::Equals ( "" ));
        REQUIRE_THAT( _exp1.what(), Catch::Equals ( "exception 1 msg" ));

        std::cout<<_exp0.GetStackTrace()<<std::endl;
        std::cout<<_exp1.GetStackTrace()<<std::endl;
    }
}