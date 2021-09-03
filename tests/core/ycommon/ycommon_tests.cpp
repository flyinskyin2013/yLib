/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-18 17:22:49
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-01 11:57:40
 * @FilePath: \yLib\tests\ycommon\ycommon_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "test_common.hpp"

DEFINE_TEST_CASE_FOR_CLASS_INFO(yCommon)

TEST_CASE( "Test yCommon apis" , "[yCommon_Apis]" ){

    SECTION("test some apis ") {
#ifdef _WIN32


#elif __linux__ || __linux
    
        yLib::yCommon _com;

        REQUIRE_THAT(_com.ConvertErrnoToStr(EINVAL), Catch::Equals ("Invalid argument"));

        std::cout<<_com.ConvertErrnoToStr(EINVAL)<<std::endl;

#endif 
    }
}