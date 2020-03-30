/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-24 16:49:36
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-26 11:05:08
 * @FilePath: \yLib\tests\yjson\yjson_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"


#include <cstring>

TEST_CASE( "Test yJson apis" , "[yJson_Apis]" ){

    SECTION("test yJsonReadFile(std::string file) ") {

        yLib::yJson _json;
        
        REQUIRE(0 == _json.yJsonReadFile("test.json"));
    }


    SECTION("yJsonReadMemory(const int8_t * mem_addr, uint64_t mem_size) ") {

        yLib::yJson _json;
        const char * _json_mem_str = "{\"node_int\":64, \"node_bool\":true, \"node_double\":46.64, \"node_stdstring\":\"fu*k your life.\", \"node_null\":null}\n";
        REQUIRE(0 == _json.yJsonReadMemory((const int8_t *)_json_mem_str, strlen(_json_mem_str)));
    }

    SECTION("test yJsonValue & yJsonGetParsedJsonObject(void);") {

        yLib::yJson _json;
        
        REQUIRE(0 == _json.yJsonReadFile("test.json"));

        yLib::yJsonValue _got_json_value = _json.yJsonGetParsedJsonObject();
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _got_json_value._value_type);


        REQUIRE(yLib::yJsonValue::INT64_TYPE == _got_json_value["node_int"]._value_type);
        REQUIRE(64 == (int64_t)_got_json_value["node_int"]);

        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _got_json_value["node_bool"]._value_type);
        REQUIRE(true == (bool)_got_json_value["node_bool"]);

        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _got_json_value["node_double"]._value_type);
        REQUIRE(46.64 == (double)_got_json_value["node_double"]);

        REQUIRE(yLib::yJsonValue::STRING_TYPE == _got_json_value["node_stdstring"]._value_type);
        REQUIRE_THAT( ((std::string)_got_json_value["node_stdstring"]).c_str(), Catch::Equals ( "fu*k your life." ));

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_null"]._value_type);

        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _got_json_value["node_array"]._value_type);
        REQUIRE(64 == (int64_t)_got_json_value["node_array"][(uint64_t)0]);
        REQUIRE(true == (bool)_got_json_value["node_array"][(uint64_t)1]);
        REQUIRE(46.64  == (double)_got_json_value["node_array"][(uint64_t)2]);
        REQUIRE_THAT( ((std::string)_got_json_value["node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "fu*k your life." ));
        REQUIRE_THAT( ((std::string)_got_json_value["node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "fu*k your life." ));
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_array"][(uint64_t)5]._value_type);

//node_object
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _got_json_value["node_object"]["sub_node_int"]._value_type);
        REQUIRE(6464 == (int64_t)_got_json_value["node_object"]["sub_node_int"]);

        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _got_json_value["node_object"]["sub_node_bool"]._value_type);
        REQUIRE(false == (bool)_got_json_value["node_object"]["sub_node_bool"]);

        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _got_json_value["node_object"]["sub_node_double"]._value_type);
        REQUIRE(46.6446 == (double)_got_json_value["node_object"]["sub_node_double"]);

        REQUIRE(yLib::yJsonValue::STRING_TYPE == _got_json_value["node_object"]["sub_node_stdstring"]._value_type);
        REQUIRE_THAT( ((std::string)_got_json_value["node_object"]["sub_node_stdstring"]).c_str(), Catch::Equals ( "sub: fu*k your life." ));

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_object"]["sub_node_null"]._value_type);

        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _got_json_value["node_object"]["sub_node_array"]._value_type);
        REQUIRE(64 == (int64_t)_got_json_value["node_object"]["sub_node_array"][(uint64_t)0]);
        REQUIRE(true == (bool)_got_json_value["node_object"]["sub_node_array"][(uint64_t)1]);
        REQUIRE(46.64  == (double)_got_json_value["node_object"]["sub_node_array"][(uint64_t)2]);
        REQUIRE_THAT( ((std::string)_got_json_value["node_object"]["sub_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "sub array: fu*k your life." ));
        REQUIRE_THAT(  ((std::string)_got_json_value["node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "sub array: fu*k your life." ));
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_object"]["sub_node_array"][(uint64_t)5]._value_type);

    }

    SECTION("test int yJsonSetJsonObject(yJsonValue & obj_val_)") {

        yLib::yJsonValue _tmp_json_obj;
        yLib::yJson _json;

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj._value_type);

        _tmp_json_obj["test_node_int"] = (int64_t)-12345;
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _tmp_json_obj["test_node_int"]._value_type);
        REQUIRE(-12345 == (int64_t)_tmp_json_obj["test_node_int"]);

        _tmp_json_obj["test_node_bool"] = true;
        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _tmp_json_obj["test_node_bool"]._value_type);
        REQUIRE(true == (bool)_tmp_json_obj["test_node_bool"]);

        _tmp_json_obj["test_node_double"] = (double)-12345.12345;
        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _tmp_json_obj["test_node_double"]._value_type);
        REQUIRE(-12345.12345 == (double)_tmp_json_obj["test_node_double"]);

        _tmp_json_obj["test_node_stdstring"] = "test node stdstring ...";
        REQUIRE(yLib::yJsonValue::STRING_TYPE == _tmp_json_obj["test_node_stdstring"]._value_type);
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_stdstring"]).c_str(), Catch::Equals ( "test node stdstring ..." ));

        _tmp_json_obj["test_node_null"] = yLib::yJsonValue();
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_null"]._value_type);

        _tmp_json_obj["test_node_array"] = yLib::yJsonValue(yLib::yJsonValue::ARRAY_TYPE);
        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _tmp_json_obj["test_node_array"]._value_type);

        _tmp_json_obj["test_node_array"][(uint64_t)0] = (int64_t)-123654;
        REQUIRE(-123654 == (int64_t)_tmp_json_obj["test_node_array"][(uint64_t)0]);

        _tmp_json_obj["test_node_array"][(uint64_t)1] = false;
        REQUIRE(false == (bool)_tmp_json_obj["test_node_array"][(uint64_t)1]);

        _tmp_json_obj["test_node_array"][(uint64_t)2] = -12.3654;
        REQUIRE(-12.3654  == (double)_tmp_json_obj["test_node_array"][(uint64_t)2]);

        _tmp_json_obj["test_node_array"][(uint64_t)3] = "test node array id 3";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "test node array id 3" ));

        _tmp_json_obj["test_node_array"][(uint64_t)4] = yLib::yJsonValue(yLib::yJsonValue::OBJECT_TYPE);
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == (int64_t)_tmp_json_obj["test_node_array"][(uint64_t)4]._value_type);

        _tmp_json_obj["test_node_array"][(uint64_t)4]["node_array_obj"] = "test node array id 4";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "test node array id 4" ));

        _tmp_json_obj["test_node_array"][(uint64_t)5] = yLib::yJsonValue();
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_array"][(uint64_t)5]._value_type);

//node_object
        _tmp_json_obj["test_node_object"] = yLib::yJsonValue(yLib::yJsonValue::OBJECT_TYPE);
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == (int64_t)_tmp_json_obj["test_node_object"]._value_type);

        _tmp_json_obj["test_node_object"]["sub_node_int"] = (int64_t)-12345;
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _tmp_json_obj["test_node_object"]["sub_node_int"]._value_type);
        REQUIRE(-12345 == (int64_t)_tmp_json_obj["test_node_object"]["sub_node_int"]);

        _tmp_json_obj["test_node_object"]["sub_node_bool"] = true;
        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_bool"]._value_type);
        REQUIRE(true == (bool)_tmp_json_obj["test_node_object"]["sub_node_bool"]);

        _tmp_json_obj["test_node_object"]["sub_node_double"] = 654.123;
        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _tmp_json_obj["test_node_object"]["sub_node_double"]._value_type);
        REQUIRE(654.123 == (double)_tmp_json_obj["test_node_object"]["sub_node_double"]);

        _tmp_json_obj["test_node_object"]["sub_node_stdstring"] = "sub: fu*k your life.";
        REQUIRE(yLib::yJsonValue::STRING_TYPE == _tmp_json_obj["test_node_object"]["sub_node_stdstring"]._value_type);
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_stdstring"]).c_str(), Catch::Equals ( "sub: fu*k your life." ));

        _tmp_json_obj["test_node_object"]["sub_node_null"] = yLib::yJsonValue(yLib::yJsonValue::NULL_TYPE);
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_null"]._value_type);

        _tmp_json_obj["test_node_object"]["sub_node_array"] = yLib::yJsonValue(yLib::yJsonValue::ARRAY_TYPE);
        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"]._value_type);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)0] = (int64_t)123978;
        REQUIRE(123978 == (int64_t)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)0]);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)1] = true;
        REQUIRE(true == (bool)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)1]);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)2] = 12.456;
        REQUIRE(12.456  == (double)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)2]);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)3] = "sub array: fu*k your life. id 3";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "sub array: fu*k your life. id 3" ));

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"] = "sub array: fu*k your life. id 4";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "sub array: fu*k your life. id 4" ));

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)5] = yLib::yJsonValue(yLib::yJsonValue::NULL_TYPE);
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)5]._value_type);


        REQUIRE(0 == _json.yJsonSetJsonObject(_tmp_json_obj));

        REQUIRE(0 == _json.yJsonWriteFile("test_tmp.json"));
    }

}

