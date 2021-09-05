/*
 * @Author: Sky
 * @Date: 2019-10-28 14:15:15
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 15:00:39
 * @Description: 
 */

#include "utility/yjson.hpp"

#include <json/json.h>

#include <fstream>

yLib::yJson::yJson() noexcept 
MACRO_INIT_YOBJECT_PROPERTY(yJson),
json_reader(nullptr),
json_reader_builder(nullptr),
json_writer(nullptr),
json_writer_builder(nullptr),
json_root_value(nullptr)
{
    //write
    json_writer_builder = new Json::StreamWriterBuilder();
    json_writer = static_cast<Json::StreamWriterBuilder*>(json_writer_builder)->newStreamWriter();

    //copy
    json_reader_builder = new Json::CharReaderBuilder();
    json_reader = static_cast<Json::CharReaderBuilder*>(json_reader_builder)->newCharReader();
    
    json_root_value = new Json::Value(Json::ValueType::nullValue);
}

yLib::yJson::~yJson(){

    //write
    if ( nullptr != json_writer_builder ){

        delete static_cast<Json::StreamWriterBuilder*>(json_writer_builder);
        json_writer_builder = nullptr;
    }
    if (nullptr != json_writer){

        delete static_cast<Json::StreamWriter*>(json_writer);
        json_writer = nullptr;
    }

    //read
    if ( nullptr != json_reader_builder ){

        delete static_cast<Json::CharReaderBuilder*>(json_reader_builder);
        json_reader_builder = nullptr;
    }
    if (nullptr != json_reader){

        delete static_cast<Json::CharReader*>(json_reader);
        json_reader = nullptr;
    }


    if ( nullptr != json_root_value ){

        delete static_cast<Json::Value*>(json_root_value);
        json_root_value = nullptr;
    }
    
}


int8_t yLib::yJson::ReadFile(const std::string &file_path_){

    //reset settings_
    static_cast<Json::CharReaderBuilder*>(json_reader_builder)->setDefaults(&static_cast<Json::CharReaderBuilder*>(json_reader_builder)->settings_);

    std::ifstream _in_file;
    
    _in_file.open(file_path_);

    if ( _in_file.is_open() ){

        _in_file.seekg(0, _in_file.end);
        int64_t _file_len = _in_file.tellg();
        _in_file.seekg(0, _in_file.beg);

        char * _json_str_buf = nullptr;
        try
        {
            _json_str_buf = new char[_file_len];
        }
        catch(const std::exception& e)
        {
            yLib::yLog::E("Allocate memory failed.please check the usage of memory");
            _in_file.close();
            return -1;
        }

        _in_file.read(_json_str_buf, _file_len);

        _in_file.close();
        
        std::string _parse_error = "";
        //JSONCPP_STRING parse_error = "";
        //begin of buffer, end of buffer + len
        bool _parse_ret = static_cast<Json::CharReader*>(json_reader)->parse(_json_str_buf, \
            _json_str_buf + _file_len, static_cast<Json::Value*>(json_root_value), &_parse_error);
        
        delete [] _json_str_buf;//delete mem
        _json_str_buf = nullptr;

        if (!_parse_ret){

            yLib::yLog::E("parse json str error, error info is %s, file read len is %d", _parse_error.c_str(), _file_len);
            return -1;
        }
    }
    else{
        
        yLib::yLog::E("yJsonReadFile(): open file failed.");
        return -1;
    }

    return 0;
}
int8_t yLib::yJson::ReadMemory(const int8_t * addr_, uint64_t size_){
    
    static_cast<Json::CharReaderBuilder*>(json_reader_builder)->setDefaults(&static_cast<Json::CharReaderBuilder*>(json_reader_builder)->settings_);

    std::string _parse_error = "";
    bool _parse_ret = static_cast<Json::CharReader*>(json_reader)->parse( \
        reinterpret_cast<const char *>(addr_), \
        reinterpret_cast<const char *>(addr_) + size_, static_cast<Json::Value*>(json_root_value), &_parse_error);
    if ( !_parse_ret ){

        yLib::yLog::E("parse json str error, error info is %s", _parse_error.c_str());
        return -1;
    }
    return 0;
}


int8_t yLib::yJson::WriteFile(const std::string &file_path_){

    std::ofstream _out_file;

    _out_file.open(file_path_);
    if (!_out_file.is_open()){

        yLib::yLog::E("WriteFile: file open failed.");
        return -1;
    }
    if ( 0 > static_cast<Json::StreamWriter*>(json_writer)->write(*static_cast<Json::Value*>(json_root_value), &_out_file) ){

        yLib::yLog::E("WriteFile: file write failed.");
        return -1;
    }

    _out_file.close();
    return 0;   
}
int8_t yLib::yJson::WriteMemory(int8_t * addr_, uint64_t max_size_){

    std::string _json_str = static_cast<Json::Value*>(json_root_value)->toStyledString();

    int _json_str_len = _json_str.length();

    if ( max_size_ - 1 <= _json_str_len ){

        yLib::yLog::E("input mem's size - 1 is less than _json_str_len");
        return -1;
    }

    memset((void *)addr_, 0, max_size_);
    memcpy((void *)addr_, _json_str.c_str(), _json_str_len);
    return 0;
}


yLib::yJsonValue yLib::yJson::GetRootObject(void){

    if ( static_cast<Json::Value*>(json_root_value)->type() == Json::ValueType::objectValue ){

        yLib::yJsonValue _tmp_val(yLib::yValue::OBJECT_TYPE);

        //deep copy a Json::Value object
        delete static_cast<Json::Value*>(_tmp_val.json_value);
        _tmp_val.json_value = nullptr;

        //reconstruct a obj using deepcopy
        _tmp_val.json_value = new Json::Value(*static_cast<Json::Value*>(json_root_value));

        //deep copy a Json::Value object
        _tmp_val.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_tmp_val.json_value);

        return _tmp_val;
    }
    else{

        return yLib::yJsonValue(yValue::NULL_TYPE);
    }
}

int8_t yLib::yJson::SetRootObject(const yJsonValue & value_){

    if (Json::ValueType::objectValue != static_cast<Json::Value*>((void*)value_.value_containter.ptr_val)->type()){

        yLib::yLog::E("Invalid json object.");
        return -1;
    }
    //deep copy a Json::Value object
    delete static_cast<Json::Value*>(json_root_value);
    json_root_value = nullptr;

    //deep copy a Json::Value object
    //reconstruct a obj using deepcopy
    json_root_value = new Json::Value(*static_cast<Json::Value*>((void*)value_.value_containter.ptr_val));

    return 0;
}


//////////////////////////////////////
//////////////////////////////////////
/////////////yJsonValue
//////////////////////////////////////
//////////////////////////////////////
#define COVERT_POINTER_TO_JSONVALUE_PTR(PTR)\
    static_cast<Json::Value*>(PTR)

static yLib::yJsonValue g_yjson_value_null_reference_obj;

yLib::yJsonValue::yJsonValue() noexcept
:yBasicValue(NULL_TYPE, nullptr),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{

    cur_value_type = yValueType::NULL_TYPE;//repeat set val

    json_value = new Json::Value();
}

yLib::yJsonValue::yJsonValue(int64_t value_) noexcept 
:yBasicValue(value_),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{


    cur_value_type = yValueType::INT64_TYPE;

    json_value = new Json::Value(value_);
}

yLib::yJsonValue::yJsonValue(uint64_t value_) noexcept 
:yBasicValue(value_),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{


    cur_value_type = yValueType::UINT64_TYPE;

    json_value = new Json::Value(value_);
}


yLib::yJsonValue::yJsonValue(bool value_) noexcept 
:yBasicValue(value_),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{

    cur_value_type = yValueType::BOOL_TYPE;

    json_value = new Json::Value(value_);
}

yLib::yJsonValue::yJsonValue(double value_) noexcept 
:yBasicValue(value_),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{

    cur_value_type = yValueType::DOUBLE_TYPE;

    json_value = new Json::Value(value_);
}

yLib::yJsonValue::yJsonValue(std::string value_) noexcept 
:yBasicValue(value_),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{

    cur_value_type = yValueType::STRING_TYPE;

    json_value = new Json::Value(value_);
}

yLib::yJsonValue::yJsonValue(yValueType value_type_) noexcept 
:yBasicValue(value_type_, nullptr),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{

    cur_value_type = value_type_;

    switch (value_type_)
    {
        case yValueType::OBJECT_TYPE:{
            
            /* code */
            json_value = new Json::Value(Json::ValueType::objectValue);
            value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
            value_holder_map = new  std::unordered_map<std::string, yJsonValue>();
            break;
        }
        case yValueType::ARRAY_TYPE:{

            /* code */
            json_value = new Json::Value(Json::ValueType::arrayValue);
            value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
            value_holder_map = new  std::unordered_map<std::string, yJsonValue>();
            break;
        }
        case yValueType::NULL_TYPE:{

            /* code */
            json_value = new Json::Value();
            value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
            break;    
        }
        default:{

            json_value = new Json::Value();
            value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
            cur_value_type = yValue::NULL_TYPE;

            yLib::yLog::E("Invalid type.");
            break;
        }
    }
}

//only used by yJsonValue
yLib::yJsonValue::yJsonValue(bool build_special_, void * ptr_) noexcept
:yBasicValue(),
json_value(nullptr),
is_special_obj(true),
value_holder_map(nullptr)
{

}

yLib::yJsonValue::yJsonValue(const yLib::yJsonValue & value_) noexcept 
:yBasicValue(),//the base obj is null-obj
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{
    CleanAllToDefault();//must be clean first,this op will modify type and value-container

    cur_value_type = value_.cur_value_type;
    is_special_obj = value_.is_special_obj;

    //sepcial object, not deep copy
    if (value_.is_special_obj){

        json_value = value_.json_value;
        value_holder_map = value_.value_holder_map;
    }
    else{//not special obj, deep copy
    
        json_value = new Json::Value(*static_cast<Json::Value*>(value_.json_value));//create json::value

        //for copy object-type array-type
        if ( nullptr != value_.value_holder_map && \
            (value_.cur_value_type == yJsonValue::OBJECT_TYPE || value_.cur_value_type == yJsonValue::ARRAY_TYPE)){
                
                //deep copy
                value_holder_map = new std::unordered_map<std::string, yJsonValue>(*value_.value_holder_map);
        }
    }

    if (cur_value_type == yJsonValue::OBJECT_TYPE){ 

        //update special value
        value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
    }
    else if (cur_value_type == yJsonValue::NULL_TYPE){

        //update special value
        value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
    }
    else if (cur_value_type == yJsonValue::ARRAY_TYPE){

        //update special value
        value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
    }
    else{

        CopyValueContainer(value_, *this);
    }
    
}

yLib::yJsonValue::yJsonValue(const yLib::yJsonValue && value_) noexcept 
:yBasicValue(value_),
json_value(nullptr),
is_special_obj(false),
value_holder_map(nullptr)
{
    CleanAllToDefault();//must be clean first,this op will modify type and value-container

    cur_value_type = value_.cur_value_type;
    is_special_obj = value_.is_special_obj;

    //sepcial object, not deep copy
    if (value_.is_special_obj){

        json_value = value_.json_value;
        value_holder_map = value_.value_holder_map;
    }
    else{//not special obj, deep copy
    
        json_value = new Json::Value(*static_cast<Json::Value*>(value_.json_value));//create json::value

        //for copy object-type array-type
        if ( nullptr != value_.value_holder_map && \
            (value_.cur_value_type == yJsonValue::OBJECT_TYPE || value_.cur_value_type == yJsonValue::ARRAY_TYPE)){
                
                //deep copy
                value_holder_map = new std::unordered_map<std::string, yJsonValue>(*value_.value_holder_map);
        }
    }

    
    if (cur_value_type == yJsonValue::OBJECT_TYPE){ 

        //update special value
        value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
    }
    else if (cur_value_type == yJsonValue::NULL_TYPE){

        //update special value
        value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
    }
    else if (cur_value_type == yJsonValue::ARRAY_TYPE){

        //update special value
        value_containter.ptr_val = reinterpret_cast<uintptr_t>(json_value);
    }
    else{

        CopyValueContainer(value_, *this);
    }
}

yLib::yJsonValue & yLib::yJsonValue::operator =(const yLib::yJsonValue & value_) noexcept
{
    //copy map
    if (yJsonValue::OBJECT_TYPE == value_.cur_value_type || yJsonValue::ARRAY_TYPE == value_.cur_value_type){

        if (nullptr == value_holder_map)
            value_holder_map = new std::unordered_map<std::string, yJsonValue>();

        value_holder_map->swap(*value_.value_holder_map);
    }

    //copy json_value
    *static_cast<Json::Value*>(json_value) = *static_cast<Json::Value*>(value_.json_value);

    CleanAllToDefault();
    CopyValueContainer(value_, *this);

    cur_value_type = value_.cur_value_type;

    //Note: we can not modified the is_special_obj, we should keep this property
    // is_special_obj
    return *this;
}

yLib::yJsonValue & yLib::yJsonValue::operator =(const yLib::yJsonValue && value_) noexcept{
    
    //copy map
    if (yJsonValue::OBJECT_TYPE == value_.cur_value_type || yJsonValue::ARRAY_TYPE == value_.cur_value_type){

        if (nullptr == value_holder_map)
            value_holder_map = new std::unordered_map<std::string, yJsonValue>();

        value_holder_map->swap(*value_.value_holder_map);
    }

    //copy json_value
    *static_cast<Json::Value*>(json_value) = *static_cast<Json::Value*>(value_.json_value);

    CleanAllToDefault();
    CopyValueContainer(value_, *this);

    cur_value_type = value_.cur_value_type;

    //Note: we can not modified the is_special_obj, we should keep this property
    return *this;
}

yLib::yJsonValue::~yJsonValue(){

    if (!is_special_obj){

        //if it is special obj, we can not delete value_holder_map, it managered by the first yJsonValue.
        if (nullptr != value_holder_map){

            delete value_holder_map;
            value_holder_map = nullptr;
        }

        //if it is special obj, we can not delete json_value, it managered by the Json::Value.
        if(nullptr != json_value){

            delete static_cast<Json::Value*>(json_value);
            json_value = nullptr;
        }
    }

    //set pointer to default-val
    value_holder_map = nullptr;
    json_value = nullptr;
    CleanAllToDefault();
}


yLib::yJsonValue::operator int64_t(){

    if ( COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isInt64() \
        || COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isInt()){


        return COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->asInt64();
    }
    // is not int64
    return -1;//Default error value;
}

yLib::yJsonValue::operator uint64_t(){

    if ( COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isUInt64() \
        || COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isUInt()){


        return COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->asUInt64();
    }
    // is not uint64
    return -1;//Default error value;
}


yLib::yJsonValue::operator bool(){

    if ( COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isBool()){


        return COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->asBool();
    }
    // is not bool
    return false;//Default error value;
}
yLib::yJsonValue::operator double(){

    if ( COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isDouble()){


        return COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->asDouble();
    }
    // is not bool
    return -1.f;//Default error value;
}
yLib::yJsonValue::operator std::string(){

    if ( COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isString()){


        return COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->asString();
    }
    // is not bool
    return "";//Default error value;
}

yLib::yJsonValue & yLib::yJsonValue::operator [](const char *key_){

    if(cur_value_type != yValue::OBJECT_TYPE){

        yLib::yLog::E("yLib::yJsonValue::operator [](const char *key_( = %s)) error: requires a array value( = %d)", key_, cur_value_type);
        throw "in yLib::yJsonValue::operator [](const char *key_): requires a object value.";
    }

    Json::Value * _find_result = nullptr;
    try
    {

        //create a null member if it does not exist
        //COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isMember(key_)  check if the key is exsited.
        //if the key_ is invalid, this op will throw a exception.
        _find_result = &COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->operator[](key_);
    }
    catch(const Json::LogicError & e)//is not object type
    {
        yLib::yLog::E("yLib::yJsonValue::operator [](const char * key_str) error: %s", e.what());
        return g_yjson_value_null_reference_obj;//null value
    }

    std::unordered_map<std::string, yJsonValue>::iterator _iter = value_holder_map->find(std::to_string((uintptr_t)_find_result));
    if ( value_holder_map->end() != _iter){//found key

        return _iter->second;//return exsited obj
    }

    //build a special obj
    yLib::yJsonValue _value(true, nullptr);

    _value.json_value = _find_result;

    _value.value_holder_map = value_holder_map;//we don't deep copy this map, because it managerd by first yJsonValue(OBJECT_TYPE)
    
    switch (_find_result->type())
    {
        case Json::ValueType::uintValue:{

            _value.cur_value_type = yLib::yJsonValue::UINT64_TYPE;
            _value.value_containter.uint64_val = _find_result->asUInt64();
            break;
        }
        case Json::ValueType::intValue:{
        
            _value.cur_value_type = yLib::yJsonValue::INT64_TYPE;
            _value.value_containter.int64_val = _find_result->asInt64();
            break;
        }

        case Json::ValueType::booleanValue :{
        
            _value.cur_value_type = yLib::yJsonValue::BOOL_TYPE;
            _value.value_containter.bool_val = _find_result->asBool();
            break;
        }

        case Json::ValueType::realValue :{
        
            _value.cur_value_type = yLib::yJsonValue::DOUBLE_TYPE;
            _value.value_containter.double_val = _find_result->asDouble();
            break;
        }

        case Json::ValueType::stringValue :{
        
            _value.cur_value_type = yLib::yJsonValue::STRING_TYPE;
            _value.value_containter.string_val = _find_result->asString();
            break;
        }

        case Json::ValueType::arrayValue :{
        
            _value.cur_value_type = yLib::yJsonValue::ARRAY_TYPE;
            _value.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_find_result);
            break;
        }

        case Json::ValueType::objectValue :{
        
            _value.cur_value_type = yLib::yJsonValue::OBJECT_TYPE;
            _value.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_find_result);
            break;
        }

        case Json::ValueType::nullValue :{
        
            _value.cur_value_type = yLib::yJsonValue::NULL_TYPE;
            _value.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_find_result);
            break;
        }
        
        default:{
            //do nothing
            break;
        }
    }

    std::pair<yJsonValueHolderMap::iterator, bool> _ret_pair = \
        value_holder_map->insert(std::make_pair(std::to_string((uintptr_t)_find_result), _value));//insert a new special obj to map

    return _ret_pair.first->second;
}

yLib::yJsonValue & yLib::yJsonValue::operator [](const std::string &key_){

    return operator[](key_.c_str());
}

yLib::yJsonValue & yLib::yJsonValue::operator [](uint64_t idx_){

    if(cur_value_type != yValue::ARRAY_TYPE){

        yLib::yLog::E("yLib::yJsonValue::operator [](uint64_t idx_( = %d)) error: requires a array value( = %d)", idx_, cur_value_type);
        throw "in yLib::yJsonValue::operator [](uint64_t idx_): requires a array value.";
    }

    Json::Value * _find_result = nullptr;
    try
    {
        //create a null member if it does not exist
        //COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->isMember(key_)  check if the key is exsited.
        //we should deep copy to avoid double free
        _find_result = &COVERT_POINTER_TO_JSONVALUE_PTR(json_value)->operator[]((Json::ArrayIndex)idx_);
    }
    catch(const Json::LogicError & e)//is not array type
    {
        yLib::yLog::E("yLib::yJsonValue::operator [](uint64_t elment_idx) error: %s", e.what());
        return g_yjson_value_null_reference_obj;//null value
    }

    std::unordered_map<std::string, yJsonValue>::iterator _iter = value_holder_map->find(std::to_string((uintptr_t)_find_result));
    if ( value_holder_map->end() != _iter){//found key

        return _iter->second;//return exsited obj
    }

    //build a special obj
    yLib::yJsonValue _value(true, nullptr);

    _value.json_value = _find_result;

    //we don't deep copy this map, because it managerd by first yJsonValue(OBJECT_TYPE) in all special obj 
    _value.value_holder_map = value_holder_map;

    switch (_find_result->type())
    {
        case Json::ValueType::uintValue:{

            _value.cur_value_type = yLib::yJsonValue::UINT64_TYPE;
            _value.value_containter.uint64_val = _find_result->asUInt64();
            break;
        }
        case Json::ValueType::intValue:{
        
            _value.cur_value_type = yLib::yJsonValue::INT64_TYPE;
            _value.value_containter.int64_val = _find_result->asInt64();
            break;
        }

        case Json::ValueType::booleanValue :{
        
            _value.cur_value_type = yLib::yJsonValue::BOOL_TYPE;
            _value.value_containter.bool_val = _find_result->asBool();
            break;
        }

        case Json::ValueType::realValue :{
        
            _value.cur_value_type = yLib::yJsonValue::DOUBLE_TYPE;
            _value.value_containter.double_val = _find_result->asDouble();
            break;
        }

        case Json::ValueType::stringValue :{
        
            _value.cur_value_type = yLib::yJsonValue::STRING_TYPE;
            _value.value_containter.string_val = _find_result->asString();
            break;
        }

        case Json::ValueType::arrayValue :{
        
            _value.cur_value_type = yLib::yJsonValue::ARRAY_TYPE;
            _value.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_find_result);
            break;
        }

        case Json::ValueType::objectValue :{
        
            _value.cur_value_type = yLib::yJsonValue::OBJECT_TYPE;
            _value.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_find_result);
            break;
        }

        case Json::ValueType::nullValue :{
        
            _value.cur_value_type = yLib::yJsonValue::NULL_TYPE;
            _value.value_containter.ptr_val = reinterpret_cast<uintptr_t>(_find_result);
            break;
        }
        
        default:{
            //do nothing
            break;
        }
    }

    std::pair<yJsonValueHolderMap::iterator, bool> _ret_pair = \
        value_holder_map->insert(std::make_pair(std::to_string((uintptr_t)_find_result), _value));//insert a new special obj to map

    //For Debuger
    yJsonValue & _dbg_ptr = _ret_pair.first->second;
    return _ret_pair.first->second;
}


yLib::yJsonValue & yLib::yJsonValue::operator=(int64_t value_)noexcept{

    value_containter.int64_val = value_;
    cur_value_type = yValue::INT64_TYPE;
    *COVERT_POINTER_TO_JSONVALUE_PTR(json_value) = Json::Value(value_);
    return *this;
}

yLib::yJsonValue & yLib::yJsonValue::operator=(uint64_t value_)noexcept{

    value_containter.uint64_val = value_;
    cur_value_type = yValue::UINT64_TYPE;
    *COVERT_POINTER_TO_JSONVALUE_PTR(json_value) = Json::Value(value_);

    return *this;
}

yLib::yJsonValue & yLib::yJsonValue::operator=(bool value_)noexcept{

    value_containter.bool_val = value_;
    cur_value_type = yValue::BOOL_TYPE;
    *COVERT_POINTER_TO_JSONVALUE_PTR(json_value) = Json::Value(value_);

    return *this;
}

yLib::yJsonValue & yLib::yJsonValue::operator=(double value_) noexcept{

    value_containter.double_val = value_;
    cur_value_type = yValue::DOUBLE_TYPE;
    *COVERT_POINTER_TO_JSONVALUE_PTR(json_value) = Json::Value(value_);

    return *this;
}


yLib::yJsonValue & yLib::yJsonValue::operator=(const std::string & value_) noexcept{

    return operator=(value_.c_str());
}

/**
 * @fn  yJsonValue & operator=(const char *value_) noexcept
 * @brief assignment func
 * @param value_ a exsited obj
 * @return the obj's reference of yJsonValue
 */        
yLib::yJsonValue & yLib::yJsonValue::operator=(const char *value_) noexcept
{

    value_containter.string_val = value_;
    cur_value_type = yValue::STRING_TYPE;
    *COVERT_POINTER_TO_JSONVALUE_PTR(json_value) = Json::Value(value_);

    return *this;
}