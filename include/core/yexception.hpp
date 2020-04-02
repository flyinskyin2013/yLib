/*
 * @Author: Sky
 * @Date: 2019-09-20 17:06:18
 * @LastEditors: Sky
 * @LastEditTime: 2020-03-26 11:27:14
 * @Description: 
 */
#ifndef _YLIB_CORE_YEXCEPTION_HPP_
#define _YLIB_CORE_YEXCEPTION_HPP_

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

#endif //_YLIB_CORE_YEXCEPTION_