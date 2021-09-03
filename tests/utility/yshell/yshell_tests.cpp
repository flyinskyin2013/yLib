/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 15:59:54
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:24:12
 * @FilePath: \yLib\tests\yshell\yshell_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"


#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(yShell)

TEST_CASE( "Test yShell apis" , "[yShell_Apis]" ){

    yLib::yShell shell;

    SECTION("test RunShellCommandEx() advanced option") {

        std::vector<std::string> cmd_vec;
        std::vector<std::string> cmd_env_vec;
        std::vector<std::string> cmd_result_vec;

        cmd_vec.push_back("/usr/bin/env");
        cmd_vec.push_back("env");

        cmd_env_vec.push_back("MY_VAR=my_env_var");

        REQUIRE(0 == shell.RunShellCommandEx(cmd_vec, cmd_env_vec, cmd_result_vec));
        REQUIRE(1 == cmd_result_vec.size());
        REQUIRE_THAT(cmd_result_vec[0].c_str(), Catch::Equals ( "MY_VAR=my_env_var" ));
    }

    SECTION("test ReconfigyShellBuffer()") {

        std::vector<std::string> cmd_vec;
        std::vector<std::string> cmd_env_vec;
        std::vector<std::string> cmd_result_vec;
        auto _construct_test_str = [](){

            std::string tmp_str;
            for (int i = 0; i < 50; i++){

                tmp_str += "abcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcdeabcde";
            }
            return tmp_str;//2500 charactors
        };

        std::string _test_str =  _construct_test_str();

        cmd_vec.push_back("/bin/echo");
        cmd_vec.push_back("echo");
        cmd_vec.push_back("-e");
        cmd_vec.push_back(_test_str);

        REQUIRE(0 == shell.RunShellCommandEx(cmd_vec, cmd_env_vec, cmd_result_vec));
        REQUIRE(3 == cmd_result_vec.size());
        REQUIRE(1023 == cmd_result_vec[0].length());
        REQUIRE(1023 == cmd_result_vec[1].length());
        REQUIRE(454 == cmd_result_vec[2].length());////only echo will add ' '


        shell.ReconfigyShellBuffer(20, 20, 3072);

        std::vector<std::string> cmd_empty;
        std::vector<std::string> cmd_env_empty;
        std::vector<std::string> cmd_result_empty;
        cmd_vec.swap(cmd_empty);
        cmd_env_vec.swap(cmd_env_empty);
        cmd_result_vec.swap(cmd_result_empty);


        cmd_vec.push_back("/bin/echo");
        cmd_vec.push_back("echo");
        cmd_vec.push_back(_test_str);

        REQUIRE(0 == shell.RunShellCommandEx(cmd_vec, cmd_env_vec, cmd_result_vec));
        REQUIRE(1 == cmd_result_vec.size());
        REQUIRE(2500 == cmd_result_vec[0].length());//only echo will add ' '
        
    }



    SECTION("test RunShellCommandEx() 100 times") {

        std::vector<std::string> cmd_vec;
        std::vector<std::string> cmd_env_vec;
        std::vector<std::string> cmd_result_vec;

        cmd_vec.push_back("/bin/echo");
        cmd_vec.push_back("echo");
        cmd_vec.push_back("-e");
        cmd_vec.push_back("test 100 times");

        cmd_env_vec.push_back("MY_VAR=my_env_var");

        for (uint64_t ii = 0; ii < 100; ii++){

            std::vector<std::string> empty_result;
            cmd_result_vec.swap(empty_result);

            REQUIRE(0 == shell.RunShellCommandEx(cmd_vec, cmd_env_vec, cmd_result_vec));
            REQUIRE(1 == cmd_result_vec.size());
            REQUIRE_THAT(cmd_result_vec[0].c_str(), Catch::Equals ( "test 100 times" ));
        }

    }
}