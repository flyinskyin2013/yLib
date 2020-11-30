/*
 * @Author: Sky
 * @Date: 2019-09-20 17:06:18
 * @LastEditors: Sky
 * @LastEditTime: 2020-11-30 16:46:00
 * @Description: 
 */
#ifndef __YLIB_CORE_YEXCEPTION_HPP__
#define __YLIB_CORE_YEXCEPTION_HPP__

#include <string>


#include "ycommon.hpp"

namespace yLib{

    /**
     *  @struct yExceptionTable
     *  @brief This is a exception table.
     */   
    struct yExceptionTable
    {
        /* data */
        int _exception_idx;
        std::string _exception_msg;

    };
    
    /**
     *  @enum yExceptionIdx
     *  @brief This is a index of exception table
     */   
    enum yExceptionIdx{

        IDX_RESERVE_EXCEPTION = 0,
        IDX_YBASIC_EXCEPTION ,
        
    };
    
    /**
     *  @class yException
     *  @brief This is base-class of the all exception in yLib.
     */    
    class __YLIB_EXPORT__ yException MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        /**
         *  @var _exception_msg
         *  @brief The exception msg.
         */   
        std::string _exception_msg;
    public:
        /**
         *  @fn    yException()
         *  @brief Default constructor
         */
        yException() noexcept; 

        /**
         *  @fn    yException()
         *  @brief Default constructor(overide)
         *  @param exception_msg the exception msg.
         */
        yException(std::string exception_msg) noexcept; 

        /**
         *  @fn    virtual ~yException() noexcept
         *  @brief Default destructor
         */
        virtual ~yException() noexcept; 

        /**
         *  @fn    virtual const char * what() noexcept
         *  @brief get the exception msg-str.
         *  @return the exception msg-str
         */
        virtual const char * what() noexcept;
    };

}

#endif //__YLIB_CORE_YEXCEPTION_HPP__