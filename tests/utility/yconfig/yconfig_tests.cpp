/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 16:31:42
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-07 11:44:20
 * @FilePath: \yLib\tests\yconfig\yconfig_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

TEST_CASE( "Test yConfig apis" , "[yConfig_Apis]" ){

    SECTION("test yConfigReadFile(const std::string & file_path)") {

        yLib::yConfig _config;
        
        REQUIRE(0 == _config.ReadFile("test.conf"));
    }

    SECTION("test yConfigGetValue(const char * node_path) and yConfigGetValue(const std::string & node_path)") {

        yLib::yConfig _config;
        _config.ReadFile("test.conf");

        yLib::yConfigValue _tmp_value = _config.GetValue("test.float");

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::FLOAT_TYPE);
        REQUIRE(2.3f == (float)_tmp_value);

        std::string _config_id = "test.string";
        _tmp_value  = _config.GetValue(_config_id);

        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::STRING_TYPE);
        REQUIRE_THAT( ((std::string)_tmp_value).c_str(), Catch::Equals ( "HHHHHHHHH" ));

        _tmp_value = _config.GetValue("test.int");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT32_TYPE);
        REQUIRE(1 == (int32_t)_tmp_value);
        
        _tmp_value = _config.GetValue("test.bool");
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::BOOL_TYPE);
        REQUIRE(true == (bool)_tmp_value);
    }

    SECTION("test yConfigSetValue , yConfigAddNode and  yConfigWriteFile series's apis ") {

        yLib::yConfig _config_tmp;
        yLib::yConfigValue _cfg_test_val;
        _cfg_test_val = (int)10;

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

        _cfg_test_val = 10;
        REQUIRE(0 == _config_tmp.SetValue("xxx.mmm.int", _cfg_test_val));

        _cfg_test_val = "yConfigValue test c_str";
        REQUIRE(0 == _config_tmp.SetValue("xxx.string", _cfg_test_val));

        _cfg_test_val = 3.1415f;
        REQUIRE(0 == _config_tmp.SetValue("xxx.float", _cfg_test_val));

        _cfg_test_val = false;
        REQUIRE(0 == _config_tmp.SetValue("xxx.nnn.bool", _cfg_test_val));
        
        REQUIRE(0 == _config_tmp.WriteFile("test_tmp.conf"));
    }

}

