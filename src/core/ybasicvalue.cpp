/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 13:50:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-10 18:44:19
 * @FilePath: \yLib\src\ybasicvalue.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "core/ybasicvalue.hpp"

//////////////////////////////////////////
////////////yValue
//////////////////////////////////////////

yLib::yValue::yValue() noexcept 
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();
}

yLib::yValue::~yValue() noexcept
{

    CleanAllToDefault();
}


/**
 * @fn  yValue(const yValue &value_)
 * @brief copy constructor
 * @param value_ a exsited obj
 * @return 
 */
yLib::yValue::yValue(const yValue &value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{
    CleanAllToDefault();

    CopyValueContainer(value_, *this);
}

/**
 * @fn  yValue & operator=(const yValue &value_) noexcept
 * @brief assignment constructor
 * @param value_ a exsited obj
 * @return the obj's reference of yValue
 */
yLib::yValue & yLib::yValue::operator=(const yValue &value_) noexcept
{
    CleanAllToDefault();
    
    CopyValueContainer(value_, *this);
    
    return *this;
}

/**
 * @fn  yValue(const yValue &&value_) noexcept
 * @brief move constructor
 * @param value_ a exsited obj
 * @return 
 */
yLib::yValue::yValue(const yValue &&value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{
    CleanAllToDefault();

    CopyValueContainer(value_, *this);
}

/**
 * @fn  yValue & operator=(const yValue &&value_) noexcept
 * @brief assignment constructor
 * @param value_ a exsited obj
 * @return the obj's reference of yValue
 */
yLib::yValue & yLib::yValue::operator=(const yValue &&value_) noexcept
{

    CleanAllToDefault();
    
    CopyValueContainer(value_, *this);
    
    return *this;
}


//other constructor
yLib::yValue::yValue(int8_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.int8_val = value_;
    cur_value_type = yValueType::INT8_TYPE;
}


yLib::yValue::yValue(uint8_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.uint8_val = value_;
    cur_value_type = yValueType::UINT8_TYPE;
}

yLib::yValue::yValue(int16_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.int16_val = value_;
    cur_value_type = yValueType::INT16_TYPE;
}


yLib::yValue::yValue(uint16_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();
    
    value_containter.uint16_val = value_;
    cur_value_type = yValueType::UINT16_TYPE;
}


yLib::yValue::yValue(int32_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.int32_val = value_;
    cur_value_type = yValueType::INT32_TYPE;
}


yLib::yValue::yValue(uint32_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{
    CleanAllToDefault();

    value_containter.uint32_val = value_; 
    cur_value_type = yValueType::UINT32_TYPE;
}


yLib::yValue::yValue(int64_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.int64_val = value_; 
    cur_value_type = yValueType::INT64_TYPE;
}


yLib::yValue::yValue(uint64_t value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.uint64_val = value_; 
    cur_value_type = yValueType::UINT64_TYPE;
}


yLib::yValue::yValue(bool value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.bool_val = value_; 
    cur_value_type = yValueType::BOOL_TYPE;
}

yLib::yValue::yValue(float value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();
    
    value_containter.float_val = value_; 
    cur_value_type = yValueType::FLOAT_TYPE;
}

yLib::yValue::yValue(double value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.double_val = value_; 
    cur_value_type = yValueType::DOUBLE_TYPE;
}

yLib::yValue::yValue(const std::string & value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.string_val = value_; 
    cur_value_type = yValueType::STRING_TYPE;
}

yLib::yValue::yValue(const char * value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();

    value_containter.string_val = value_; 
    cur_value_type = yValueType::STRING_TYPE;
}

yLib::yValue::yValue(yValueType type_, void * value_) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yValue)
{

    CleanAllToDefault();
    switch (type_)
    {
        case yValueType::OBJECT_TYPE:{
            
            cur_value_type = yValueType::OBJECT_TYPE;
            value_containter.object_val = (uintptr_t)value_;
            break;
        }

        case yValueType::ARRAY_TYPE:{

            cur_value_type = yValueType::ARRAY_TYPE;
            value_containter.array_val = (uintptr_t)value_;
            break;
        }

        case yValueType::GROUP_TYPE:{

            cur_value_type = yValueType::GROUP_TYPE;
            value_containter.group_val = (uintptr_t)value_;
            break;
        }
        default:{
            
            cur_value_type = yValueType::NONE_TYPE;
            break;
        }
    }

}



//basic convert yValue to basic value-type

yLib::yValue::operator int8_t() const
{
    return value_containter.int8_val;
}


yLib::yValue::operator uint8_t() const{

    return value_containter.uint8_val;
}


yLib::yValue::operator int16_t() const
{

    return value_containter.int16_val;
}


yLib::yValue::operator uint16_t() const
{

    return value_containter.uint16_val;
}

yLib::yValue::operator int32_t() const
{
    return value_containter.int32_val;
}

yLib::yValue::operator uint32_t() const
{
    return value_containter.uint32_val;
}

yLib::yValue::operator int64_t() const
{
    return value_containter.int64_val;
}

yLib::yValue::operator uint64_t() const
{
    return value_containter.uint64_val;
}

yLib::yValue::operator bool() const
{
    return value_containter.bool_val;
}

yLib::yValue::operator float() const
{
    return value_containter.float_val;
}

yLib::yValue::operator double() const
{
    return value_containter.double_val;
}

yLib::yValue::operator std::string() const
{
    return value_containter.string_val;
}
//operator const char * () const; this will conflict with std::string's constructor




/**
 * @fn  virtual yValue & operator=(int8_t value_)
 * @brief convert int8_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int8_t value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::INT8_TYPE;
    value_containter.int8_val = value_;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint8_t value_)
 * @brief convert uint8_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint8_t value_)
{

    CleanAllToDefault();

    cur_value_type = yValueType::UINT8_TYPE;
    value_containter.uint8_val = value_;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(int16_t value_)
 * @brief convert int16_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int16_t value_)
{

    CleanAllToDefault();

    cur_value_type = yValueType::INT16_TYPE;
    value_containter.int16_val = value_;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint16_t value_)
 * @brief convert uint16_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint16_t value_)
{

    CleanAllToDefault();

    cur_value_type = yValueType::UINT16_TYPE;
    value_containter.uint16_val = value_;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(int32_t value_)
 * @brief convert int32_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int32_t value_){

    CleanAllToDefault();

    cur_value_type = yValueType::INT32_TYPE;
    value_containter.int32_val = value_;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint32_t value_)
 * @brief convert uint32_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint32_t value_)
{

    CleanAllToDefault();

    cur_value_type = yValueType::UINT32_TYPE;
    value_containter.uint32_val = value_;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(int64_t value_)
 * @brief convert int64_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(int64_t value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::INT64_TYPE;
    value_containter.int64_val = value_;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(uint64_t value_)
 * @brief convert uint64_t to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(uint64_t value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::UINT64_TYPE;
    value_containter.uint64_val = value_;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(float value_)
 * @brief convert float to exsited obj of yValue
 * @return return obj of yValue
 */        
yLib::yValue & yLib::yValue::operator=(float value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::FLOAT_TYPE;
    value_containter.float_val = value_;

    return *this;
}
/**
 * @fn  virtual yValue & operator=(double value_)
 * @brief convert double to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(double value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::DOUBLE_TYPE;
    value_containter.double_val = value_;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(bool value_)
 * @brief convert bool to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(bool value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::BOOL_TYPE;
    value_containter.bool_val = value_;

    return *this;
}

/**
 * @fn  virtual yValue & operator=(std::string value_)
 * @brief convert std::string to exsited obj of yValue
 * @return return obj of yValue
 */
yLib::yValue & yLib::yValue::operator=(const std::string &value_)
{
    return operator=(value_.c_str());
}

yLib::yValue & yLib::yValue::operator=(const char *value_)
{
    CleanAllToDefault();

    cur_value_type = yValueType::STRING_TYPE;
    value_containter.string_val = value_;

    return *this;
}

yLib::yValue::yValueType yLib::yValue::GetType(void) const
{

    return cur_value_type;
}

/**
 * @fn  virtual void * GetSpecialValue(yValueType type_) const
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
void * yLib::yValue::GetSpecialValue(yValueType type_) const
{

    switch (type_)
    {
        case yValueType::OBJECT_TYPE:{
            
            return (void*)value_containter.object_val;
            break;
        }

        case yValueType::ARRAY_TYPE:{

            return (void*)value_containter.array_val;
            break;
        }

        case yValueType::GROUP_TYPE:{

            return (void*)value_containter.group_val;
            break;
        }
        default:{
            
            return nullptr;
            break;
        }
    }
}

/**
 * @fn  void CleanAllToDefault(void)
 * @brief set obj of yValue to default.
 * @return 
 * @warning this op will modify type and value-container
 */
void yLib::yValue::CleanAllToDefault(void)
{

    cur_value_type = yValueType::NONE_TYPE;

    value_containter.int8_val = 0;
    value_containter.uint8_val = 0;

    value_containter.int16_val = 0;
    value_containter.uint16_val = 0;

    value_containter.int32_val = 0;
    value_containter.uint32_val = 0;            

    value_containter.int64_val = 0;
    value_containter.uint64_val = 0;    

    value_containter.float_val = 0;
    value_containter.double_val = 0;

    value_containter.bool_val = 0;

    value_containter.string_val = "";

    //special val type
    value_containter.object_val = 0;
    value_containter.array_val = 0;
    value_containter.group_val = 0;
}


/**
 * @fn  CopyValueContainer(const yValue & src_, yValue & dst_)
 * @brief copy src_'s value-container to dst_'s value-container
 * @param src_ the value-container copy from
 * @param dst_ the value-container copy to
 * @return 
 */
void yLib::yValue::CopyValueContainer(const yValue & src_, yValue & dst_){

    dst_.cur_value_type = src_.cur_value_type;//copy type

    //copy value
    dst_.value_containter.int8_val = src_.value_containter.int8_val;
    dst_.value_containter.uint8_val = src_.value_containter.uint8_val;

    dst_.value_containter.int16_val = src_.value_containter.int16_val;
    dst_.value_containter.uint16_val = src_.value_containter.uint16_val;

    dst_.value_containter.int32_val = src_.value_containter.int32_val;
    dst_.value_containter.uint32_val = src_.value_containter.uint32_val;

    dst_.value_containter.int64_val = src_.value_containter.int64_val;
    dst_.value_containter.uint64_val = src_.value_containter.uint64_val;

    dst_.value_containter.float_val = src_.value_containter.float_val;
    dst_.value_containter.double_val = src_.value_containter.double_val;

    dst_.value_containter.bool_val = src_.value_containter.bool_val;

    dst_.value_containter.string_val = src_.value_containter.string_val;
    
    dst_.value_containter.object_val = src_.value_containter.object_val;
    dst_.value_containter.array_val = src_.value_containter.array_val;
    dst_.value_containter.group_val = src_.value_containter.group_val;

    
}