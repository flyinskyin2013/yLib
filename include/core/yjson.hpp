/*
 * @Author: Sky
 * @Date: 2019-10-28 14:16:37
 * @LastEditors: Sky
 * @LastEditTime: 2019-10-30 09:26:41
 * @Description: 
 */

#ifndef _YLIB_CORE_YJSON_HPP_
#define _YLIB_CORE_YJSON_HPP_

#include "ycommon.hpp"

namespace Json{

    class CharReader;
    class StreamWriter;
    class Value;
}

namespace yLib{

    class yJson MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
    public:
        yJson(/* args */) noexcept;
        ~yJson();

        //I/O
        int yJsonReadFile(std::string file);
        int yJsonReadMemory(const uint8_t * mem_addr, uint64_t mem_size);
        int yJsonWriteFile(std::string file);
        int yJsonWriteMemory(const uint8_t * mem_addr, uint64_t mem_size);

        //operator 

    private:
        Json::CharReader * _json_reader = nullptr;
        Json::StreamWriter * _json_writer = nullptr;
        Json::Value * _json_root_value = nullptr;
    };


    class yJsonValue MACRO_PUBLIC_INHERIT_YOBJECT
    {
    public:
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
        };

        yJsonValueType _value_type = yJsonValueType::NONE_TYPE;
    private:
        /* data */
        int64_t _int64_value = 0;
        bool _boolean_value = false;
        double _double_value = 0.0;
        std::string _stdstring_value = "";

        Json::Value * _json_value = nullptr;

    public:
        yJsonValue(/*Value args */) noexcept;
        ~yJsonValue();

        operator int64_t();
        operator bool();
        operator double();
        operator std::string();
        
    };

    
}




#endif //_YLIB_CORE_YJSON_HPP_