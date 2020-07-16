/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-01-14 15:03:53
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-16 09:56:48
 * @FilePath: \yLib\tests\all_in_one\all_in_one_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */





#if defined(_WIN32) || defined(_WIN64)

//include ylog tests
#include "ylog/ylog_tests.cpp"

//include ycommon tests
#include "ycommon/ycommon_tests.cpp"

//include yobject tests
#include "yobject/yobject_tests.cpp"

//include ybasicvalue tests
#include "ybasicvalue/ybasicvalue_tests.cpp"

//include yconfig tests
#include "yconfig/yconfig_tests.cpp"

//include yjson tests
#include "yjson/yjson_tests.cpp"

//include yexception tests
#include "yexception/yexception_tests.cpp"

#elif defined(__linux__) || defined(__linux)
    
//include ylog tests
#include "ylog/ylog_tests.cpp"

//include ycommon tests
#include "ycommon/ycommon_tests.cpp"

//include yobject tests
#include "yobject/yobject_tests.cpp"

//include ybasicvalue tests
#include "ybasicvalue/ybasicvalue_tests.cpp"

//include yconfig tests
#include "yconfig/yconfig_tests.cpp"

//include yjson tests
#include "yjson/yjson_tests.cpp"

//include yexception tests
#include "yexception/yexception_tests.cpp"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix




