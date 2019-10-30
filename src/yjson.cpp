/*
 * @Author: Sky
 * @Date: 2019-10-28 14:15:15
 * @LastEditors: Sky
 * @LastEditTime: 2019-10-30 09:25:33
 * @Description: 
 */

#include <yjson.hpp>

yLib::yJson::yJson() MACRO_INIT_YOBJECT_PROPERTY(yJson){

    
}

yLib::yJson::~yJson(){

    _json_root_value = nullptr;
}


int yLib::yJson::yJsonReadFile(std::string file){

    return 0;
}
int yLib::yJson::yJsonReadMemory(const uint8_t * mem_addr, uint64_t mem_size){

    return 0;
}
int yLib::yJson::yJsonWriteFile(std::string file){

    return 0;   
}
int yLib::yJson::yJsonWriteMemory(const uint8_t * mem_addr, uint64_t mem_size){

    return 0;
}


yLib::yJsonValue::yJsonValue() MACRO_INIT_YOBJECT_PROPERTY(yJsonValue){

    
}

yLib::yJsonValue::~yJsonValue(){

    _json_value = nullptr;
}

yLib::yJsonValue::operator int64_t(){

    return _int64_value;
}
yLib::yJsonValue::operator bool(){

    return _boolean_value;
}
yLib::yJsonValue::operator double(){

    return _double_value;
}
yLib::yJsonValue::operator std::string(){

    return _stdstring_value;
}