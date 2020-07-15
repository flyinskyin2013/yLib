/*
 * @Author: Sky
 * @Date: 2019-09-20 17:06:18
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:30:18
 * @Description: 
 */
#ifndef __YLIB_CORE_YEXCEPTION_HPP__
#define __YLIB_CORE_YEXCEPTION_HPP__

#include <string>


#include "ycommon.hpp"

namespace yLib{

    struct yExceptionTable
    {
        /* data */
        int _exception_idx;
        std::string _exception_msg;

    };
    
    enum yExceptionIdx{

        IDX_RESERVE_EXCEPTION = 0,
        IDX_YBASIC_EXCEPTION ,
        
    };
    
    class __YLIB_EXPORT__ yException MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        std::string _exception_msg;
    public:
        yException(/* args */) noexcept; 
        yException(/* args */std::string exception_msg) noexcept; 
        virtual ~yException() noexcept; 
        virtual const char * what() noexcept;
    };

}

#endif //__YLIB_CORE_YEXCEPTION_HPP__