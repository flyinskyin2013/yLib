/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 16:31:42
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-24 16:45:29
 * @FilePath: \yLib\tests\yconfig\yconfig_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

TEST_CASE( "Test yConfig apis" , "[yConfig_Apis]" ){

    SECTION("test yConfigReadFile(const std::string & file_path)") {

        yLib::yConfig _config;
        
        REQUIRE(0 == _config.yConfigReadFile("test.conf"));
    }

    SECTION("test yConfigGetValue(const char * node_path) and yConfigGetValue(const std::string & node_path)") {

        yLib::yConfig _config;
        _config.yConfigReadFile("test.conf");

        yLib::yConfigValue _tmp_value = _config.yConfigGetValue("test.float");

        REQUIRE(_tmp_value.GetyBasicValueType() == yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE);
        REQUIRE(2.3f == (float)_tmp_value);

        std::string _config_id = "test.string";
        _tmp_value  = _config.yConfigGetValue(_config_id);

        REQUIRE(_tmp_value.GetyBasicValueType() == yLib::yBasicValueType::STRING_YBASICVALUE_TYPE);
        REQUIRE_THAT( ((std::string)_tmp_value).c_str(), Catch::Equals ( "HHHHHHHHH" ));

        _tmp_value = _config.yConfigGetValue("test.int");
        REQUIRE(_tmp_value.GetyBasicValueType() == yLib::yBasicValueType::INT64_YBASICVALUE_TYPE);
        REQUIRE(1 == (int64_t)_tmp_value);
        
        _tmp_value = _config.yConfigGetValue("test.bool");
        REQUIRE(_tmp_value.GetyBasicValueType() == yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE);
        REQUIRE(true == (bool)_tmp_value);
    }

    SECTION("test yConfigSetValue , yConfigAddNode and  yConfigWriteFile series's apis ") {

        yLib::yConfig _config_tmp;
        yLib::yConfigValue _cfg_test_val;
        _cfg_test_val = (int)10;

        REQUIRE(0 == _config_tmp.yConfigAddNode(".", "xxx", yLib::yConfigValueType::GROUP_TYPE));

        REQUIRE(0 == _config_tmp.yConfigAddNode("xxx", "mmm", yLib::yConfigValueType::GROUP_TYPE));
        REQUIRE(0 == _config_tmp.yConfigAddNode("xxx", "nnn", yLib::yConfigValueType::GROUP_TYPE));

        REQUIRE(0 == _config_tmp.yConfigAddNode("xxx.mmm", "int", yLib::yConfigValueType::INT_TYPE, _cfg_test_val));

        _cfg_test_val = 2.567f;
        REQUIRE(0 == _config_tmp.yConfigAddNode("xxx", "float", yLib::yConfigValueType::FLOAT_TYPE, _cfg_test_val));
        
        _cfg_test_val = std::string("good val");
        REQUIRE(0 == _config_tmp.yConfigAddNode("xxx", "string", yLib::yConfigValueType::STRING_TYPE, _cfg_test_val));

        _cfg_test_val = true;
        REQUIRE(0 == _config_tmp.yConfigAddNode("xxx.nnn", "bool", yLib::yConfigValueType::BOOL_TYPE, _cfg_test_val));

        _cfg_test_val = 10;
        REQUIRE(0 == _config_tmp.yConfigSetValue("xxx.mmm.int", _cfg_test_val));

        _cfg_test_val = "yConfigValue test c_str";
        REQUIRE(0 == _config_tmp.yConfigSetValue("xxx.string", _cfg_test_val));

        _cfg_test_val = 3.1415f;
        REQUIRE(0 == _config_tmp.yConfigSetValue("xxx.float", _cfg_test_val));

        _cfg_test_val = false;
        REQUIRE(0 == _config_tmp.yConfigSetValue("xxx.nnn.bool", _cfg_test_val));
        
        REQUIRE(0 == _config_tmp.yConfigWriteFile("test_tmp.conf"));
    }

}

