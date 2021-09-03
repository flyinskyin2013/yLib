/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-01-07 13:45:41
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-03 17:54:02
 * @FilePath: \yLib\tests\test_yLog\test_ylog.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */


#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <string>
#include <iostream>

int _the_log_file_len0_case1 = 0;
int _the_log_file_len0_case2 = 0;
int _the_log_file_len1 = 0;
int _the_log_file_len2 = 0;


TEST_CASE( "Test yLog for standard I/O" , "[yLog_IO]" ){

    SECTION( "yLog::Init()" ) {

        yLib::yLogTagProperty _prop;
        _prop.is_log_to_stdio = false;
        _prop.is_log_to_file = true;
        _prop.file_param.multi_log_file = true;
        yLib::yLog::Init(_prop, "");//set default tag prop

        std::string category_name0 = "test_ylog0";
        std::string category_name1 = "test_ylog123";

        yLib::yLogTagProperty _tag_prop;
        
        _tag_prop.is_log_to_stdio = false;
        _tag_prop.is_log_to_file = true;
        _tag_prop.log_level &= yLib::yLogLevel::DISABLE_INFO_LOG_LEVEL; 
        _tag_prop.log_level &= yLib::yLogLevel::DISABLE_ERROR_LOG_LEVEL; 
        yLib::yLog::Init(_tag_prop, category_name0);

        yLib::yLogTagProperty _tag_prop1;
        _tag_prop1.is_log_to_stdio = false;
        _tag_prop1.is_log_to_file = true;
        _tag_prop1.log_level &= yLib::yLogLevel::DISABLE_WARN_LOG_LEVEL; 
        _tag_prop1.log_level &= yLib::yLogLevel::DISABLE_DEBUG_LOG_LEVEL; 
        yLib::yLog::Init(_tag_prop1, category_name1);
        
    }

    
    SECTION( "input a string that len is greater than 4095 bytes " ) {
        
        
        yLib::yLog::I("%d\n", 123);
        yLib::yLog::W("%x\n", 0x124);
        yLib::yLog::I("%s\n", "125");
        yLib::yLog::W("%s\n", "125");
        yLib::yLog::E("%s\n", "125");
        yLib::yLog::D("%s\n", "125");
        // E(const char *, const char *)
        yLib::yLog::D("%c%c%c\n", '1', '2', '6');
        
        // msg-len
        _the_log_file_len0_case1 += 4*7;

        // the log-header-len
        _the_log_file_len0_case1 += 47*7;

        std::string test_str ; 
        for (int i = 0; i < 90; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        
        LOGI() <<test_str; //auto newline 
        LOGW() <<test_str;
        LOGE() <<test_str;
        LOGD() <<test_str;
        
        // get buf overflow, the buf[1024*8 - 1] == '\n'
        _the_log_file_len0_case1 += 4*(1024*8);


        test_str += '\n';
        yLib::yLog::I("", test_str);
        yLib::yLog::W("", test_str);
        yLib::yLog::E("", test_str);
        yLib::yLog::D("", test_str);

        // get buf overflow, the buf[1024*8 - 1] == '\n'
        _the_log_file_len0_case1 += 4*(1024*8);


        std::cout<<"_the_log_file_len0_case1="<<_the_log_file_len0_case1<<std::endl;
    }
}


TEST_CASE( "Test yLog for multi-file" , "[yLog_multi-file]" ){

    std::string category_name0 = "test_ylog0";
    std::string category_name1 = "test_ylog123";

    SECTION( "input a string that len is greater than 4095 bytes " ) {

        yLib::yLog::I(category_name0, "%d\n", 127);
        yLib::yLog::W(category_name0, "%x\n", 0x128);
        yLib::yLog::E(category_name0, "%s\n", "129");
        // E(const char *, const char *)
        yLib::yLog::D(category_name0, "%c%c%c\n", '1', '3', '0');

        // msg-len
        _the_log_file_len1 += 4*2;

        // the log-header-len, tag-size + '\n' + base-msg-header
        _the_log_file_len1 += (47 + category_name0.length() + 1) *2;
        
        yLib::yLog::I(category_name1, "%d\n", 127);
        yLib::yLog::W(category_name1, "%x\n", 0x128);
        yLib::yLog::E(category_name1, "%s\n", "129");
        // E(const char *, const char *)
        yLib::yLog::D(category_name1, "%c%c%c\n", '1', '3', '0');
        // msg-len
        _the_log_file_len2 += 4*2;

        // the log-header-len, tag-size + '\n' + base-msg-header
        _the_log_file_len2 += (47 + category_name1.length() + 1) *2;

        std::string test_str ; 
        for (int i = 0; i < 81; i++)
            test_str += "test catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        
        // construct 8k msg-data
        test_str += "test   catchtest catchtest catchtest catchtest catchtest catchtest catchtest catchtest catch";//100 bytes;
        
        LOGI(test_ylog0) <<test_str; //auto newline 
        LOGW(test_ylog0) <<test_str;
        LOGE(test_ylog0) <<test_str;
        LOGD(test_ylog0) <<test_str;

        _the_log_file_len1 += 2*(1024*8);

        LOGI(test_ylog123) <<test_str; //auto newline 
        LOGW(test_ylog123) <<test_str;
        LOGE(test_ylog123) <<test_str;
        LOGD(test_ylog123) <<test_str;
        
        _the_log_file_len2 += 2*(1024*8);

        test_str += '\n';
        yLib::yLog::I(category_name0, test_str);
        yLib::yLog::W(category_name0, test_str);
        yLib::yLog::E(category_name0, test_str);
        yLib::yLog::D(category_name0, test_str);
        _the_log_file_len1 += 2*(1024*8);


        yLib::yLog::I(category_name1, test_str);
        yLib::yLog::W(category_name1, test_str);
        yLib::yLog::E(category_name1, test_str);
        yLib::yLog::D(category_name1, test_str);
        _the_log_file_len2 += 2*(1024*8);

    }

    // 
    yLib::yLogTagProperty _tag_prop;
    _tag_prop.is_log_to_file = true;
    yLib::yLog::Init(_tag_prop, "YLOGE");
    yLib::yLog::Init(_tag_prop, "YLOGE_IF");
    yLib::yLog::Init(_tag_prop, "YLOGE_IF_EVERY_N");
    yLib::yLog::Init(_tag_prop, "YLOG_EVERY_N");
    yLib::yLog::Init(_tag_prop, "YLOGE_FIRST_N");


    // call below 4 times
    LOGE(YLOGE)<<"my LOGE";
    int _t = 6;
    LOGI_IF(YLOGE_IF, (_t < 5))<<"my YLOGE_IF < 5";
    LOGI_IF(YLOGE_IF, (_t > 5))<<"my YLOGE_IF > 5";
    LOGW_IF_EVERY_N(YLOGE_IF_EVERY_N, (_t < 5), 2)<<"my YLOGE_IF_EVERY_N < 5 per 2times.";
    LOGW_IF_EVERY_N(YLOGE_IF_EVERY_N, (_t > 5), 2)<<"my YLOGE_IF_EVERY_N > 5 per 2times.";
    LOGD_EVERY_N(YLOG_EVERY_N, 2)<<"my YLOG_EVERY_N  per 2times.";
    LOGD_FIRST_N(YLOGE_FIRST_N, 2)<<"my YLOGE_FIRST_N  first 2times.";

    // msg-len
    _the_log_file_len0_case2 += 8 + 16 + 36 + 29 + 32;

    // the log-header-len
    _the_log_file_len0_case2 += (47 + 6) + (47 + 9) + (47 + 17) + (47 + 13) + (47 + 14);

    std::cout<<"_the_log_file_len0_case2="<<_the_log_file_len0_case2<<std::endl;
    std::cout<<"_the_log_file_len1="<<_the_log_file_len1<<std::endl;
    std::cout<<"_the_log_file_len2="<<_the_log_file_len2<<std::endl;
}
