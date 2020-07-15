/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 13:50:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:38:37
 * @FilePath: \yLib\src\ybasicvalue.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ybasicvalue.hpp"

yLib::yBasicValue::yBasicValue() noexcept
MACRO_INIT_YOBJECT_PROPERTY(yBasicValue) 
{
    _cur_basic_value_type = yLib::yBasicValueType::NONE_YBASICVALUE_TYPE;
}

yLib::yBasicValue::~yBasicValue(){

    
}


yLib::yBasicValue::yBasicValue(int64_t value_) noexcept{
    
    _cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE ;
    _value_containter._n_value_int64 = value_;
}
yLib::yBasicValue::yBasicValue(uint64_t value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::UINT64_YBASICVALUE_TYPE ;
    _value_containter._n_value_uint64 = value_;
}
yLib::yBasicValue::yBasicValue(bool value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE ;
    _value_containter._b_value_bool = value_;
}
yLib::yBasicValue::yBasicValue(float value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE ;
    _value_containter._f_value_float = value_;
}
yLib::yBasicValue::yBasicValue(double value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE ;
    _value_containter._f_value_double = value_;
}
yLib::yBasicValue::yBasicValue(std::string & value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE ;
    _value_containter._str_value_string = value_;
}
yLib::yBasicValue::yBasicValue(const char * value_) noexcept{

    _cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE ;
    _value_containter._str_value_string = value_;
}




yLib::yBasicValue::yBasicValue(const yLib::yBasicValue & value_) noexcept{

    _cur_basic_value_type = value_._cur_basic_value_type;
    switch (_cur_basic_value_type)
    {
    case yLib::yBasicValueType::INT64_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._n_value_int64 = value_._value_containter._n_value_int64;
        break;
    }
    case yLib::yBasicValueType::UINT64_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._n_value_uint64 = value_._value_containter._n_value_uint64;
        break;
    }
    case yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._b_value_bool = value_._value_containter._b_value_bool;
        break;
    }
    case yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._f_value_float = value_._value_containter._f_value_float;
        break;
    }
    case yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._f_value_double = value_._value_containter._f_value_double;
        break;
    }
    case yLib::yBasicValueType::STRING_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._str_value_string = value_._value_containter._str_value_string;
        break;
    }
    default:
    {
        _cur_basic_value_type = yLib::yBasicValueType::NONE_YBASICVALUE_TYPE;
        yLib::yLog::W("yBasicValue(): the value_-type is invalid.");
        break;
    }
    }
}

yLib::yBasicValue & yLib::yBasicValue::operator=(const yLib::yBasicValue & value_) noexcept{

    _cur_basic_value_type = value_._cur_basic_value_type;
    switch (_cur_basic_value_type)
    {
    case yLib::yBasicValueType::INT64_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._n_value_int64 = value_._value_containter._n_value_int64;
        break;
    }
    case yLib::yBasicValueType::UINT64_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._n_value_uint64 = value_._value_containter._n_value_uint64;
        break;
    }
    case yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._b_value_bool = value_._value_containter._b_value_bool;
        break;
    }
    case yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._f_value_float = value_._value_containter._f_value_float;
        break;
    }
    case yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._f_value_double = value_._value_containter._f_value_double;
        break;
    }
    case yLib::yBasicValueType::STRING_YBASICVALUE_TYPE:{
        /* code */
        _value_containter._str_value_string = value_._value_containter._str_value_string;
        break;
    }
    default:
    {
        _cur_basic_value_type = yLib::yBasicValueType::NONE_YBASICVALUE_TYPE;
        yLib::yLog::W("yBasicValue(): the value_-type is invalid.");
        break;
    }
    }

    return *this;
}




yLib::yBasicValue::operator int64_t() const{

    return _value_containter._n_value_int64;
}
yLib::yBasicValue::operator uint64_t() const{

    return _value_containter._n_value_uint64;
}
yLib::yBasicValue::operator bool() const{

    return _value_containter._b_value_bool;
}
yLib::yBasicValue::operator float() const{

    return _value_containter._f_value_float;
}
yLib::yBasicValue::operator double() const{

    return _value_containter._f_value_double;
}
yLib::yBasicValue::operator std::string() const{

    return _value_containter._str_value_string;
}

// yLib::yBasicValue::operator const char * () const{

//     return _value_containter._str_value_string.c_str();
// }

yLib::yBasicValue & yLib::yBasicValue::operator=(int64_t value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::INT64_YBASICVALUE_TYPE;
    this->_value_containter._n_value_int64 = value_;
    return *this;
}
yLib::yBasicValue & yLib::yBasicValue::operator=(uint64_t value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::UINT64_YBASICVALUE_TYPE;
    this->_value_containter._n_value_uint64 = value_;
    return *this;
}
yLib::yBasicValue & yLib::yBasicValue::operator=(bool value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::BOOL_YBASICVALUE_TYPE;
    this->_value_containter._b_value_bool = value_;
    return *this;
}
yLib::yBasicValue & yLib::yBasicValue::operator=(float value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::FLOAT_YBASICVALUE_TYPE;
    this->_value_containter._f_value_float = value_;
    return *this;
}
yLib::yBasicValue & yLib::yBasicValue::operator=(double value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::DOUBLE_YBASICVALUE_TYPE;
    this->_value_containter._f_value_double = value_;
    return *this;
}
yLib::yBasicValue & yLib::yBasicValue::operator=(std::string & value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    this->_value_containter._str_value_string = value_;
    return *this;
}

yLib::yBasicValue & yLib::yBasicValue::operator=(std::string && value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    this->_value_containter._str_value_string = value_;
    return *this;
}

yLib::yBasicValue & yLib::yBasicValue::operator=(const char * value_){

    this->_cur_basic_value_type = yLib::yBasicValueType::STRING_YBASICVALUE_TYPE;
    this->_value_containter._str_value_string = value_;
    return *this;
}


yLib::yBasicValueType yLib::yBasicValue::GetyBasicValueType(void) const{

    return _cur_basic_value_type;
}