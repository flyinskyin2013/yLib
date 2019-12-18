/*
 * @Author: Sky
 * @Date: 2019-11-20 15:22:17
 * @LastEditors: Sky
 * @LastEditTime: 2019-12-10 09:16:37
 * @Description: 
 */
#include "ylib.hpp"


void TEST_READ(void){
    yLib::yLog::I("=============================TEST_READ=============================");
    yLib::yJson test_yjson;
    
    if ( 0 > test_yjson.yJsonReadFile("test.json") ){

        yLib::yLog::E("open json file failed.");
        return ;
    }

    yLib::yJsonValue _json_value = test_yjson.yJsonGetValue();

    //json basic 
    yLib::yLog::I("node int is %d", (int64_t)_json_value["node_int"]);
    yLib::yLog::I("node bool is %s", (true == (bool)_json_value["node_bool"])?"true":"false" );
    yLib::yLog::I("node double is %f", (double)_json_value["node_double"]);
    yLib::yLog::I("node stdstring is %s", ((std::string)_json_value["node_stdstring"]).c_str());
    yLib::yLog::I("node null is %s", (_json_value["node_null"]._value_type == yLib::yJsonValue::yJsonValueType::NULL_TYPE)?"null":"not null");
    
    //json array
    yLib::yLog::I("node array[0] is %d", (int64_t)_json_value["node_array"][(uint64_t)0]);
    yLib::yLog::I("node array[1] is %s", (true == (bool)_json_value["node_array"][(uint64_t)1])?"true":"false"  );
    yLib::yLog::I("node array[2] is %f", (double)_json_value["node_array"][(uint64_t)2]);
    yLib::yLog::I("node array[3] is %s", ((std::string)_json_value["node_array"][(uint64_t)3]).c_str() );
    yLib::yLog::I("node array[4] is %s", (_json_value["node_array"][(uint64_t)4]._value_type == yLib::yJsonValue::yJsonValueType::OBJECT_TYPE)?"json object":"not json object" );
    yLib::yLog::I("node array[4] is str %s", ((std::string)_json_value["node_array"][(uint64_t)4]["node_array_obj"]).c_str() );
    yLib::yLog::I("node array[5] is %s", (_json_value["node_array"][(uint64_t)5]._value_type == yLib::yJsonValue::yJsonValueType::NULL_TYPE)?"null":"not null" );

    
    //json object
    yLib::yLog::I("node object is %s", (_json_value["node_object"]._value_type == yLib::yJsonValue::yJsonValueType::OBJECT_TYPE)?"json object":"not json object");
    yLib::yLog::I("sub node int is %d", (int64_t)_json_value["node_object"]["sub_node_int"]);
    yLib::yLog::I("sub node bool is %s", (true == (bool)_json_value["node_object"]["sub_node_bool"])?"true":"false" );
    yLib::yLog::I("sub node double is %f", (double)_json_value["node_object"]["sub_node_double"]);
    yLib::yLog::I("sub node stdstring is %s", ((std::string)_json_value["node_object"]["sub_node_stdstring"]).c_str() );
    yLib::yLog::I("sub node null is %s", (_json_value["node_object"]["sub_node_null"]._value_type == yLib::yJsonValue::yJsonValueType::NULL_TYPE)?"null":"not null");
    
    
    yLib::yLog::I("sub node array[0] is %d", (int64_t)_json_value["node_object"]["sub_node_array"][(uint64_t)0]);
    yLib::yLog::I("sub node array[1] is %s", (true == (bool)_json_value["node_object"]["sub_node_array"][(uint64_t)1])?"true":"false"  );
    yLib::yLog::I("sub node array[2] is %f", (double)_json_value["node_object"]["sub_node_array"][(uint64_t)2]);
    yLib::yLog::I("sub node array[3] is %s", ((std::string)_json_value["node_object"]["sub_node_array"][(uint64_t)3]).c_str());
    yLib::yLog::I("sub node array[4] is %s", (_json_value["node_object"]["sub_node_array"][(uint64_t)4]._value_type == yLib::yJsonValue::yJsonValueType::OBJECT_TYPE)?"json object":"not json object");
    yLib::yLog::I("sub node array[4] str is %s", ((std::string)_json_value["node_object"]["sub_node_array"][(uint64_t)4]["node_array_obj"]).c_str());
    yLib::yLog::I("sub node array[5] is %s", (_json_value["node_object"]["sub_node_array"][(uint64_t)5]._value_type == yLib::yJsonValue::yJsonValueType::NULL_TYPE)?"null":"not null" );
   
}

yLib::yJsonValue TEST_WRITE(void){

    yLib::yLog::I("==============================TEST_WRITE============================");

    yLib::yJson json;

    yLib::yJsonValue root_value;
    yLib::yJsonValue object_value(yLib::yJsonValue::yJsonValueType::OBJECT_TYPE);
    yLib::yJsonValue array_value(yLib::yJsonValue::yJsonValueType::ARRAY_TYPE);
    yLib::yJsonValue int_value((int64_t)99);
    yLib::yJsonValue double_value(99.99f);
    yLib::yJsonValue bool_value(true);
    yLib::yJsonValue string_value(std::string("Fu*k your life."));

    yLib::yJsonValue null_value;

    object_value["null"] = null_value;
    object_value["double"] = double_value;
    object_value["bool"] = bool_value;
    object_value["string"] = string_value;
    object_value["int"] = yLib::yJsonValue((int64_t)99);
    object_value["array"] = array_value;


    array_value[(uint64_t)0] = null_value;
    array_value[(uint64_t)1] = double_value;
    array_value[(uint64_t)2] = bool_value;
    array_value[(uint64_t)3] = string_value;
    array_value[(uint64_t)4] = int_value;
    array_value[(uint64_t)5] = object_value;

    root_value["root_value"] = yLib::yJsonValue(std::string("test root value"));
    root_value["null"] = null_value;
    root_value["double"] = double_value;
    root_value["bool"] = bool_value;
    root_value["string"] = string_value;
    root_value["int"] = int_value;
    root_value["object"] = object_value;
    root_value["array"] = array_value;


    json.yJsonWriteValue(root_value);
    json.yJsonWriteFile("tmp.json");

    char* _buf_tmp = new char[2048];
    
    json.yJsonWriteMemory((int8_t *)_buf_tmp, 2048);
    yLib::yLog::I("json_str to mem buf =%s", _buf_tmp);

    delete [] _buf_tmp;


    return std::move(root_value);
}
int main(int argc, char * argv[]){

    TEST_READ();
    yLib::yLog::I("==========================================================");
    yLib::yLog::I("==========================================================");
    yLib::yLog::I("==========================================================");
    


    yLib::yJsonValue val = TEST_WRITE();

    char* _buf_tmp = new char[2048];
    yLib::yJson json;

    json.yJsonWriteValue(val);
    json.yJsonWriteMemory((int8_t *)_buf_tmp, 2048);
    yLib::yLog::I("json_str in main to mem buf =%s", _buf_tmp);
    yLib::yLog::I("json_str in main to root_value =%s", std::string(val["root_value"]).c_str());
    
    delete [] _buf_tmp;
    return 0;
}