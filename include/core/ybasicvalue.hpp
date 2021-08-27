/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-19 13:50:15
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 15:52:01
 * @FilePath: \yLib\include\core\ybasicvalue.hpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#ifndef __YLIB_CORE_YBASICvalueHPP__
#define __YLIB_CORE_YBASICvalueHPP__

#include <cstdint>
#include <string>

#include "yobject.hpp"
#include "utility/ylog.hpp"

namespace yLib{

    /**
     *  @class yValue
     *  @brief This is base-class of the all yXxxValues in yLib.
     *  It supports int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t/int64_t/uint64_t/ 
     *              float/double/bool/std::string/object_val/array_val/group_val/null_val/ptr_val 
     */
    class __YLIB_EXPORT__ yValue:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
        public:

        /**
         *  @typedef 
         *  @brief typedef __yvaluetype__ to yValueType.
         *  @warning 
         */
        typedef enum __yvaluetype__:uint8_t{

            NONE_TYPE = 0,

            //------------------------int-------------------
            //int8_t/uint8_t
            INT8_TYPE = 1,
            UINT8_TYPE = 2,

            //int16_t/uint16_t
            INT16_TYPE ,
            UINT16_TYPE ,

            //int32_t/uint32_t
            INT32_TYPE ,
            UINT32_TYPE ,

            //int64_t/uint64_t
            INT64_TYPE ,
            UINT64_TYPE ,
            //------------------------int-------------------

            //------------------------float-------------------
            FLOAT_TYPE,
            DOUBLE_TYPE,
            //------------------------float-------------------

            //------------------------boolean-------------------
            BOOL_TYPE,
            //------------------------boolean-------------------

            //------------------------string-------------------
            STRING_TYPE,
            //------------------------string-------------------

            //------------------------special-------------------
            //for yJson
            OBJECT_TYPE,
            //for yJson
            ARRAY_TYPE,
            //for yJson
            NULL_TYPE,
            //for yConfig
            GROUP_TYPE,

            //for usual ptr
            POINTER_TYPE
            //------------------------string-------------------
        }yValueType;

        public:
        
        /**
         * @fn  yValue() noexcept
         * @brief default constructor
         * @param 
         * @return 
         */
        yValue() noexcept;

        /**
         * @fn  yValue(const yValue &value)
         * @brief copy constructor
         * @param value a exsited obj
         * @return 
         */
        yValue(const yValue &value) noexcept;

        /**
         * @fn  yValue & operator=(const yValue &value) noexcept
         * @brief assignment constructor
         * @param value a exsited obj
         * @return the obj's reference of yValue
         */
        yValue & operator=(const yValue &value) noexcept;

        /**
         * @fn  yValue(const yValue &&value) noexcept
         * @brief move constructor
         * @param value a exsited obj
         * @return 
         */
        yValue(const yValue &&value) noexcept;

        /**
         * @fn  yValue & operator=(const yValue &&value) noexcept
         * @brief assignment constructor
         * @param value a exsited obj
         * @return the obj's reference of yValue
         */
        yValue & operator=(const yValue &&value) noexcept;

        /**
         * @fn  virtual ~yValue() noexcept
         * @brief default destructor
         * @param 
         * @return 
         */
        virtual ~yValue() noexcept;
        
        //other constructor
        /**
         * @fn  explicit yValue(int8_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(int8_t value) noexcept;

        /**
         * @fn  explicit yValue(uint8_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(uint8_t value) noexcept;

        /**
         * @fn  explicit yValue(int16_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(int16_t value) noexcept;

        /**
         * @fn  explicit yValue(uint16_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(uint16_t value) noexcept;


        /**
         * @fn  explicit yValue(int32_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(int32_t value) noexcept;

        /**
         * @fn  explicit yValue(uint32_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(uint32_t value) noexcept;

        /**
         * @fn  explicit yValue(int64_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(int64_t value) noexcept;

        /**
         * @fn  explicit yValue(uint64_t value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(uint64_t value) noexcept;

        /**
         * @fn  explicit yValue(bool value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(bool value) noexcept;

        /**
         * @fn  explicit yValue(float value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(float value) noexcept;

        /**
         * @fn  explicit yValue(double value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(double value) noexcept;

        /**
         * @fn  explicit yValue(std::string &value) noexcept
         * @brief override constructor
         * @param value the initial val.
         * @return 
         */
        explicit yValue(const std::string & value) noexcept;

        /**
         * @fn  yValue(yValueType type_, void * value) noexcept
         * @brief override constructor
         * @param type the type of value.
         * @param value the initial val.
         * @return 
         */
        yValue(yValueType type, void * value) noexcept;



        //basic convert yValue to basic value-type
        /**
         * @fn  virtual operator int8_t() const noexcept
         * @brief convert yValue to int8_t
         * @return return a int8_t's val from obj.
         */
        virtual operator int8_t() const noexcept;

        /**
         * @fn virtual operator uint8_t() const noexcept
         * @brief convert yValue to uint8_t
         * @return return a uint8_t's val from obj.
         */
        virtual operator uint8_t() const noexcept;

        /**
         * @fn  virtual operator int16_t() const noexcept
         * @brief convert yValue to int16_t
         * @return return a int16_t's val from obj.
         */
        virtual operator int16_t() const noexcept;

        /**
         * @fn  virtual operator uint16_t() const noexcept
         * @brief convert yValue to uint16_t
         * @return return a uint16_t's val from obj.
         */
        virtual operator uint16_t() const noexcept;


        /**
         * @fn  virtual operator int32_t() const noexcept
         * @brief convert yValue to int32_t
         * @return return a int32_t's val from obj.
         */
        virtual operator int32_t() const noexcept;

        /**
         * @fn  virtual operator uint32_t() const noexcept
         * @brief convert yValue to uint32_t
         * @return return a uint32_t's val from obj.
         */
        virtual operator uint32_t() const noexcept;

        /**
         * @fn  virtual operator int64_t() const noexcept
         * @brief convert yValue to int64_t
         * @return return a int64_t's val from obj.
         */
        virtual operator int64_t() const noexcept;

        /**
         * @fn  virtual operator uint64_t() const noexcept
         * @brief convert yValue to uint64_t
         * @return return a uint64_t's val from obj.
         */
        virtual operator uint64_t() const noexcept;

        /**
         * @fn  virtual operator bool() const noexcept
         * @brief convert yValue to bool
         * @return return a bool's val from obj.
         */
        virtual operator bool() const noexcept;

        /**
         * @fn  virtual operator float() const noexcept
         * @brief convert yValue to float
         * @return return a float's val from obj.
         */
        virtual operator float() const noexcept;

        /**
         * @fn  virtual operator double() const noexcept
         * @brief convert yValue to double
         * @return return a double's val from obj.
         */
        virtual operator double() const noexcept;

        /**
         * @fn  virtual operator std::string() const noexcept
         * @brief convert yValue to std::string
         * @return return a std::string's val from obj.
         */
        virtual operator std::string() const noexcept;
        //operator const char * () const; this will conflict with std::string's constructor

        //other assignment operator
        /**
         * @fn  virtual yValue & operator=(int8_t value) noexcept
         * @brief convert int8_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(int8_t value) noexcept;
        /**
         * @fn  virtual yValue & operator=(uint8_t value) noexcept
         * @brief convert uint8_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(uint8_t value) noexcept;

        /**
         * @fn  virtual yValue & operator=(int16_t value) noexcept
         * @brief convert int16_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(int16_t value) noexcept;
        /**
         * @fn  virtual yValue & operator=(uint16_t value) noexcept
         * @brief convert uint16_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(uint16_t value) noexcept;

        /**
         * @fn  virtual yValue & operator=(int32_t value) noexcept
         * @brief convert int32_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(int32_t value) noexcept;
        /**
         * @fn  virtual yValue & operator=(uint32_t value) noexcept
         * @brief convert uint32_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(uint32_t value) noexcept;

        /**
         * @fn  virtual yValue & operator=(int64_t value) noexcept
         * @brief convert int64_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(int64_t value) noexcept;
        /**
         * @fn  virtual yValue & operator=(uint64_t value) noexcept
         * @brief convert uint64_t to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(uint64_t value) noexcept;

        /**
         * @fn  virtual yValue & operator=(float value) noexcept
         * @brief convert float to exsited obj of yValue
         * @return return obj of yValue
         */        
        virtual yValue & operator=(float value) noexcept;
        /**
         * @fn  virtual yValue & operator=(double value) noexcept
         * @brief convert double to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(double value) noexcept;

        /**
         * @fn  virtual yValue & operator=(bool value) noexcept
         * @brief convert bool to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(bool value) noexcept;

        /**
         * @fn  virtual yValue & operator=(std::string value) noexcept
         * @brief convert std::string to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(const std::string &value) noexcept;

        /**
         * @fn  virtual yValue & operator=(const char *value) noexcept
         * @brief convert char * to exsited obj of yValue
         * @return return obj of yValue
         */
        virtual yValue & operator=(const char *value) noexcept;

        /**
         * @fn  virtual yValueType GetType(void) const noexcept
         * @brief get the val-type of obj.
         * @return return the yValueType of yValue's obj.
         */
        virtual yValueType GetType(void) const noexcept;

        /**
         * @fn  virtual void * GetSpecialValue(void) const noexcept
         *
         * @brief get the val-type of obj.
         * 
         * @return the value of special type 
         * @retval other_addr ok.
         * @retval nullptr error.
         * 
         */
        virtual void * GetSpecialValue(void) const noexcept;

        protected:
        /**
         * @fn  virtual void CleanAllToDefault(void) noexcept
         * @brief set obj of yValue to default.
         * @return 
         * @warning this op will modify type and value-container
         */
        virtual void CleanAllToDefault(void) noexcept;

        /**
         * @fn  static void CopyValueContainer(const yValue & src, yValue & dst) noexcept
         * @brief copy src_'s value-container to dst_'s value-container
         * @param src_ the value-container copy from
         * @param dst_ the value-container copy to
         * @return 
         */
        static void CopyValueContainer(const yValue & src, yValue & dst) noexcept;

        /**
         * @struct  struct __valuecontainer__
         * @brief the container of yValue
         */
        struct __valuecontainer__{
            
            //merge int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t into int64_t/uint64_t
            int64_t int64_val;
            uint64_t uint64_val;    

            // 
            float float_val;
            double double_val;

            // 
            bool bool_val;

            std::string string_val;

            // merge object_val/array_val/group_val/null_val into  ptr_val  
            //special val type
            //usual pointer
            uintptr_t ptr_val;

        } value_containter;

        /**
         * @var  yValueType cur_valuetype
         * @brief the current val-type of yValue
         */
        yValueType cur_value_type;


        YLIB_DECLARE_CLASSINFO_CONTENT(yValue);
    };

    typedef yValue yBasicValue;
}



#endif //__YLIB_CORE_YBASICvalueHPP__