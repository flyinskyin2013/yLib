/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 15:59:54
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-12 16:22:04
 * @FilePath: \yLib\tests\yshell\yshell_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"

// #define UNICODE

#include "ylib.hpp"


#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yShell)

TEST_CASE( "Test yShell apis" , "[yShell_Apis]" ){

    yLib::yShell shell;

    SECTION("test Execute() advanced option") {

        std::vector<YLIB_STD_STRING> cmd_vec;
        std::vector<YLIB_STD_STRING> arg_vec;
        std::vector<YLIB_STD_STRING> cmd_env_vec;
        std::vector<YLIB_STD_STRING> cmd_result_vec;

        #ifdef __linux__ || __linux
        cmd_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING(/usr/bin/env));
        #endif //__linux__ || __linux

        cmd_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING(env));

        cmd_env_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING("MY_VAR=my_env_var"));

        REQUIRE(0 == (int)shell.Execute(cmd_vec, arg_vec, cmd_env_vec, cmd_result_vec));
        REQUIRE(1 == cmd_result_vec.size());

        #ifdef __linux__ || __linux
        REQUIRE_THAT(cmd_result_vec[0].c_str(), Catch::Equals ( "MY_VAR=my_env_var" ));
        #endif //__linux__ || __linux
    }

    SECTION("test ReconfigyShellBuffer()") {

        std::vector<YLIB_STD_STRING> cmd_vec;
        std::vector<YLIB_STD_STRING> arg_vec;
        std::vector<YLIB_STD_STRING> cmd_env_vec;
        std::vector<YLIB_STD_STRING> cmd_result_vec;
        auto _construct_test_str = [](){

            YLIB_STD_STRING tmp_str;
            for (int i = 0; i < 50; i++){

                tmp_str += CONVERT_STR_TO_YLIB_STD_STRING(abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcde);
            }
            return tmp_str;//2500 charactors
        };

        YLIB_STD_STRING _test_str =  _construct_test_str();

        #ifdef __linux__ || __linux
        cmd_vec.push_back("/bin/echo");
        #endif //__linux__ || __linux

        cmd_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING(echo));
        
        #ifdef __linux__ || __linux
        cmd_vec.push_back("-e");
        #endif //__linux__ || __linux
        
        cmd_vec.push_back(_test_str);

        REQUIRE(0 == (int)shell.Execute(cmd_vec, arg_vec, cmd_env_vec, cmd_result_vec));
        REQUIRE(1 == cmd_result_vec.size());
        REQUIRE(2500 == cmd_result_vec[0].length());
    }



    SECTION("test RunShellCommandEx() 100 times") {

        std::vector<YLIB_STD_STRING> cmd_vec;
        std::vector<YLIB_STD_STRING> arg_vec;
        std::vector<YLIB_STD_STRING> cmd_env_vec;
        std::vector<YLIB_STD_STRING> cmd_result_vec;

        #ifdef __linux__ || __linux
        cmd_vec.push_back("/bin/echo");
        #endif //__linux__ || __linux

        cmd_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING("echo"));
        
        #ifdef __linux__ || __linux
        cmd_vec.push_back("-e");
        #endif //__linux__ || __linux

        cmd_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING("test 100 times"));

        cmd_env_vec.push_back(CONVERT_STR_TO_YLIB_STD_STRING("MY_VAR=my_env_var"));

        for (uint64_t ii = 0; ii < 100; ii++){

            std::vector<YLIB_STD_STRING> empty_result;
            cmd_result_vec.swap(empty_result);

            REQUIRE(0 == (int)shell.Execute(cmd_vec, arg_vec, cmd_env_vec, cmd_result_vec));
            REQUIRE(1 == cmd_result_vec.size());

            #ifdef __linux__ || __linux
            REQUIRE_THAT(cmd_result_vec[0].c_str(), Catch::Equals ( "test 100 times" ));
            #endif //__linux__ || __linux
        }

    }
}