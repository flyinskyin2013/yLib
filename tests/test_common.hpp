/*
 * @Author: Sky
 * @Date: 2021-08-26 16:58:00
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-09 17:37:04
 * @Description: 
 */

#ifndef __TEST_COMMON_HPP___
#define __TEST_COMMON_HPP___

#define DEFINE_TEST_CASE_FOR_CLASS_INFO(cls_name) \
    TEST_CASE("Test "#cls_name" classinfo attributes" , "["#cls_name"_ClassInfoAttribute]" ){ \
        SECTION("test "#cls_name) { \
            REQUIRE_THAT( yLib::cls_name::yLibGetClassInfo().class_name, Catch::Equals (#cls_name)); \
            if (nullptr != yLib::cls_name::yLibGetClassInfo().default_creator_method){\
                auto _obj_ptr = yLib::cls_name::yLibGetClassInfo().default_creator_method();\
            }\
        } \
    }  
    
#define DEFINE_TEST_CASE_FOR_CLASS_INFO_DIFF(cls_name, cls_name_new) \
    TEST_CASE("Test "#cls_name" classinfo attributes" , "["#cls_name"_ClassInfoAttribute]" ){ \
        SECTION("test "#cls_name) { \
            REQUIRE_THAT( yLib::cls_name::yLibGetClassInfo().class_name, Catch::Equals (#cls_name_new)); \
            if (nullptr != yLib::cls_name::yLibGetClassInfo().default_creator_method){\
                auto _obj_ptr = yLib::cls_name::yLibGetClassInfo().default_creator_method();\
            }\
        } \
    }  
    
#endif //__TEST_COMMON_HPP___