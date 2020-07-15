/*
 * @Author: Sky
 * @Date: 2019-10-28 14:15:15
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:40:01
 * @Description: 
 */

#include "utility/yjson.hpp"

#include <json/json.h>

#include <fstream>

yLib::yJson::yJson() noexcept MACRO_INIT_YOBJECT_PROPERTY(yJson){

    _json_writer_builder = new Json::StreamWriterBuilder();
    _json_reader_builder = new Json::CharReaderBuilder();
    _json_root_value = new Json::Value(Json::ValueType::nullValue);
}

yLib::yJson::~yJson(){

    if ( nullptr != _json_writer_builder ){

        delete _json_writer_builder;
    }
    _json_writer_builder = nullptr;


    if ( nullptr != _json_reader_builder ){

        delete _json_reader_builder;
    }
    _json_reader_builder = nullptr;

    if ( nullptr != _json_root_value ){

        delete _json_root_value;
    }
    _json_root_value = nullptr;
}


int yLib::yJson::yJsonReadFile(std::string file){

    _json_reader_builder->setDefaults(&_json_reader_builder->settings_);
    
    // //get a new json value
    // if (nullptr == _json_root_value){

    //     _json_root_value = new Json::Value(Json::ValueType::nullValue);
    // }
    // else
    // {
    //     delete _json_root_value;//remove exsited json value
    //     _json_root_value = new Json::Value(Json::ValueType::nullValue);
    // }
    

    
    std::ifstream in_file;
    
    in_file.open(file);

    if ( in_file.is_open() ){

        in_file.seekg(0, in_file.end);
        int64_t file_len = in_file.tellg();
        in_file.seekg(0, in_file.beg);

        _json_str_buf = new char[file_len];

        in_file.read(_json_str_buf, file_len);

        in_file.close();
        
        std::string parse_error = "";
        //JSONCPP_STRING parse_error = "";
        bool parse_ret = _json_reader_builder->newCharReader()->parse(_json_str_buf, \
            _json_str_buf + file_len - 1, _json_root_value, &parse_error);
        
        delete [] _json_str_buf;
        if ( !parse_ret ){

            yLib::yLog::E("parse json str error, error info is %s", parse_error.c_str());
            return -1;
        }
    }
    else{
        
        yLib::yLog::E("yJsonReadFile(): open file failed.");
        return -1;
    }

    return 0;
}
int yLib::yJson::yJsonReadMemory(const int8_t * mem_addr, uint64_t mem_size){
    
    _json_reader_builder->setDefaults(&_json_reader_builder->settings_);

    // //get a new json value
    // if (nullptr == _json_root_value){

    //     _json_root_value = new Json::Value(Json::ValueType::nullValue);
    // }
    // else
    // {
    //     delete _json_root_value;//remove exsited json value
    //     _json_root_value = new Json::Value(Json::ValueType::nullValue);
    // }


    std::string parse_error = "";
    bool parse_ret = _json_reader_builder->newCharReader()->parse( \
        reinterpret_cast<const char *>(mem_addr), \
        reinterpret_cast<const char *>(mem_addr) + mem_size - 1, _json_root_value, &parse_error);
    if ( !parse_ret ){

        yLib::yLog::E("parse json str error, error info is %s", parse_error.c_str());
        return -1;
    }
    return 0;
}


int yLib::yJson::yJsonWriteFile(std::string file){

    std::ofstream out_file;

    out_file.open(file);
    if ( !out_file.is_open() ){

        yLib::yLog::E("yJsonWriteFile: file open failed.");
        return -1;
    }
    if ( 0 > _json_writer_builder->newStreamWriter()->write(*_json_root_value, &out_file) ){

        yLib::yLog::E("yJsonWriteFile: file write failed.");
        return -1;
    }

    out_file.close();
    return 0;   
}
int yLib::yJson::yJsonWriteMemory(int8_t * mem_addr, uint64_t mem_max_size){

    std::string json_str = _json_root_value->toStyledString();
    
    const char * ptr_json_str = json_str.c_str();

    int json_str_len = _json_root_value->toStyledString().length();

    if ( mem_max_size - 1 <= json_str_len ){

        yLib::yLog::E("input mem's size - 1 is less than json_str_size");
        return -1;
    }

    memset((void *)mem_addr, 0, mem_max_size);
    memcpy((void *)mem_addr, ptr_json_str, json_str_len);
    return 0;
}


yLib::yJsonValue yLib::yJson::yJsonGetValue(void){

    return yJsonGetParsedJsonObject();
}

int yLib::yJson::yJsonWriteValue(yJsonValue & value){

    return yJsonSetJsonObject(value);
}



yLib::yJsonValue yLib::yJson::yJsonGetParsedJsonObject(void){
    
    // switch (_json_root_value->type())
    // {
    // case Json::ValueType::intValue:
    //     /* code */
    //     return yJsonValue((int64_t)_json_root_value->asInt());
    //     break;

    // case Json::ValueType::uintValue:
    //     /* code */
    //     return yJsonValue((uint64_t)_json_root_value->asUInt());
    //     break;
        
    // case Json::ValueType::realValue:
    //     /* code */
    //     return yJsonValue(_json_root_value->asDouble());
    //     break;
        
    // case Json::ValueType::stringValue:
    //     /* code */
    //     return yJsonValue(_json_root_value->asString());
    //     break;        
    // case Json::ValueType::booleanValue:
    //     return yJsonValue(_json_root_value->asBool());
    //     break;
    // case Json::ValueType::arrayValue:

    //     yJsonValue _tmp_val(yLib::yJsonValue::yJsonValueType::ARRAY_TYPE);
    //     *_tmp_val._json_root_value = _json_root_value
    //     break;
    // case Json::ValueType::objectValue:
    //     value_._value_type = yLib::yJsonValue::yJsonValueType::OBJECT_TYPE;
    //     break;
    // case Json::ValueType::nullValue:
    //     value_._value_type = yLib::yJsonValue::yJsonValueType::NULL_TYPE;
    //     break;
    // default:
    //     yLib::yLog::E("not support yet.");
    //     value_._value_type = yLib::yJsonValue::yJsonValueType::NULL_TYPE;
    //     break;
    // }

    if ( _json_root_value->type() == Json::ValueType::objectValue ){

        yLib::yJsonValue _tmp_val(yLib::yJsonValue::yJsonValueType::OBJECT_TYPE);
        *_tmp_val._json_root_value = *_json_root_value;
        return _tmp_val;
    }
    else{

        return yLib::yJsonValue();
    }

}
int yLib::yJson::yJsonSetJsonObject(yJsonValue & obj_val_){

    *_json_root_value = *obj_val_._json_root_value;
    return 0;
}


// yLib::yJsonValue::yJsonValue() noexcept MACRO_INIT_YOBJECT_PROPERTY(yJsonValue){

    
// }









yLib::yJsonValue::yJsonValue(int64_t value_) noexcept 
{

    _object_name = "yJsonValue";

    _value_type = yJsonValueType::INT64_TYPE;
    _cur_basic_value_type = yBasicValueType::INT64_YBASICVALUE_TYPE;
    _value_containter._n_value_int64 = value_;

    _json_root_value = new Json::Value(value_);
}
yLib::yJsonValue::yJsonValue(bool value_) noexcept 
{
    _object_name = "yJsonValue";

    _value_type = yJsonValueType::BOOL_TYPE;
    _cur_basic_value_type = yBasicValueType::BOOL_YBASICVALUE_TYPE;
    _value_containter._b_value_bool = value_;

    _json_root_value = new Json::Value(value_);
}
yLib::yJsonValue::yJsonValue(double value_) noexcept 
{

    _object_name = "yJsonValue";

    _value_type = yJsonValueType::DOUBLE_TYPE;
    _cur_basic_value_type = yBasicValueType::DOUBLE_YBASICVALUE_TYPE;
    _value_containter._f_value_double = value_;

    _json_root_value = new Json::Value(value_);
}
yLib::yJsonValue::yJsonValue(std::string stdstr_) noexcept 
{

    _object_name = "yJsonValue";

    _value_type = yJsonValueType::STRING_TYPE;
    _cur_basic_value_type = yBasicValueType::STRING_YBASICVALUE_TYPE;
    _value_containter._str_value_string = stdstr_;

    _json_root_value = new Json::Value(stdstr_);
}

yLib::yJsonValue::yJsonValue(yJsonValueType value_type_) noexcept 
{

    _object_name = "yJsonValue";

    _value_type = value_type_;

    switch (value_type_)
    {
    case yJsonValueType::OBJECT_TYPE:
        /* code */
        _json_root_value = new Json::Value(Json::ValueType::objectValue);
        break;

    case yJsonValueType::ARRAY_TYPE:
        /* code */
        _json_root_value = new Json::Value(Json::ValueType::arrayValue);
        break;

    case yJsonValueType::NULL_TYPE:
        /* code */
        _json_root_value = new Json::Value();
        break;    
    default:
        _json_root_value = new Json::Value();
        yLib::yLog::E("not support yet.");
        break;
    }
    
    
}

yLib::yJsonValue::yJsonValue(const yLib::yJsonValue & value_) noexcept 
{

    _object_name = "yJsonValue";

    _value_type = value_._value_type;
    _cur_basic_value_type = value_._cur_basic_value_type;

    switch (_value_type)
    {
    case yJsonValueType::INT64_TYPE:
        /* code */
        _value_containter._n_value_int64 = value_._value_containter._n_value_int64;
        break;
    case yJsonValueType::DOUBLE_TYPE:
        _value_containter._f_value_double = value_._value_containter._f_value_double;
        break;
    case yJsonValueType::BOOL_TYPE:
        _value_containter._b_value_bool = value_._value_containter._b_value_bool;
        break;
    case yJsonValueType::STRING_TYPE:
        _value_containter._str_value_string = value_._value_containter._str_value_string;
        break;
    case yJsonValueType::OBJECT_TYPE:
        break;
    case yJsonValueType::ARRAY_TYPE:
        break;
    case yJsonValueType::NULL_TYPE:
        break;
    default:
        break;
    }


    if ( value_._json_root_value ==  value_._json_root_value_bak){//only call by yLib::yJsonValue::operator[]

        _json_root_value = value_._json_root_value_bak;
    }
    else{

        _json_root_value = new Json::Value();//new a null json object
        *_json_root_value = *value_._json_root_value;
    }
}
yLib::yJsonValue::yJsonValue(yLib::yJsonValue && value_) noexcept 
{

    _object_name = "yJsonValue";


    _value_type = value_._value_type;
    _cur_basic_value_type = value_._cur_basic_value_type;

    switch (_value_type)
    {
    case yJsonValueType::INT64_TYPE:
        /* code */
        _value_containter._n_value_int64 = value_._value_containter._n_value_int64;
        break;
    case yJsonValueType::DOUBLE_TYPE:
        _value_containter._f_value_double = value_._value_containter._f_value_double;
        break;
    case yJsonValueType::BOOL_TYPE:
        _value_containter._b_value_bool = value_._value_containter._b_value_bool;
        break;
    case yJsonValueType::STRING_TYPE:
        _value_containter._str_value_string = value_._value_containter._str_value_string;
        break;
    case yJsonValueType::OBJECT_TYPE:
        break;
    case yJsonValueType::ARRAY_TYPE:
        break;
    case yJsonValueType::NULL_TYPE:
        break;
    default:
        break;
    }
    
    if ( value_._json_root_value == value_._json_root_value_bak){//only call by yLib::yJsonValue::operator[]

        _json_root_value = value_._json_root_value_bak;
    }
    else{

        _json_root_value = new Json::Value();//new a null json object
        *_json_root_value = *value_._json_root_value;
    }
}

yLib::yJsonValue::~yJsonValue(){

    _json_root_value = nullptr;
    
    if ( _json_root_value == _json_root_value_bak ){

    }
    else{

        delete _json_root_value;
        _json_root_value = nullptr;
        _json_root_value_bak = nullptr;
    }

}

/*
yLib::yJsonValue::operator int64_t(){

    if ( _json_root_value->isInt64() \
        || _json_root_value->isInt() \
        || _json_root_value->isIntegral() \
        || _json_root_value->isUInt64() \
        || _json_root_value->isUInt()){


        _int64_value = _json_root_value->asInt64();
        _value_type = yJsonValueType::INT64_TYPE;
    }
    else{// is not int64

        _value_type = yJsonValueType::ERROR_TYPE;
        _int64_value = 0;
        return -1;//Default error value;
    }
    return _int64_value;
}
yLib::yJsonValue::operator bool(){

    if ( _json_root_value->isBool() ){

        _boolean_value = _json_root_value->asBool();
        _value_type = yJsonValueType::BOOL_TYPE;
    }
    else
    {
        _value_type = yJsonValueType::ERROR_TYPE;
        _boolean_value = false;
        return false;//Default error value;
    }
    
    return _boolean_value;
}
yLib::yJsonValue::operator double(){

    if ( _json_root_value->isDouble() ){

        _double_value = _json_root_value->asDouble();
        _value_type = yJsonValueType::DOUBLE_TYPE;
    }
    else
    {
        _value_type = yJsonValueType::ERROR_TYPE;
        _double_value = 0.0f;
        return -1.0f; //Default error value;
    }
    
    return _double_value;
}
yLib::yJsonValue::operator std::string(){

    if ( _json_root_value->isString() ){

        _stdstring_value = _json_root_value->asString();
        _value_type = yJsonValueType::STRING_TYPE;
    }
    else
    {
        _value_type = yJsonValueType::ERROR_TYPE;
        _stdstring_value = "";
        return std::string("");//Default error value;
    }
    
    return _stdstring_value;
}
*/

yLib::yJsonValue  yLib::yJsonValue::operator [](std::string key_str){

    return operator[](key_str.c_str());
}

yLib::yJsonValue  yLib::yJsonValue::operator [](const char * key_str){

    yLib::yJsonValue value_;

    //value_._json_root_value_bak = value_._json_root_value;
    delete value_._json_root_value;
    value_._json_root_value = nullptr;

    
    try
    {
        //now, we can not delete value_._json_root_value in destruct, it is managed by Json::Value
        value_._json_root_value = &_json_root_value->operator[](key_str);
    }
    catch(const Json::LogicError & e)
    {
        yLib::yLog::E("yLib::yJsonValue::operator [](const char * key_str) error: %s", e.what());
        return yLib::yJsonValue();//null value
    }
    
    value_._json_root_value_bak = value_._json_root_value;




    switch (value_._json_root_value->type())
    {
    case Json::ValueType::uintValue:
    case Json::ValueType::intValue:{
     
        value_._value_type = yLib::yJsonValue::INT64_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
        value_._value_containter._n_value_int64 = value_._json_root_value->asInt64();
        break;
    }

    case Json::ValueType::booleanValue :{
     
        value_._value_type = yLib::yJsonValue::BOOL_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
        value_._value_containter._b_value_bool = value_._json_root_value->asBool();
        break;
    }

    case Json::ValueType::realValue :{
     
        value_._value_type = yLib::yJsonValue::DOUBLE_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE;
        value_._value_containter._f_value_double = value_._json_root_value->asDouble();
        break;
    }

    case Json::ValueType::stringValue :{
     
        value_._value_type = yLib::yJsonValue::STRING_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
        value_._value_containter._str_value_string = value_._json_root_value->asString();
        break;
    }

    case Json::ValueType::arrayValue :{
     
        value_._value_type = yLib::yJsonValue::ARRAY_TYPE;
        break;
    }

    case Json::ValueType::objectValue :{
     
        value_._value_type = yLib::yJsonValue::OBJECT_TYPE;

        break;
    }

    case Json::ValueType::nullValue :{
     
        value_._value_type = yLib::yJsonValue::NULL_TYPE;

        break;
    }
    
    default:
        break;
    }



    //check special value type
    // if ( value_._json_root_value->isObject() ){

    //     value_._value_type = yLib::yJsonValue::yJsonValueType::OBJECT_TYPE;
    // }

    // if ( value_._json_root_value->isArray() ){

    //     value_._value_type = yLib::yJsonValue::yJsonValueType::ARRAY_TYPE;
    // }

    // if ( value_._json_root_value->isNull() ){

    //     value_._value_type = yLib::yJsonValue::yJsonValueType::NULL_TYPE;
    // }

    return value_;
}
yLib::yJsonValue  yLib::yJsonValue::operator [](uint64_t elment_idx){

    yLib::yJsonValue value_;

    try
    {
        //now, we can not delete value_._json_root_value in destruct, it is managed by Json::Value
        value_._json_root_value =  &_json_root_value->operator[]((Json::ArrayIndex)elment_idx);
    }
    catch(const Json::LogicError & e)
    {
        yLib::yLog::E("yLib::yJsonValue::operator [](uint64_t elment_idx) error: %s", e.what());
        return yLib::yJsonValue();//null value
    }

    value_._json_root_value_bak = value_._json_root_value;



    switch (value_._json_root_value->type())
    {
    case Json::ValueType::uintValue:
    case Json::ValueType::intValue:{
     
        value_._value_type = yLib::yJsonValue::INT64_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
        value_._value_containter._n_value_int64 = value_._json_root_value->asInt64();
        break;
    }

    case Json::ValueType::booleanValue :{
     
        value_._value_type = yLib::yJsonValue::BOOL_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
        value_._value_containter._b_value_bool = value_._json_root_value->asBool();
        break;
    }

    case Json::ValueType::realValue :{
     
        value_._value_type = yLib::yJsonValue::DOUBLE_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE;
        value_._value_containter._f_value_double = value_._json_root_value->asDouble();
        break;
    }

    case Json::ValueType::stringValue :{
     
        value_._value_type = yLib::yJsonValue::STRING_TYPE;
        value_._cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
        value_._value_containter._str_value_string = value_._json_root_value->asString();
        break;
    }

    case Json::ValueType::arrayValue :{
     
        value_._value_type = yLib::yJsonValue::ARRAY_TYPE;
        break;
    }

    case Json::ValueType::objectValue :{
     
        value_._value_type = yLib::yJsonValue::OBJECT_TYPE;

        break;
    }

    case Json::ValueType::nullValue :{
     
        value_._value_type = yLib::yJsonValue::NULL_TYPE;

        break;
    }
    
    default:
        break;
    }











    // //check special value type
    // if ( value_._json_root_value->isObject() ){

    //     value_._value_type = yLib::yJsonValue::yJsonValueType::OBJECT_TYPE;
    // }

    // if ( value_._json_root_value->isArray() ){

    //     value_._value_type = yLib::yJsonValue::yJsonValueType::ARRAY_TYPE;
    // }

    // if ( value_._json_root_value->isNull() ){

    //     value_._value_type = yLib::yJsonValue::yJsonValueType::NULL_TYPE;
    // }
    
    return value_;
}



yLib::yJsonValue & yLib::yJsonValue::operator =(const yLib::yJsonValue & value_) noexcept{

    _value_type = value_._value_type;
    _cur_basic_value_type = value_._cur_basic_value_type;
    
    switch (_value_type)
    {
    case yJsonValueType::INT64_TYPE:
        /* code */
        _value_containter._n_value_int64 = value_._value_containter._n_value_int64;
        break;
    case yJsonValueType::DOUBLE_TYPE:
        _value_containter._f_value_double = value_._value_containter._f_value_double;
        break;
    case yJsonValueType::BOOL_TYPE:
        _value_containter._b_value_bool = value_._value_containter._b_value_bool;
        break;
    case yJsonValueType::STRING_TYPE:
        _value_containter._str_value_string = value_._value_containter._str_value_string;
        break;
    case yJsonValueType::OBJECT_TYPE:
        break;
    case yJsonValueType::ARRAY_TYPE:
        break;
    case yJsonValueType::NULL_TYPE:
        break;
    default:
        break;
    }


    *(_json_root_value) = *(value_._json_root_value);
    return *this;
}
yLib::yJsonValue & yLib::yJsonValue::operator =(yLib::yJsonValue && value_) noexcept{
    
    _value_type = value_._value_type;
    _cur_basic_value_type = value_._cur_basic_value_type;

    switch (_value_type)
    {
    case yJsonValueType::INT64_TYPE:
        /* code */
        _value_containter._n_value_int64 = value_._value_containter._n_value_int64;
        break;
    case yJsonValueType::DOUBLE_TYPE:
        _value_containter._f_value_double = value_._value_containter._f_value_double;
        break;
    case yJsonValueType::BOOL_TYPE:
        _value_containter._b_value_bool = value_._value_containter._b_value_bool;
        break;
    case yJsonValueType::STRING_TYPE:
        _value_containter._str_value_string = value_._value_containter._str_value_string;
        break;
    case yJsonValueType::OBJECT_TYPE:
        break;
    case yJsonValueType::ARRAY_TYPE:
        break;
    case yJsonValueType::NULL_TYPE:
        break;
    default:
        break;
    }


    *_json_root_value = *value_._json_root_value;
    return *this;
}

// yLib::yJsonValue & yLib::yJsonValue::operator =(yJsonValue value_){

    
//     switch (value_._value_type)
//     {
//     case yJsonValueType::INT64_TYPE:
//         /* code */
//         break;
//     case yJsonValueType::DOUBLE_TYPE:
//         break;
//     case yJsonValueType::BOOL_TYPE:
//         break;
//     case yJsonValueType::STRING_TYPE:
//         break;
//     case yJsonValueType::OBJECT_TYPE:
//         break;
//     case yJsonValueType::ARRAY_TYPE:
//         break;
//     case yJsonValueType::NULL_TYPE:
//         break;
//     default:
//         break;
//     }
// }


yLib::yJsonValue & yLib::yJsonValue::operator=(int64_t value){

    _cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
    _value_type = yLib::yJsonValue::INT64_TYPE;

    _value_containter._n_value_int64 = value;
    *_json_root_value = value;

    return *this;
}
yLib::yJsonValue & yLib::yJsonValue::operator=(bool value){

    _cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
    _value_type = yLib::yJsonValue::BOOL_TYPE;

    _value_containter._b_value_bool = value;
    *_json_root_value = value;

    return *this;
}
// yLib::yJsonValue & yLib::yJsonValue::operator=(float value){

//     _cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE;
//     _value_type = yLib::yJsonValue::DOUBLE_TYPE;

//     _value_containter._f_value_double = value;
//     *_json_root_value = value;

//     return *this;
// }

yLib::yJsonValue & yLib::yJsonValue::operator=(double value){

    _cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE;
    _value_type = yLib::yJsonValue::DOUBLE_TYPE;

    _value_containter._f_value_double = (double)value;
    *_json_root_value = value;

    return *this;
}


yLib::yJsonValue & yLib::yJsonValue::operator=(std::string & value){

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    _value_type = yLib::yJsonValue::STRING_TYPE;

    _value_containter._str_value_string = value;
    *_json_root_value = value;

    return *this;
}

yLib::yJsonValue & yLib::yJsonValue::operator=(std::string && value){

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    _value_type = yLib::yJsonValue::STRING_TYPE;

    _value_containter._str_value_string = value;
    *_json_root_value = value;

    return *this;
}

yLib::yJsonValue & yLib::yJsonValue::operator=(const char * value){

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    _value_type = yLib::yJsonValue::STRING_TYPE;

    _value_containter._str_value_string = value;
    *_json_root_value = value;
    
    return *this;
}