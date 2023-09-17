/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 16:31:42
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-30 16:09:42
 * @FilePath: /yLib/tests/utility/yconfig/yconfig_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <cfloat>
#include <climits>
#include <cstdint>

TEST_CASE( "Test yConfig apis" , "[yConfig_Apis]" ){

    SECTION("test yConfigReadFile(const std::string & file_path) failed") {

        yLib::yConfig _config;
        
        //for more code coverage, when ycompiler fails.
        //get  "Expect an identifier" error
        REQUIRE(-1 == _config.ReadFile("test_fail.conf"));
    }

    SECTION("test yConfigReadFile(const std::string & file_path)") {

        yLib::yConfig _config;
        
        REQUIRE(0 == _config.ReadFile("test.conf"));

        // more code coverage
        yLib::yConfigValue _value_c("aaaa");

        std::string _std_str = "bbbb";
        yLib::yConfigValue _value_std_str(_std_str);

        yLib::yConfigValue _value_copy_ctor(_value_std_str);

        yLib::yConfigValue _value_b(true);
        double _d_val = 123.123;
        yLib::yConfigValue _value_d(_d_val);

        yLib::yConfigValue _value_int64((int64_t)123456);

        yLib::yConfigValue _value_uintptr((uintptr_t)0x123456);
        yLib::yConfigValue _value_uintptr1;
        _value_uintptr1 = (uintptr_t)0x654321;
        yLib::yConfigValue _value_uintptr2;
        _value_uintptr2 = _value_uintptr1;
        uintptr_t _uintptr = (uintptr_t)_value_uintptr1;
    }

    SECTION("test yConfigGetValue(const char * node_path) and yConfigGetValue(const std::string & node_path)") {

        yLib::yConfig _config;
        _config.ReadFile("test.conf");

        yLib::yConfigValue _tmp_value;

        //test root decl
        _tmp_value = _config.GetValue("test_int");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);
        REQUIRE(1 == (int64_t)_tmp_value);

        _tmp_value = _config.GetValue("test_float");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);
        double _d_val_root = 2.3;
        REQUIRE(_d_val_root == (double)_tmp_value);

        _tmp_value = _config.GetValue("test_bool");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::BOOL_TYPE);
        REQUIRE(false == (bool)_tmp_value);

        _tmp_value  = _config.GetValue("test_string");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::STRING_TYPE);
        REQUIRE_THAT( ((std::string)_tmp_value).c_str(), Catch::Equals ( "HelloWorld" ));
        //test root decl

        _tmp_value = _config.GetValue("test.float");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);
        double _d_val = 2.3;
        REQUIRE(_d_val == (double)_tmp_value);

        std::string _config_id = "test.string";
        _tmp_value  = _config.GetValue(_config_id);

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::STRING_TYPE);
        REQUIRE_THAT( ((std::string)_tmp_value).c_str(), Catch::Equals ( "HHHHHHHHH" ));

        _tmp_value = _config.GetValue("test.int");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);
        REQUIRE(1 == (int64_t)_tmp_value);
        
        _tmp_value = _config.GetValue("test.bool");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::BOOL_TYPE);
        REQUIRE(true == (bool)_tmp_value);
        
        //get sub node
        _tmp_value = _config.GetValue("test.sub.float");

        _d_val = 4.3;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);
        REQUIRE(_d_val == (double)_tmp_value);

        _config_id = "test.sub.string";
        _tmp_value  = _config.GetValue(_config_id);

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::STRING_TYPE);
        REQUIRE_THAT( ((std::string)_tmp_value).c_str(), Catch::Equals ( "MMMMMMMM" ));

        _tmp_value = _config.GetValue("test.sub.int");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);
        REQUIRE(2 == (int64_t)_tmp_value);
        
        _tmp_value = _config.GetValue("test.sub.bool");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::BOOL_TYPE);
        REQUIRE(false == (bool)_tmp_value);
    }

    SECTION("test yConfigSetValue , yConfigAddNode and  yConfigWriteFile series's apis ") {

        yLib::yConfig _config_tmp;
        yLib::yConfigValue _cfg_test_val;
        _cfg_test_val = (int64_t)10;

        REQUIRE(0 == _config_tmp.AddNode(".", "xxx", yLib::yBasicValue::GROUP_TYPE));

        REQUIRE(0 == _config_tmp.AddNode("xxx", "mmm", yLib::yBasicValue::GROUP_TYPE));
        REQUIRE(0 == _config_tmp.AddNode("xxx", "nnn", yLib::yBasicValue::GROUP_TYPE));

        REQUIRE(0 == _config_tmp.AddNode("xxx.mmm", "int", yLib::yBasicValue::INT32_TYPE, _cfg_test_val));

        _cfg_test_val = 2.567f;
        REQUIRE(0 == _config_tmp.AddNode("xxx", "float", yLib::yBasicValue::FLOAT_TYPE, _cfg_test_val));
        
        _cfg_test_val = std::string("good val");
        REQUIRE(0 == _config_tmp.AddNode("xxx", "string", yLib::yBasicValue::STRING_TYPE, _cfg_test_val));

        _cfg_test_val = true;
        REQUIRE(0 == _config_tmp.AddNode("xxx.nnn", "bool", yLib::yBasicValue::BOOL_TYPE, _cfg_test_val));

        _cfg_test_val = (int64_t)10;
        REQUIRE(0 == _config_tmp.SetValue("xxx.mmm.int", _cfg_test_val));

        _cfg_test_val = "yConfigValue test c_str";
        REQUIRE(0 == _config_tmp.SetValue("xxx.string", _cfg_test_val));

        _cfg_test_val = 3.1415f;
        REQUIRE(0 == _config_tmp.SetValue("xxx.float", _cfg_test_val));

        _cfg_test_val = false;
        REQUIRE(0 == _config_tmp.SetValue("xxx.nnn.bool", _cfg_test_val));
        
        REQUIRE(0 == _config_tmp.WriteFile("test_tmp.conf"));
    }

    SECTION("test yConfig-int64-double-limits") {

        yLib::yConfig _config;
        _config.ReadFile("test.conf");

        yLib::yConfigValue _tmp_value = _config.GetValue("test_border.int_max");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);

        REQUIRE(INT64_MAX == (int64_t)_tmp_value);


        _tmp_value = _config.GetValue("test_border.int_min");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);

        // LLONG_MIN strtoll return LLONG_MAX LLONG_MIN
        // LLONG_MIN val is different from //http://www.cplusplus.com/reference/climits/
        REQUIRE(LLONG_MIN + 1 == (int64_t)_tmp_value);

        _tmp_value = _config.GetValue("test_border.double_p_max");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);

        REQUIRE(DBL_MAX == (double)_tmp_value);

        _tmp_value = _config.GetValue("test_border.double_p_min");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);

        //DBL_TRUE_MIN is c11 standard
        #define DBL_TRUE_MIN 4.9406564584124654e-324
        REQUIRE(DBL_TRUE_MIN == (double)_tmp_value);//denormalized numbers

        _tmp_value = _config.GetValue("test_border.double_n_max");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);

        REQUIRE((-DBL_TRUE_MIN) == (double)_tmp_value);
        #undef DBL_TRUE_MIN

        _tmp_value = _config.GetValue("test_border.double_n_min");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);

        REQUIRE((-DBL_MAX) == (double)_tmp_value);
    }

}

