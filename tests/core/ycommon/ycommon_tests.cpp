/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-18 17:22:49
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-01 20:04:49
 * @FilePath: \yLib\tests\ycommon\ycommon_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"
#include <iostream>

TEST_CASE( "Test yCommon basic attributes" , "[yCommon_BasicAttributes]" ){

    SECTION("test GetyLibVersion() ") {

        yLib::yCommon _common;
        std::string _lib_ver = _common.GetyLibVersion();

        REQUIRE_THAT( _lib_ver, Catch::Equals ( std::string("V") + std::to_string(yLib::GetyLibBuildVersionMajor()) + "." + \
        std::to_string(yLib::GetyLibBuildVersionMinor()) + "." + std::to_string(yLib::GetyLibBuildVersionPatch()) ));
    }

    SECTION("test GetyLibBuildInfo() ") {

        yLib::yCommon _common;
        std::string _lib_build_info = _common.GetyLibBuildInfo();

        std::cout << _lib_build_info <<std::endl;
    }
}