/*
 * @Author: Sky
 * @Date: 2021-08-26 16:58:00
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 11:22:04
 * @Description: 
 */

#ifndef __TEST_COMMON_HPP___
#define __TEST_COMMON_HPP___

#define DEFINE_TEST_CASE_FOR_CLASS_INFO(cls_name) \
    TEST_CASE("Test "#cls_name" classinfo attributes" , "["#cls_name"_ClassInfoAttribute]" ){ \
        SECTION("test "#cls_name) { \
            REQUIRE_THAT( yLib::cls_name::GetClassInfo().class_name, Catch::Equals (#cls_name)); \
        } \
    }  
    
#define DEFINE_TEST_CASE_FOR_CLASS_INFO_DIFF(cls_name, cls_name_new) \
    TEST_CASE("Test "#cls_name" classinfo attributes" , "["#cls_name"_ClassInfoAttribute]" ){ \
        SECTION("test "#cls_name) { \
            REQUIRE_THAT( yLib::cls_name::GetClassInfo().class_name, Catch::Equals (#cls_name_new)); \
        } \
    }  
    
#endif //__TEST_COMMON_HPP___