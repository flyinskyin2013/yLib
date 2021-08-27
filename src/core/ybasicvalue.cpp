/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 13:50:29
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 16:06:24
 * @FilePath: \yLib\src\ybasicvalue.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ybasicvalue.hpp"

//////////////////////////////////////////
////////////yValue
//////////////////////////////////////////

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yValue)


yLib::yValue::yValue() noexcept 
{

    CleanAllToDefault();
}

yLib::yValue::~yValue() noexcept
{

    CleanAllToDefault();
}


/**
 * @fn  yValue(const yValue &value)
 * @brief copy constructor
 * @param value a exsited obj
 * @return 
 */
yLib::yValue::yValue(const yValue &value) noexcept

{
    CleanAllToDefault();

    CopyValueContainer(value, *this);
}

/**
 * @fn  yValue & operator=(const yValue &value) noexcept
 * @brief assignment constructor
 * @param value a exsited obj
 * @return the obj's reference of yValue
 */
yLib::yValue & yLib::yValue::operator=(const yValue &value) noexcept
{
    CleanAllToDefault();
    
    CopyValueContainer(value, *this);
    
    return *this;
}

/**
 * @fn  yValue(const yValue &&value) noexcept
 * @brief move constructor
 * @param value a exsited obj
 * @return 
 */
yLib::yValue::yValue(const yValue &&value) noexcept

{
    CleanAllToDefault();

    CopyValueContainer(value, *this);
}

/**
 * @fn  yValue & operator=(const yValue &&value) noexcept
 * @brief assignment constructor
 * @param value a exsited obj
 * @return the obj's reference of yValue
 */
yLib::yValue & yLib::yValue::operator=(const yValue &&value) noexcept
{

    CleanAllToDefault();
    
    CopyValueContainer(value, *this);
    
    return *this;
}


//other constructor
yLib::yValue::yValue(int8_t value_) noexcept

{

    CleanAllToDefault();

    value_containter.int64_val = value_;
    cur_value_type = yValueType::INT8_TYPE;
}


yLib::yValue::yValue(uint8_t value_) noexcept

{

    CleanAllToDefault();

    value_containter.uint64_val = value_;
    cur_value_type = yValueType::UINT8_TYPE;
}

yLib::yValue::yValue(int16_t value_) noexcept

{

    CleanAllToDefault();

    value_containter.int64_val = value_;
    cur_value_type = yValueType::INT16_TYPE;
}


yLib::yValue::yValue(uint16_t value_) noexcept

{

    CleanAllToDefault();
    
    value_containter.uint64_val = value_;
    cur_value_type = yValueType::UINT16_TYPE;
}


yLib::yValue::yValue(int32_t value_) noexcept

{

    CleanAllToDefault();

    value_containter.int64_val = value_;
    cur_value_type = yValueType::INT32_TYPE;
}


yLib::yValue::yValue(uint32_t value_) noexcept

{
    CleanAllToDefault();

    value_containter.uint64_val = value_; 
    cur_value_type = yValueType::UINT32_TYPE;
}

yLib::yValue::yValue(int64_t value) noexcept

{

    CleanAllToDefault();

    value_containter.int64_val = value; 
    cur_value_type = yValueType::INT64_TYPE;
}


yLib::yValue::yValue(uint64_t value) noexcept

{

    CleanAllToDefault();

    value_containter.uint64_val = value; 
    cur_value_type = yValueType::UINT64_TYPE;
}


yLib::yValue::yValue(bool value) noexcept

{

    CleanAllToDefault();

    value_containter.bool_val = value; 
    cur_value_type = yValueType::BOOL_TYPE;
}

yLib::yValue::yValue(float value) noexcept

{

    CleanAllToDefault();
    
    value_containter.float_val = value; 
    cur_value_type = yValueType::FLOAT_TYPE;
}

yLib::yValue::yValue(double value) noexcept

{

    CleanAllToDefault();

    value_containter.double_val = value; 
    cur_value_type = yValueType::DOUBLE_TYPE;
}

yLib::yValue::yValue(const std::string & value) noexcept

{

    CleanAllToDefault();

    value_containter.string_val = value; 
    cur_value_type = yValueType::STRING_TYPE;
}

yLib::yValue::yValue(yValueType type, void * value) noexcept

{

    CleanAllToDefault();
    cur_value_type = type;
    value_containter.ptr_val = (uintptr_t)value;
}



//basic convert yValue to basic value-type

yLib::yValue::operator int8_t() const noexcept
{
    return (int8_t)value_containter.int64_val;
}


yLib::yValue::operator uint8_t() const noexcept
{

    return (uint8_t)value_containter.uint64_val;
}


yLib::yValue::operator int16_t() const noexcept
{

    return (int16_t)value_containter.int64_val;
}


yLib::yValue::operator uint16_t() const noexcept
{

    return (uint16_t)value_containter.uint64_val;
}

yLib::yValue::operator int32_t() const noexcept
{
    return (int32_t)value_containter.int64_val;
}

yLib::yValue::operator uint32_t() const noexcept
{
    return (uint32_t)value_containter.uint64_val;
}

yLib::yValue::operator int64_t() const noexcept
{
    return value_containter.int64_val;
}

yLib::yValue::operator uint64_t() const noexcept
{
    return value_containter.uint64_val;
}

yLib::yValue::operator bool() const noexcept
{
    return value_containter.bool_val;
}

yLib::yValue::operator float() const noexcept
{
    return value_containter.float_val;
}

yLib::yValue::operator double() const noexcept
{
    return value_containter.double_val;
}

yLib::yValue::operator std::string() const noexcept
{
    return value_containter.string_val;
}
//operator const char * () const; this will conflict with std::string's constructor




/**
 * @fn  virtual yValue & operator=(int8_t value) noexcept
 * @brief convert int8_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int8_t value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::INT8_TYPE;
    value_containter.int64_val = value;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint8_t value) noexcept
 * @brief convert uint8_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint8_t value) noexcept
{

    CleanAllToDefault();

    cur_value_type = yValueType::UINT8_TYPE;
    value_containter.uint64_val = value;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(int16_t value) noexcept
 * @brief convert int16_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int16_t value) noexcept
{

    CleanAllToDefault();

    cur_value_type = yValueType::INT16_TYPE;
    value_containter.int64_val = value;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint16_t value) noexcept
 * @brief convert uint16_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint16_t value) noexcept
{

    CleanAllToDefault();

    cur_value_type = yValueType::UINT16_TYPE;
    value_containter.uint64_val = value;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(int32_t value) noexcept
 * @brief convert int32_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int32_t value) noexcept
{

    CleanAllToDefault();

    cur_value_type = yValueType::INT32_TYPE;
    value_containter.int64_val = value;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint32_t value) noexcept
 * @brief convert uint32_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint32_t value) noexcept
{

    CleanAllToDefault();

    cur_value_type = yValueType::UINT32_TYPE;
    value_containter.uint64_val = value;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(int64_t value) noexcept
 * @brief convert int64_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int64_t value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::INT64_TYPE;
    value_containter.int64_val = value;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint64_t value) noexcept
 * @brief convert uint64_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint64_t value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::UINT64_TYPE;
    value_containter.uint64_val = value;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(float value) noexcept
 * @brief convert float to exsited obj of yValue
 * @return return obj of yValue
 */        
yLib::yValue & yLib::yValue::operator=(float value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::FLOAT_TYPE;
    value_containter.float_val = value;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(double value) noexcept
 * @brief convert double to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(double value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::DOUBLE_TYPE;
    value_containter.double_val = value;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(bool value) noexcept
 * @brief convert bool to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(bool value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::BOOL_TYPE;
    value_containter.bool_val = value;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(std::string value) noexcept
 * @brief convert std::string to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(const std::string &value) noexcept
{
    return operator=(value.c_str());
}

yLib::yValue & yLib::yValue::operator=(const char *value) noexcept
{
    CleanAllToDefault();

    cur_value_type = yValueType::STRING_TYPE;
    value_containter.string_val = value;

    return *this;
}

yLib::yValue::yValueType yLib::yValue::GetType(void) const noexcept
{

    return cur_value_type;
}

/**
 * @fn  virtual void * GetSpecialValue(void) const noexcept
 *
 * @brief get the val-type of obj.
 * 
 * @param type_ get value of special type 
 * 
 * @return the value of special type 
 * @retval other_addr ok.
 * @retval nullptr error.
 * 
 * @warning this call only support TYPE_OBJECT, TYPE_ARRAY, TYPE_GROUP
 */
void * yLib::yValue::GetSpecialValue(void) const noexcept
{
    return (void*)value_containter.ptr_val;
}

/**
 * @fn  void CleanAllToDefault(void) noexcept
 * @brief set obj of yValue to default.
 * @return 
 * @warning this op will modify type and value-container
 */
void yLib::yValue::CleanAllToDefault(void) noexcept
{

    cur_value_type = yValueType::NONE_TYPE;

    value_containter.int64_val = 0;
    value_containter.uint64_val = 0;    

    value_containter.float_val = 0;
    value_containter.double_val = 0;

    value_containter.bool_val = 0;

    value_containter.string_val = "";

    //special val type
    value_containter.ptr_val = 0;
}

void yLib::yValue::CopyValueContainer(const yValue & src, yValue & dst) noexcept
{

    dst.cur_value_type = src.cur_value_type;//copy type

    //copy value
    dst.value_containter.int64_val = src.value_containter.int64_val;
    dst.value_containter.uint64_val = src.value_containter.uint64_val;

    dst.value_containter.float_val = src.value_containter.float_val;
    dst.value_containter.double_val = src.value_containter.double_val;

    dst.value_containter.bool_val = src.value_containter.bool_val;

    dst.value_containter.string_val = src.value_containter.string_val;
    
    dst.value_containter.ptr_val = src.value_containter.ptr_val;
}