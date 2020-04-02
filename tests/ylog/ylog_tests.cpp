/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-01-07 13:45:41
 * @LastEditors  : Sky
 * @LastEditTime : 2020-01-14 16:30:55
 * @FilePath: \yLib\tests\test_yLog\test_ylog.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <string>

TEST_CASE( "Test yLog for standard I/O" , "[yLog_IO]" ){

    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::I("test yLib::yLog::I");
        
        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::I(test_str.c_str());
    }


    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::W("test yLib::yLog::W");
        
        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::W(test_str.c_str());
    }


    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::E("test yLib::yLog::E");

        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::E(test_str.c_str());
    }


    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::D("test yLib::yLog::D");

        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::D(test_str.c_str());
    }
}



TEST_CASE( "Test yLog for log4cpp" , "[yLog_log4cpp]" ){

    std::string category_name0 = "test_ylog0";
    std::string category_name1 = "test_ylog1";

    SECTION("Init yLib::yLog"){

        yLib::yLog::SetLog4cpp(true);//enable log4cpp
        yLib::yLog::SetLog4cppSubCategory(category_name0);
        yLib::yLog::SetLog4cppLogLevel(category_name0, yLib::yLogLevel::_DISABLE_WARN_LOG_LEVEL_);
        yLib::yLog::SetyLogLogLevel(category_name0, yLib::yLogLevel::_DISABLE_INFO_LOG_LEVEL_);
        yLib::yLog::SetLog4cppSubCategory(category_name1);
        yLib::yLog::SetLog4cppLogLevel(category_name1, yLib::yLogLevel::_DISABLE_DEBUG_LOG_LEVEL_);
        yLib::yLog::SetyLogLogLevel(category_name0, yLib::yLogLevel::_DISABLE_ERROR_LOG_LEVEL_);
    }
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::I("test yLib::yLog::I");
        
        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::I(test_str.c_str());

        
        yLib::yLog::I(category_name0, test_str.c_str());
    }


    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::W("test yLib::yLog::W");
        
        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::W(test_str.c_str());
        

        yLib::yLog::W(category_name0, test_str.c_str());
    }


    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::E("test yLib::yLog::E");

        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::E(test_str.c_str());


        yLib::yLog::E(category_name1, test_str.c_str());
    }


    
    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::D("test yLib::yLog::D");
        
        std::string test_str ; 
        for (int i = 0; i < 50; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        yLib::yLog::D(test_str.c_str());


        yLib::yLog::D(category_name1, test_str.c_str());
    }
}


// TEST_CASE("Test ylog to log-file", "ylog"){

    
// }