/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Author: Sky
 * @Date: 2019-10-28 14:16:37
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-10 15:34:14
 * @Description: 
 */

#ifndef __YLIB_UTILITY_YJSON_HPP__
#define __YLIB_UTILITY_YJSON_HPP__

#include "core/ycommon.hpp"
#include "core/ybasicvalue.hpp"
#include "ylog.hpp"

#include <memory>
#include <cstring>
#include <unordered_map>
#include <utility>

namespace yLib{
    class yJsonValue;
    /**
     *  @class yJson
     *  @brief This is a tool to parse the json file.
     */
    class __YLIB_CLASS_DECLSPEC__ yJson  MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        
    public:
        /**
         * @fn  yJson() noexcept
         * @brief default constructor
         * @param 
         * @return 
         * @warning 
         */
        yJson(/* args */) noexcept;

        /**
         * @fn  ~yJson() noexcept
         * @brief default destructor
         * @param 
         * @return 
         */
        ~yJson();

        //I/O,Defaultly, json-str decode or encode  by utf-8
        /**
         * @fn  int8_t ReadFile(const std::string &file_path_)
         * @brief parse json object from json-file.
         * @param file_path_ the json-file.
         * @return this op's status.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t ReadFile(const std::string &file_path_);
        /**
         * @fn  int8_t ReadMemory(const int8_t * addr_, uint64_t size_)
         * @brief parse json object from memory.
         * @param addr_ the mem's addr to be parsed.
         * @param size_ the mem's size to be parsed.
         * @return this op's status.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t ReadMemory(const int8_t * addr_, uint64_t size_);
        
        /**
         * @fn  int8_t WriteFile(const std::string &file_path_)
         * @brief write the str of a json object to a json-file.
         * @param file_path_ the json-file.
         * @return this op's status.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t WriteFile(const std::string &file_path_);
        /**
         * @fn  int8_t WriteMemory(int8_t * addr_, uint64_t max_size_)
         * @brief write the str of a json object to memory.
         * @param addr_ the mem's addr to be written.
         * @param max_size_ the max mem's size to be written.
         * @return this op's status.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t WriteMemory(int8_t * addr_, uint64_t max_size_);

        /**
         * @fn  yJsonValue GetRootObject(void)
         * @brief get the root-obj of json to be read.
         * @return the obj of yJsonValue
         * @warning 
         */
        yJsonValue GetRootObject(void);
        /**
         * @fn  int8_t SetRootObject(const yJsonValue & val_)
         * @brief set the root-obj of json to be written.
         * @param value_ the root-obj
         * @return this op's status.
         * @retval 0 ok.
         * @retval -1 error.
         * @warning 
         */
        int8_t SetRootObject(const yJsonValue & value_);
    private:
        void * json_reader = nullptr;
        void * json_reader_builder = nullptr;
        void * json_writer = nullptr;
        void * json_writer_builder = nullptr;
        void * json_root_value = nullptr;
    };

    /**
     *  @class yJsonValue
     *  @brief This is the yJson's value-container, it inherits from yValue.
     */
    class __YLIB_CLASS_DECLSPEC__ yJsonValue  :public yBasicValue
    {
    public:
        friend class yJson;

    private:
        void * json_value;
        typedef std::unordered_map<std::string, yJsonValue> yJsonValueHolderMap;
        std::unordered_map<std::string, yJsonValue> * value_holder_map;
        /**
         * @fn yJsonValue(bool build_special_, void * ptr_) noexcept
         * @brief sepacial constructor, we will build a empty obj.
         * @param 
         * @return 
         * @warning this will set a flag that controls the desconstructer's behavior, this op is only used by yJsonValue.
         */
        yJsonValue(bool build_special_, void * ptr_) noexcept;
        bool is_special_obj;
    public:
        /**
         * @fn  yJsonValue() noexcept
         * @brief default constructor
         * @param 
         * @return 
         * @warning 
         */
        yJsonValue() noexcept;
        
        //special constructor
        /**
         * @fn  explicit yJsonValue(int64_t value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         * @warning 
         */
        explicit yJsonValue(int64_t value_) noexcept;
        /**
         * @fn  explicit yJsonValue(uint64_t value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         * @warning 
         */
        explicit yJsonValue(uint64_t value_) noexcept;

        /**
         * @fn  explicit yJsonValue(bool value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         * @warning 
         */        
        explicit yJsonValue(bool value_) noexcept;

        /**
         * @fn  explicit yJsonValue(double value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         * @warning 
         */             
        explicit yJsonValue(double value_) noexcept;

        /**
         * @fn  explicit yJsonValue(std::string value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         * @warning 
         */             
        explicit yJsonValue(std::string value_) noexcept;

        /**
         * @fn  explicit yJsonValue(yValueType type_) noexcept
         * @brief override constructor
         * @param type_ the initial type.
         * @return 
         * @warning 
         */             
        explicit yJsonValue(yValueType type_) noexcept;
        
        /**
         * @fn  ~yJsonValue() noexcept
         * @brief default destructor
         * @param 
         * @return 
         */
        ~yJsonValue();

        //copy and move constructor, not deep copy,_json_root_value==value_._json_root_value
        /**
         * @fn  yJsonValue(const yJsonValue &value_)
         * @brief copy constructor
         * @param value_ a exsited obj
         * @return 
         */
        yJsonValue(const yJsonValue & value_) noexcept;
        /**
         * @fn  yJsonValue(const yJsonValue &value_)
         * @brief move constructor
         * @param value_ a exsited obj
         * @return 
         */
        yJsonValue(const yJsonValue && value_) noexcept;

        /**
         * @fn  yJsonValue & operator=(const yJsonValue &value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         * @warning this op will swap value_ and *this, deep copy
         */
        yJsonValue & operator=(const yJsonValue & value_) noexcept;
        /**
         * @fn  yJsonValue & operator=(const yJsonValue &&value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         * @warning this op will swap value_ and *this, deep copy
         */
        yJsonValue & operator=(const yJsonValue && value_) noexcept;


        //other op
        /**
         * @fn  yJsonValue & operator=(int64_t value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         */
        yJsonValue & operator=(int64_t value_) noexcept;
        /**
         * @fn  yJsonValue & operator=(uint64_t value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         */        
        yJsonValue & operator=(uint64_t value_) noexcept;
        /**
         * @fn  yJsonValue & operator=(bool value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         */
        yJsonValue & operator=(bool value_) noexcept;
        /**
         * @fn  yJsonValue & operator=(double value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         */
        yJsonValue & operator=(double value_) noexcept;
        /**
         * @fn  yJsonValue & operator=(const std::string &value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         */        
        yJsonValue & operator=(const std::string &value_) noexcept;
        /**
         * @fn  yJsonValue & operator=(const char *value_) noexcept
         * @brief assignment func
         * @param value_ a exsited obj
         * @return the obj's reference of yJsonValue
         */        
        yJsonValue & operator=(const char *value_) noexcept;

        /**
         * @fn  yJsonValue & operator [](const std::string &key_)
         * @brief search key_'s value
         * @param key_ the key_ to search
         * @return the obj's reference of yJsonValue
         * @warning the same as yJsonValue & operator [](const char *key_). @see yJsonValue & operator [](const char *key_)
         */     
        yJsonValue & operator [](const std::string &key_);
        /**
         * @fn  yJsonValue & operator [](const char *key_)
         * @brief search key_'s value
         * @param key_ the key_ to search
         * @return the obj's reference of yJsonValue
         * @warning if the obj's type is not object-type, this op will throw a exception.
         */     
        yJsonValue & operator [](const char *key_);
        
        /**
         * @fn  yJsonValue & operator [](uint64_t idx_)
         * @brief get idx_'s value of json-array
         * @param idx_ the value's idx
         * @return the obj's reference of yJsonValue
         * @warning if the obj's type is not array-type, this op will throw a exception.
         */             
        yJsonValue & operator [](uint64_t idx_);

        /**
         * @fn operator int64_t() const
         * @brief convert yJsonValue to int64_t
         * @return return a int64_t's val from obj.
         */
        operator int64_t();

        /**
         * @fn operator uint64_t() const
         * @brief convert yJsonValue to uint64_t
         * @return return a uint64_t's val from obj.
         */
        operator uint64_t();

        /**
         * @fn operator bool() const
         * @brief convert yJsonValue to bool
         * @return return a bool's val from obj.
         */
        operator bool();

        /**
         * @fn operator double() const
         * @brief convert yJsonValue to double
         * @return return a double's val from obj.
         */
        operator double();

        /**
         * @fn operator std::string() const
         * @brief convert yJsonValue to std::string
         * @return return a std::string's val from obj.
         */
        operator std::string();

    };

    
}




#endif //__YLIB_UTILITY_YJSON_HPP__