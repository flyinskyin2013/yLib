/*
 * @Author: Sky
 * @Date: 2019-10-28 14:16:37
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-26 11:01:36
 * @Description: 
 */

#ifndef _YLIB_CORE_YJSON_HPP_
#define _YLIB_CORE_YJSON_HPP_

#include "ycommon.hpp"
#include "ybasicvalue.hpp"
#include "ylog.hpp"


#include <memory>
#include <cstring>

namespace Json{

    class CharReader;
    class CharReaderBuilder;
    class StreamWriter;
    class StreamWriterBuilder;
    class Value;
}

namespace yLib{
    class yJsonValue;

    class __YLIB_EXPORT__ yJson  MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
    public:
        yJson(/* args */) noexcept;
        ~yJson();

        //I/O,Defaultly, json-str decode or encode  by utf-8
        int yJsonReadFile(std::string file);
        int yJsonReadMemory(const int8_t * mem_addr, uint64_t mem_size);
        int yJsonWriteFile(std::string file);
        int yJsonWriteMemory(int8_t * mem_addr, uint64_t mem_max_size);

        //deprecate apis
        __YLIB_DEPRECATED_ATTRIBUTE__ yJsonValue yJsonGetValue(void);
        __YLIB_DEPRECATED_ATTRIBUTE__ int yJsonWriteValue(yJsonValue & value);

        yJsonValue  yJsonGetParsedJsonObject(void);
        int yJsonSetJsonObject(yJsonValue & obj_val_);
    private:
        Json::CharReader * _json_reader = nullptr;
        Json::CharReaderBuilder * _json_reader_builder = nullptr;
        Json::StreamWriter * _json_writer = nullptr;
        Json::StreamWriterBuilder * _json_writer_builder = nullptr;


        Json::Value * _json_root_value = nullptr;

        char * _json_str_buf = nullptr;
    };


    class __YLIB_EXPORT__ yJsonValue  :public yBasicValue
    {
    public:
        friend class yJson;
        enum yJsonValueType{
            
            //basic type
            NONE_TYPE = 0,
            INT64_TYPE = 1,
            BOOL_TYPE = 2,
            FLOAT_TYPE = 3,
            DOUBLE_TYPE = 4,
            STRING_TYPE = 5,
            
            //some other type
            ARRAY_TYPE = 100, // []
            OBJECT_TYPE = 101, // {}
            NULL_TYPE = 102, //null
            ERROR_TYPE = 200,
        };

        yJsonValueType _value_type = yJsonValueType::NONE_TYPE;
    private:
        /* data */
        /*
        int64_t _int64_value = 0;
        bool _boolean_value = false;
        double _double_value = 0.0f;
        std::string _stdstring_value = "";
        */
       
        Json::Value * _json_root_value = nullptr ;
        
        //special for operator[](char * str)
        Json::Value * _json_root_value_bak = nullptr;
    public:
        

        //yJsonValue(/*Value args */) noexcept;
        //special yJsonValue
        explicit yJsonValue(int64_t value_) noexcept;
        explicit yJsonValue(bool value_) noexcept;
        explicit yJsonValue(double value_) noexcept;
        explicit yJsonValue(std::string stdstr_) noexcept;
        explicit yJsonValue(yJsonValueType value_type_ = NULL_TYPE) noexcept;
        
        ~yJsonValue();

        //copy and move constructor, not deep copy,_json_root_value==value_._json_root_value
        yJsonValue(const yJsonValue & value_) noexcept;
        yJsonValue(yJsonValue && value_) noexcept;

        yJsonValue & operator =(const yJsonValue & value_) noexcept;
        yJsonValue & operator =(yJsonValue && value_) noexcept;
/*
        operator int64_t();
        operator bool();
        operator double();
        operator std::string();
*/

        yJsonValue & operator=(int64_t value);
        yJsonValue & operator=(bool value);
        //Important notes: the float-type is treated as double-type.
        // yJsonValue & operator=(float value);
        yJsonValue & operator=(double value);
        yJsonValue & operator=(std::string & value);
        yJsonValue & operator=(const char * value);


        yJsonValue operator [](std::string key_str);
        yJsonValue operator [](const char * key_str);
        yJsonValue operator [](uint64_t elment_idx);

        //yJsonValue & operator =(yJsonValue value_);//
        
    };

    
}




#endif //_YLIB_CORE_YJSON_HPP_