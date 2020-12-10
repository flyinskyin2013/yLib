/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 16:32:06
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-07 14:20:51
 * @FilePath: \yLib\tests\ybasicvalue\ybasicvalue_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

TEST_CASE( "Test yBasicValue constructor and convert apis" , "[yBasicValue_ConstructorAndConvertApis]" ){

    SECTION("test yBasicValue() ") {

        yLib::yBasicValue _tmp_value;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::NONE_TYPE);
    }

    SECTION("test yBasicValue(int64_t value_) ") {

        yLib::yBasicValue _tmp_value((int64_t)-10000);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);

        REQUIRE(-10000 == (int64_t)_tmp_value);
    }

    SECTION("test yBasicValue(uint64_t value_)") {

        yLib::yBasicValue _tmp_value((uint64_t)10000);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT64_TYPE);

        REQUIRE(10000 == (uint64_t)_tmp_value);
    }

    SECTION("test yBasicValue(bool value_) ") {

        yLib::yBasicValue _tmp_value(true);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::BOOL_TYPE);

        REQUIRE(true == (bool)_tmp_value);
    }

    SECTION("test yBasicValue(float value_) ") {

        yLib::yBasicValue _tmp_value(1.123456f);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::FLOAT_TYPE);
        
        //Catch::Approx<float> _tmp_target = Catch::Approx(1.123456);
        REQUIRE(1.123456f == (float)_tmp_value);
    }

    SECTION("test yBasicValue(double value_) ") {

        yLib::yBasicValue _tmp_value(1.12345678);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);

        //Catch::Approx _tmp_target = Catch::Approx(1.123456f);
        REQUIRE(1.12345678 == (double)_tmp_value);
    }

    SECTION("test yBasicValue(std::string & value_) ") {

        std::string _tmp_test_str = "test yBasicValue by str";
        yLib::yBasicValue _tmp_value(_tmp_test_str);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::STRING_TYPE);

        std::string _ss_tmp_str = _tmp_value;

        REQUIRE_THAT( _ss_tmp_str.c_str(), Catch::Equals ( "test yBasicValue by str" ));
    }

    // SECTION("test yBasicValue(const char * value_) ") {

    //     const char * _tmp_test_c_str = "test yBasicValue by c_str";
    //     yLib::yBasicValue _tmp_value(_tmp_test_c_str);
    //     REQUIRE(_tmp_value.GetyBasicValueType() == yLib::yBasicValueType::STRING_YBASICVALUE_TYPE);

    //     REQUIRE_THAT( (const char *)_tmp_value, Catch::Equals ( "test yBasicValue by c_str" ));
    // }

    SECTION("test operator= function ") {

        yLib::yBasicValue _tmp_value;
        
        _tmp_value = (int64_t) -22222;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT64_TYPE);
        REQUIRE(-22222 == (int64_t)_tmp_value);

        _tmp_value = (uint64_t) 22222;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT64_TYPE);
        REQUIRE(22222 == (uint64_t)_tmp_value);

        _tmp_value = 2.123456f;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::FLOAT_TYPE);
        //Catch::Approx _tmp_target = Catch::Approx(2.123456f);
        REQUIRE(2.123456f == (float)_tmp_value);

        _tmp_value = 2.12345678;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::DOUBLE_TYPE);
        //Catch::Approx _tmp_target = Catch::Approx(2.12345678);
        REQUIRE(2.12345678 == (double)_tmp_value);

        _tmp_value = false;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::BOOL_TYPE);
        REQUIRE(false == (bool)_tmp_value);

        std::string _tmp_str = "test yBasicValue by c_str1";
        _tmp_value = _tmp_str;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::STRING_TYPE);
        std::string _ss_tmp_str = _tmp_value;
        //std::string _ss_tmp_str1 = (std::string)_tmp_value; //Do not support this convert-type;
        REQUIRE_THAT( _ss_tmp_str.c_str(), Catch::Equals ( "test yBasicValue by c_str1" ));

        // const char * _tmp_test_c_str = "test yBasicValue by c_str1";
        // _tmp_value = _tmp_test_c_str;
        // REQUIRE(_tmp_value.GetyBasicValueType() == yLib::yBasicValueType::STRING_YBASICVALUE_TYPE);
        // REQUIRE_THAT( (const char *)_tmp_value, Catch::Equals ( "test yBasicValue by c_str1" ));
    }
}

