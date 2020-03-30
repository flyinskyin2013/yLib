/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-18 17:22:49
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-19 11:50:30
 * @FilePath: \yLib\tests\ycommon\ycommon_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

TEST_CASE( "Test yCommon basic attributes" , "[yCommon_BasicAttributes]" ){

    SECTION("test GetyLibVersion() ") {

        yLib::yCommon _common;
        std::string _lib_ver = _common.GetyLibVersion();

        REQUIRE_THAT( _lib_ver, Catch::Equals ( std::string("V") + YLIB_VERSION ));
    }
}