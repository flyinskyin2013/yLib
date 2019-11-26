/*
 * @Author: Sky
 * @Date: 2019-10-28 14:16:37
 * @LastEditors: Sky
 * @LastEditTime: 2019-11-26 19:00:20
 * @Description: 
 */

#ifndef _YLIB_CORE_YJSON_HPP_
#define _YLIB_CORE_YJSON_HPP_

#include "ycommon.hpp"
#include "ylog.hpp"

namespace Json{

    class CharReader;
    class CharReaderBuilder;
    class StreamWriter;
    class StreamWriterBuilder;
    class Value;
}

namespace yLib{
    class yJsonValue;

    class yJson MACRO_PUBLIC_INHERIT_YOBJECT
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

        //operator 
        yJsonValue yJsonGetValue(void);
        int yJsonWriteValue(yJsonValue & value);
    private:
        Json::CharReader * _json_reader = nullptr;
        Json::CharReaderBuilder * _json_reader_builder = nullptr;
        Json::StreamWriter * _json_writer = nullptr;
        Json::StreamWriterBuilder * _json_writer_builder = nullptr;


        Json::Value * _json_root_value = nullptr;

        char * _json_str_buf = nullptr;
    };


    class yJsonValue MACRO_PUBLIC_INHERIT_YOBJECT
    {
    public:
        friend class yJson;
        enum yJsonValueType{
            
            //basic type
            NONE_TYPE = 0,
            INT64_TYPE = 1,
            BOOL_TYPE = 2,
            DOUBLE_TYPE = 3,
            STRING_TYPE = 4,
            
            //some other type
            ARRAY_TYPE = 100, // []
            OBJECT_TYPE = 101, // {}
            NULL_TYPE = 102, //null
            ERROR_TYPE = 200,
        };

        yJsonValueType _value_type = yJsonValueType::NONE_TYPE;
    private:
        /* data */
        int64_t _int64_value = 0;
        bool _boolean_value = false;
        double _double_value = 0.0f;
        std::string _stdstring_value = "";
        Json::Value * _json_root_value = nullptr ;
        //special for operator[](char * str)
        Json::Value * _json_root_value_bak = nullptr;
    public:
        

        //yJsonValue(/*Value args */) noexcept;
        //special yJsonValue
        yJsonValue(int64_t value_) noexcept;
        yJsonValue(bool value_) noexcept;
        yJsonValue(double value_) noexcept;
        yJsonValue(std::string stdstr_) noexcept;
        yJsonValue(yJsonValueType value_type_ = NULL_TYPE) noexcept;
        
        ~yJsonValue();

        //copy and move constructor
        yJsonValue(const yJsonValue & value_) noexcept;
        yJsonValue(yJsonValue && value_) noexcept;

        yJsonValue & operator =(const yJsonValue & value_) noexcept;
        yJsonValue & operator =(yJsonValue && value_) noexcept;

        operator int64_t();
        operator bool();
        operator double();
        operator std::string();

        yJsonValue operator [](std::string key_str);
        yJsonValue operator [](const char * key_str);
        yJsonValue operator [](uint64_t elment_idx);

        //yJsonValue & operator =(yJsonValue value_);//
        
    };

    
}




#endif //_YLIB_CORE_YJSON_HPP_