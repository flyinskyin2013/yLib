/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-24 16:49:36
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-10 11:30:08
 * @FilePath: \yLib\tests\yjson\yjson_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"


#include <cstring>

TEST_CASE( "Test yJson apis" , "[yJson_Apis]" ){

    SECTION("test yJsonReadFile(std::string file) ") {

        yLib::yJson _json;
        
        REQUIRE(0 == _json.ReadFile("test.json"));

        // code coverage improve
        char _tmp_buf[1024];
        REQUIRE(0 == _json.WriteMemory((int8_t *)_tmp_buf, 1024));
        
        // code coverage improve
        std::string _key_str = "node_int";
        yLib::yJsonValue _got_json_value = _json.GetRootObject();
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _got_json_value[_key_str].GetType());
        REQUIRE(64 == (int64_t)_got_json_value[_key_str]);
    }

    SECTION("test yJsonValue ctor ") {

        yLib::yJsonValue _val_int64(int64_t(1));
        yLib::yJsonValue _val_uint64(uint64_t(2));
        uint64_t _val_test = (uint64_t)_val_uint64;
        
        yLib::yJsonValue _val_bool(false);
        yLib::yJsonValue _val_double(double(3.f));
        yLib::yJsonValue _val_string(std::string("4"));
        _val_string = std::string("5");
        yLib::yJsonValue _val_obj(yLib::yJsonValue::yValueType::OBJECT_TYPE);

        // invalid type
        yLib::yJsonValue _val_obj1(yLib::yJsonValue::yValueType::INT8_TYPE);
        _val_obj1 = _val_uint64;

    }

    SECTION("yJsonReadMemory(const int8_t * mem_addr, uint64_t mem_size) ") {

        yLib::yJson _json;
        const char * _json_mem_str = "{\"node_int\":64, \"node_bool\":true, \"node_double\":46.64, \"node_stdstring\":\"fu*k your life.\", \"node_null\":null}\n";
        REQUIRE(0 == _json.ReadMemory((const int8_t *)_json_mem_str, strlen(_json_mem_str)));
    }

    SECTION("test yJsonValue & yJsonGetParsedJsonObject(void);") {

        yLib::yJson _json;
        
        REQUIRE(0 == _json.ReadFile("test.json"));

        yLib::yJsonValue _got_json_value = _json.GetRootObject();
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _got_json_value.GetType());


        REQUIRE(yLib::yJsonValue::INT64_TYPE == _got_json_value["node_int"].GetType());
        REQUIRE(64 == (int64_t)_got_json_value["node_int"]);

        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _got_json_value["node_bool"].GetType());
        REQUIRE(true == (bool)_got_json_value["node_bool"]);

        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _got_json_value["node_double"].GetType());
        REQUIRE(46.64 == (double)_got_json_value["node_double"]);

        REQUIRE(yLib::yJsonValue::STRING_TYPE == _got_json_value["node_stdstring"].GetType());
        REQUIRE_THAT( ((std::string)_got_json_value["node_stdstring"]).c_str(), Catch::Equals ( "fu*k your life." ));

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_null"].GetType());

        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _got_json_value["node_array"].GetType());
        REQUIRE(64 == (int64_t)_got_json_value["node_array"][(uint64_t)0]);
        REQUIRE(true == (bool)_got_json_value["node_array"][(uint64_t)1]);
        REQUIRE(46.64  == (double)_got_json_value["node_array"][(uint64_t)2]);
        REQUIRE_THAT( ((std::string)_got_json_value["node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "fu*k your life." ));
        REQUIRE_THAT( ((std::string)_got_json_value["node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "fu*k your life." ));
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_array"][(uint64_t)5].GetType());

//node_object
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _got_json_value["node_object"]["sub_node_int"].GetType());
        REQUIRE(6464 == (int64_t)_got_json_value["node_object"]["sub_node_int"]);

        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _got_json_value["node_object"]["sub_node_bool"].GetType());
        REQUIRE(false == (bool)_got_json_value["node_object"]["sub_node_bool"]);

        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _got_json_value["node_object"]["sub_node_double"].GetType());
        REQUIRE(46.6446 == (double)_got_json_value["node_object"]["sub_node_double"]);

        REQUIRE(yLib::yJsonValue::STRING_TYPE == _got_json_value["node_object"]["sub_node_stdstring"].GetType());
        REQUIRE_THAT( ((std::string)_got_json_value["node_object"]["sub_node_stdstring"]).c_str(), Catch::Equals ( "sub: fu*k your life." ));

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_object"]["sub_node_null"].GetType());

        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _got_json_value["node_object"]["sub_node_array"].GetType());
        REQUIRE(64 == (int64_t)_got_json_value["node_object"]["sub_node_array"][(uint64_t)0]);
        REQUIRE(true == (bool)_got_json_value["node_object"]["sub_node_array"][(uint64_t)1]);
        REQUIRE(46.64  == (double)_got_json_value["node_object"]["sub_node_array"][(uint64_t)2]);
        REQUIRE_THAT( ((std::string)_got_json_value["node_object"]["sub_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "sub array: fu*k your life." ));
        REQUIRE_THAT(  ((std::string)_got_json_value["node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "sub array: fu*k your life." ));
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _got_json_value["node_object"]["sub_node_array"][(uint64_t)5].GetType());

    }

    SECTION("test int yJsonSetJsonObject(yJsonValue & obj_val_)") {

        yLib::yJsonValue _tmp_json_obj(yLib::yJsonValue::OBJECT_TYPE);
        yLib::yJson _json;

        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _tmp_json_obj.GetType());

        _tmp_json_obj["test_node_int"] = (int64_t)-12345;
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _tmp_json_obj["test_node_int"].GetType());
        REQUIRE(-12345 == (int64_t)_tmp_json_obj["test_node_int"]);

        _tmp_json_obj["test_node_bool"] = true;
        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _tmp_json_obj["test_node_bool"].GetType());
        REQUIRE(true == (bool)_tmp_json_obj["test_node_bool"]);

        _tmp_json_obj["test_node_double"] = (double)-12345.12345;
        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _tmp_json_obj["test_node_double"].GetType());
        REQUIRE(-12345.12345 == (double)_tmp_json_obj["test_node_double"]);

        _tmp_json_obj["test_node_stdstring"] = "test node stdstring ...";
        REQUIRE(yLib::yJsonValue::STRING_TYPE == _tmp_json_obj["test_node_stdstring"].GetType());
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_stdstring"]).c_str(), Catch::Equals ( "test node stdstring ..." ));

        _tmp_json_obj["test_node_null"] = yLib::yJsonValue();
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_null"].GetType());

        _tmp_json_obj["test_node_array"] = yLib::yJsonValue(yLib::yJsonValue::ARRAY_TYPE);
        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _tmp_json_obj["test_node_array"].GetType());

        _tmp_json_obj["test_node_array"][(uint64_t)0] = (int64_t)-123654;
        REQUIRE(-123654 == (int64_t)_tmp_json_obj["test_node_array"][(uint64_t)0]);

        _tmp_json_obj["test_node_array"][(uint64_t)1] = false;
        REQUIRE(false == (bool)_tmp_json_obj["test_node_array"][(uint64_t)1]);

        _tmp_json_obj["test_node_array"][(uint64_t)2] = -12.3654;
        REQUIRE(-12.3654  == (double)_tmp_json_obj["test_node_array"][(uint64_t)2]);

        _tmp_json_obj["test_node_array"][(uint64_t)3] = "test node array id 3";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "test node array id 3" ));

        _tmp_json_obj["test_node_array"][(uint64_t)4] = yLib::yJsonValue(yLib::yJsonValue::OBJECT_TYPE);
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == (int64_t)_tmp_json_obj["test_node_array"][(uint64_t)4].GetType());

        _tmp_json_obj["test_node_array"][(uint64_t)4]["node_array_obj"] = "test node array id 4";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "test node array id 4" ));

        _tmp_json_obj["test_node_array"][(uint64_t)5] = yLib::yJsonValue();
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_array"][(uint64_t)5].GetType());

//node_object
        _tmp_json_obj["test_node_object"] = yLib::yJsonValue(yLib::yJsonValue::OBJECT_TYPE);
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == (int64_t)_tmp_json_obj["test_node_object"].GetType());

        _tmp_json_obj["test_node_object"]["sub_node_int"] = (int64_t)-12345;
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _tmp_json_obj["test_node_object"]["sub_node_int"].GetType());
        REQUIRE(-12345 == (int64_t)_tmp_json_obj["test_node_object"]["sub_node_int"]);

        _tmp_json_obj["test_node_object"]["sub_node_bool"] = true;
        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_bool"].GetType());
        REQUIRE(true == (bool)_tmp_json_obj["test_node_object"]["sub_node_bool"]);

        _tmp_json_obj["test_node_object"]["sub_node_double"] = 654.123;
        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _tmp_json_obj["test_node_object"]["sub_node_double"].GetType());
        REQUIRE(654.123 == (double)_tmp_json_obj["test_node_object"]["sub_node_double"]);

        _tmp_json_obj["test_node_object"]["sub_node_stdstring"] = "sub: fu*k your life.";
        REQUIRE(yLib::yJsonValue::STRING_TYPE == _tmp_json_obj["test_node_object"]["sub_node_stdstring"].GetType());
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_stdstring"]).c_str(), Catch::Equals ( "sub: fu*k your life." ));

        _tmp_json_obj["test_node_object"]["sub_node_null"] = yLib::yJsonValue(yLib::yJsonValue::NULL_TYPE);
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_null"].GetType());

        _tmp_json_obj["test_node_object"]["sub_node_array"] = yLib::yJsonValue(yLib::yJsonValue::ARRAY_TYPE);
        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"].GetType());

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)0] = (int64_t)123978;
        REQUIRE(123978 == (int64_t)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)0]);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)1] = true;
        REQUIRE(true == (bool)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)1]);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)2] = 12.456;
        REQUIRE(12.456  == (double)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)2]);

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)3] = "sub array: fu*k your life. id 3";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "sub array: fu*k your life. id 3" ));

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4] = yLib::yJsonValue(yLib::yJsonValue::OBJECT_TYPE);
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4].GetType());

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"] = "sub array: fu*k your life. id 4";
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "sub array: fu*k your life. id 4" ));

        _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)5] = yLib::yJsonValue(yLib::yJsonValue::NULL_TYPE);
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)5].GetType());

        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _tmp_json_obj.GetType());

        REQUIRE(0 == _json.SetRootObject(_tmp_json_obj));

        REQUIRE(0 == _json.WriteFile("test_tmp.json"));
    }

    SECTION("Re-test int yJsonSetJsonObject(yJsonValue & obj_val_) to check write is ready") {

        yLib::yJson _json;
        
        REQUIRE(0 == _json.ReadFile("test_tmp.json"));

        yLib::yJsonValue _tmp_json_obj = _json.GetRootObject();
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _tmp_json_obj.GetType());

        //test value 
        REQUIRE(yLib::yJsonValue::INT64_TYPE == _tmp_json_obj["test_node_int"].GetType());
        REQUIRE(-12345 == (int64_t)_tmp_json_obj["test_node_int"]);

        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _tmp_json_obj["test_node_bool"].GetType());
        REQUIRE(true == (bool)_tmp_json_obj["test_node_bool"]);

        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _tmp_json_obj["test_node_double"].GetType());
        REQUIRE(-12345.12345 == (double)_tmp_json_obj["test_node_double"]);

        REQUIRE(yLib::yJsonValue::STRING_TYPE == _tmp_json_obj["test_node_stdstring"].GetType());
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_stdstring"]).c_str(), Catch::Equals ( "test node stdstring ..." ));

        //test null
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_null"].GetType());

        //test array
        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _tmp_json_obj["test_node_array"].GetType());

        //test array 0
        REQUIRE(-123654 == (int64_t)_tmp_json_obj["test_node_array"][(uint64_t)0]);

        //test array 1
        REQUIRE(false == (bool)_tmp_json_obj["test_node_array"][(uint64_t)1]);

        //test array 2
        REQUIRE(-12.3654  == (double)_tmp_json_obj["test_node_array"][(uint64_t)2]);

        //test array 3
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "test node array id 3" ));

        //test array 4
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == (int64_t)_tmp_json_obj["test_node_array"][(uint64_t)4].GetType());

        //test array 4
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "test node array id 4" ));

        //test array 5
        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_array"][(uint64_t)5].GetType());

//node_object
        //test object
        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == (int64_t)_tmp_json_obj["test_node_object"].GetType());

        REQUIRE(yLib::yJsonValue::INT64_TYPE == _tmp_json_obj["test_node_object"]["sub_node_int"].GetType());
        REQUIRE(-12345 == (int64_t)_tmp_json_obj["test_node_object"]["sub_node_int"]);

        REQUIRE(yLib::yJsonValue::BOOL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_bool"].GetType());
        REQUIRE(true == (bool)_tmp_json_obj["test_node_object"]["sub_node_bool"]);

        REQUIRE(yLib::yJsonValue::DOUBLE_TYPE == _tmp_json_obj["test_node_object"]["sub_node_double"].GetType());
        REQUIRE(654.123 == (double)_tmp_json_obj["test_node_object"]["sub_node_double"]);

        REQUIRE(yLib::yJsonValue::STRING_TYPE == _tmp_json_obj["test_node_object"]["sub_node_stdstring"].GetType());
        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_stdstring"]).c_str(), Catch::Equals ( "sub: fu*k your life." ));

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_null"].GetType());

        //test object-array
        REQUIRE(yLib::yJsonValue::ARRAY_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"].GetType());

        REQUIRE(123978 == (int64_t)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)0]);

        REQUIRE(true == (bool)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)1]);

        REQUIRE(12.456  == (double)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)2]);

        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)3]).c_str(), Catch::Equals ( "sub array: fu*k your life. id 3" ));

        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4].GetType());

        REQUIRE_THAT( ((std::string)_tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"]).c_str(), Catch::Equals ( "sub array: fu*k your life. id 4" ));

        REQUIRE(yLib::yJsonValue::NULL_TYPE == _tmp_json_obj["test_node_object"]["sub_node_array"][(uint64_t)5].GetType());

        REQUIRE(yLib::yJsonValue::OBJECT_TYPE == _tmp_json_obj.GetType());

        REQUIRE(0 == _json.SetRootObject(_tmp_json_obj));

        REQUIRE(0 == _json.WriteFile("test_tmp.json"));
    }
}

