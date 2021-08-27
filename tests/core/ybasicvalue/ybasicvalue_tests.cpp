/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 16:32:06
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 16:38:50
 * @FilePath: \yLib\tests\ybasicvalue\ybasicvalue_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include "test_common.hpp"

DEFINE_TEST_CASE_FOR_CLASS_INFO(yValue)

TEST_CASE( "Test yBasicValue constructor and convert apis" , "[yBasicValue_ConstructorAndConvertApis]" ){

    SECTION("test yBasicValue() ") {

        yLib::yBasicValue _tmp_value;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::NONE_TYPE);
    }

    SECTION("test yValue(const yValue &value)") {

        yLib::yBasicValue _tmp_value;
        yLib::yBasicValue _tmp_value1(_tmp_value);
        
        REQUIRE(_tmp_value1.GetType() == yLib::yBasicValue::NONE_TYPE);
    }

    SECTION("test operator=(const yValue &value) ") {

        yLib::yBasicValue _tmp_value;
        yLib::yBasicValue _tmp_value1;

        _tmp_value = _tmp_value;

        REQUIRE(_tmp_value1.GetType() == yLib::yBasicValue::NONE_TYPE);
    }

    SECTION("test yValue(const yValue &&value) ") {

        yLib::yBasicValue _tmp_value;
        yLib::yBasicValue _tmp_value1(std::move(_tmp_value));

        REQUIRE(_tmp_value1.GetType() == yLib::yBasicValue::NONE_TYPE);
    }

    SECTION("test operator=(const yValue &&value) ") {

        yLib::yBasicValue _tmp_value;
        yLib::yBasicValue _tmp_value1;
        
        _tmp_value1 = std::move(_tmp_value);


        REQUIRE(_tmp_value1.GetType() == yLib::yBasicValue::NONE_TYPE);
    }

    SECTION("test yBasicValue(int8_t value_) ") {

        yLib::yBasicValue _tmp_value((int8_t)-100);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT8_TYPE);

        REQUIRE(-100 == (int8_t)_tmp_value);
    }

    SECTION("test yBasicValue(uint8_t value_)") {

        yLib::yBasicValue _tmp_value((uint8_t)100);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT8_TYPE);

        REQUIRE(100 == (uint8_t)_tmp_value);
    }

    SECTION("test yBasicValue(int16_t value_) ") {

        yLib::yBasicValue _tmp_value((int16_t)-10000);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT16_TYPE);

        REQUIRE(-10000 == (int16_t)_tmp_value);
    }

    SECTION("test yBasicValue(uint16_t value_)") {

        yLib::yBasicValue _tmp_value((uint16_t)10000);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT16_TYPE);

        REQUIRE(10000 == (uint16_t)_tmp_value);
    }

    SECTION("test yBasicValue(int32_t value_) ") {

        yLib::yBasicValue _tmp_value((int32_t)-10000);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT32_TYPE);

        REQUIRE(-10000 == (int32_t)_tmp_value);
    }

    SECTION("test yBasicValue(uint32_t value_)") {

        yLib::yBasicValue _tmp_value((uint32_t)10000);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT32_TYPE);

        REQUIRE(10000 == (uint32_t)_tmp_value);
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

    SECTION("test yValue(yValueType type, void * value)") {

        yLib::yBasicValue _tmp_value(yLib::yValue::NULL_TYPE, nullptr);
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::NULL_TYPE);

        REQUIRE(_tmp_value.GetSpecialValue() == nullptr);
    }


    SECTION("test operator= function ") {

        yLib::yBasicValue _tmp_value;

        _tmp_value = (int8_t) -22;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT8_TYPE);
        REQUIRE(-22 == (int8_t)_tmp_value);

        _tmp_value = (uint8_t) 22;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT8_TYPE);
        REQUIRE(22 == (uint8_t)_tmp_value);

        _tmp_value = (int16_t) -22222;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT16_TYPE);
        REQUIRE(-22222 == (int16_t)_tmp_value);

        _tmp_value = (uint16_t) 22222;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT16_TYPE);
        REQUIRE(22222 == (uint16_t)_tmp_value);  

        _tmp_value = (int32_t) -22222;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::INT32_TYPE);
        REQUIRE(-22222 == (int32_t)_tmp_value);

        _tmp_value = (uint32_t) 22222;
        REQUIRE(_tmp_value.GetType() == yLib::yBasicValue::UINT32_TYPE);
        REQUIRE(22222 == (uint32_t)_tmp_value);

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

    }
}

