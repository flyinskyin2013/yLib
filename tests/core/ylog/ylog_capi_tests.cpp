/*
 * @Author: Sky
 * @Date: 2021-11-09 11:32:55
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-09 11:56:43
 * @Description: 
 */
#include "catch2/catch.hpp"
#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include "ylib_c.h"

#ifdef __cplusplus
}
#endif //__cplusplus

#include <string>
#include <iostream>

TEST_CASE( "Test yLog for standard I/O" , "[yLog_IO]" ){

    SECTION( "yLog::Init()" ) {

        yLogTagPropertyC _prop;
        _prop.is_log_to_stdio = false;
        _prop.is_log_to_file = true;
        _prop.multi_log_file = true;
        ylib_ylog_init(&_prop, "");//set default tag prop


        ylib_ylog_i("Hello, yLog: %d", 2333);
        ylib_ylog_i_tag("yLogCTag", "Hello, yLog: %d", 2333);
        ylib_ylog_d("Hello, yLog: %d", 2333);
        ylib_ylog_d_tag("yLogCTag", "Hello, yLog: %d", 2333);
        ylib_ylog_w("Hello, yLog: %d", 2333);
        ylib_ylog_w_tag("yLogCTag", "Hello, yLog: %d", 2333);
        ylib_ylog_e("Hello, yLog: %d", 2333);
        ylib_ylog_e_tag("yLogCTag", "Hello, yLog: %d", 2333);
        
    }
}