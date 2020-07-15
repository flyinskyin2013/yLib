/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 13:50:15
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:31:56
 * @FilePath: \yLib\include\core\ybasicvalue.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __YLIB_CORE_YBASICVALUE_HPP__
#define __YLIB_CORE_YBASICVALUE_HPP__

#include <cstdint>
#include <string>

#include "yobject.hpp"
#include "utility/ylog.hpp"

namespace yLib{

    typedef enum __yvalue_type__:int32_t{

        NONE_YBASICVALUE_TYPE = 0,
        INT64_YBASICVALUE_TYPE = 1,
        UINT64_YBASICVALUE_TYPE = 2,
        BOOL_YBASICVALUE_TYPE = 3,
        FLOAT_YBASICVALUE_TYPE = 4,
        DOUBLE_YBASICVALUE_TYPE = 5,
        STRING_YBASICVALUE_TYPE = 6,

    }yBasicValueType;


    class __YLIB_EXPORT__ yBasicValue MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        yBasicValue() noexcept;
        ~yBasicValue() noexcept;

        //other constructor
        explicit yBasicValue(int64_t value_) noexcept;
        explicit yBasicValue(uint64_t value_) noexcept;
        explicit yBasicValue(bool value_) noexcept;
        explicit yBasicValue(float value_) noexcept;
        explicit yBasicValue(double value_) noexcept;
        explicit yBasicValue(std::string & value_) noexcept;
        explicit yBasicValue(const char * value_) noexcept;

        //copy constructor
        yBasicValue(const yBasicValue & value_) noexcept;
        yBasicValue & operator=(const yBasicValue & value_) noexcept;

        operator int64_t() const;
        operator uint64_t() const;
        operator bool() const;
        operator float() const;
        operator double() const;
        operator std::string() const;
        //operator const char * () const; this will conflict with std::string's constructor

        yBasicValue & operator=(int64_t value_);
        yBasicValue & operator=(uint64_t value_);
        yBasicValue & operator=(bool value_);
        yBasicValue & operator=(float value_);
        yBasicValue & operator=(double value_);
        yBasicValue & operator=(std::string & value_);
        yBasicValue & operator=(std::string && value_);
        yBasicValue & operator=(const char * value_);

        yBasicValueType GetyBasicValueType(void) const;


        struct __value_container__{
            
            int64_t _n_value_int64;
            uint64_t _n_value_uint64;
            bool _b_value_bool;
            float _f_value_float;
            double _f_value_double;
            std::string _str_value_string;
        } _value_containter;

        yBasicValueType _cur_basic_value_type;
        private:
        
    };
}



#endif //__YLIB_CORE_YBASICVALUE_HPP__