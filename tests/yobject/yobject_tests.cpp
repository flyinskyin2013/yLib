/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 10:30:43
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-19 10:39:29
 * @FilePath: \yLib\tests\yobject\yobject_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"

TEST_CASE( "Test yObject basic attributes" , "[yObject_BasicAttributes]" ){

    SECTION("test _object_name ") {

        yLib::yObject _tmp_obj("test_obj");


        REQUIRE_THAT( _tmp_obj._object_name, Catch::Equals ( "test_obj" ));
    }
}