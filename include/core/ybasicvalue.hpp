/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 13:50:15
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 16:04:05
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

    /**
     *  @typedef 
     *  @brief typedef __yvalue_type__ to yBasicValueType.
     */
    typedef enum __yvalue_type__:int32_t{

        NONE_YBASICVALUE_TYPE = 0,
        INT64_YBASICVALUE_TYPE = 1,
        UINT64_YBASICVALUE_TYPE = 2,
        BOOL_YBASICVALUE_TYPE = 3,
        FLOAT_YBASICVALUE_TYPE = 4,
        DOUBLE_YBASICVALUE_TYPE = 5,
        STRING_YBASICVALUE_TYPE = 6,

    }yBasicValueType;


    /**
     *  @class yBasicValue
     *  @brief This is base-class of the all yXxxValues in yLib.
     */
    class __YLIB_EXPORT__ yBasicValue MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        
        /**
         * @fn  yBasicValue() noexcept
         * @brief default constructor
         * @param 
         * @return 
         */
        yBasicValue() noexcept;

        /**
         * @fn  ~yBasicValue() noexcept
         * @brief default destructor
         * @param 
         * @return 
         */
        ~yBasicValue() noexcept;

        //other constructor

        /**
         * @fn  explicit yBasicValue(int64_t value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(int64_t value_) noexcept;

        /**
         * @fn  explicit yBasicValue(uint64_t value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(uint64_t value_) noexcept;

        /**
         * @fn  explicit yBasicValue(bool value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(bool value_) noexcept;

        /**
         * @fn  explicit yBasicValue(float value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(float value_) noexcept;

        /**
         * @fn  explicit yBasicValue(double value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(double value_) noexcept;

        /**
         * @fn  explicit yBasicValue(std::string value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(std::string & value_) noexcept;

        /**
         * @fn  explicit yBasicValue(const char * value_) noexcept
         * @brief override constructor
         * @param value_ the initial val.
         * @return 
         */
        explicit yBasicValue(const char * value_) noexcept;

        //copy constructor
        /**
         * @fn  yBasicValue(const yBasicValue & value_) noexcept
         * @brief copy constructor
         * @param value_ the initial val.
         * @return 
         */
        yBasicValue(const yBasicValue & value_) noexcept;

        /**
         * @fn  yBasicValue & operator=(const yBasicValue & value_) noexcept
         * @brief assignment constructor
         * @param value_ the initial val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(const yBasicValue & value_) noexcept;

        /**
         * @fn  operator int64_t() const
         * @brief convert yBasicValue to int64_t
         * @return return a int64_t's val from obj.
         */
        operator int64_t() const;

        /**
         * @fn  operator uint64_t() const
         * @brief convert yBasicValue to uint64_t
         * @return return a uint64_t's val from obj.
         */
        operator uint64_t() const;

        /**
         * @fn  operator bool() const
         * @brief convert yBasicValue to bool
         * @return return a bool's val from obj.
         */
        operator bool() const;

        /**
         * @fn  operator float() const
         * @brief convert yBasicValue to float
         * @return return a float's val from obj.
         */
        operator float() const;

        /**
         * @fn  operator double() const
         * @brief convert yBasicValue to double
         * @return return a double's val from obj.
         */
        operator double() const;

        /**
         * @fn  operator std::string() const
         * @brief convert yBasicValue to std::string
         * @return return a std::string's val from obj.
         */
        operator std::string() const;
        //operator const char * () const; this will conflict with std::string's constructor


        /**
         * @fn  yBasicValue & operator=(int64_t value_)
         * @brief convert int64_t to yBasicValue
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(int64_t value_);

        /**
         * @fn  yBasicValue & operator=(uint64_t value_)
         * @brief convert uint64_t to yBasicValue
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(uint64_t value_);

        /**
         * @fn  yBasicValue & operator=(bool value_)
         * @brief convert bool to yBasicValue
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(bool value_);

        /**
         * @fn  yBasicValue & operator=(float value_)
         * @brief convert float to yBasicValue
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(float value_);

        /**
         * @fn  yBasicValue & operator=(double value_)
         * @brief convert double to yBasicValue
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(double value_);

        /**
         * @fn  yBasicValue & operator=(std::string & value_)
         * @brief convert std::string to yBasicValue
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(std::string & value_);

        /**
         * @fn  yBasicValue & operator=(std::string && value_)
         * @brief convert std::string to yBasicValue(override)
         * @param value_ the input val.
         * @return return a yBasicValue's obj.
         */
        yBasicValue & operator=(std::string && value_);

        // /**
        //  * @fn  yBasicValue & operator=(std::string && value_)
        //  * @brief convert const char * value_ to yBasicValue(override)
        //  * @param value_ the input val.
        //  * @return return a yBasicValue's obj.
        //  */
        // yBasicValue & operator=(const char * value_);


        /**
         * @fn  yBasicValueType GetyBasicValueType(void) const
         * @brief get the val-type of obj.
         * @return return the yBasicValueType of yBasicValue's obj.
         */
        yBasicValueType GetyBasicValueType(void) const;

        /**
         * @struct  struct __value_container__
         * @brief the container of yBasicValue
         */
        struct __value_container__{
            
            int64_t _n_value_int64;
            uint64_t _n_value_uint64;
            bool _b_value_bool;
            float _f_value_float;
            double _f_value_double;
            std::string _str_value_string;
        } _value_containter;

        /**
         * @var  yBasicValueType _cur_basic_value_type
         * @brief the current val-type of yBasicValue
         */
        yBasicValueType _cur_basic_value_type;
        private:
        
    };
}



#endif //__YLIB_CORE_YBASICVALUE_HPP__