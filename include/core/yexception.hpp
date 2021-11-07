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
 * @Date: 2019-09-20 17:06:18
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 09:28:15
 * @Description: 
 */
#ifndef __YLIB_CORE_YEXCEPTION_HPP__
#define __YLIB_CORE_YEXCEPTION_HPP__

#include <string>
#include <exception>


#include "ycommon.hpp"

namespace yLib{

    /**
     *  @class yException
     *  @brief This is base-class of the all exception in yLib.
     */    
    class __YLIB_CLASS_DECLSPEC__ yException:
    YLIB_PUBLIC_INHERIT_YOBJECT,
    public std::exception
    {
    private:
        /* data */
        /**
         *  @var exception_msg
         *  @brief The exception msg.
         */   
        std::string exception_msg;

        /**
         *  @var stack_trace_msg
         *  @brief The stack trace msg.
         */   
        std::string stack_trace_msg;

        /**
         *  @var max_stack_item_num
         *  @brief The num of stack item.
         */   
        int max_stack_item_num = 0;

        /**
         *  @fn    inline void get_stack_trace_info(void) noexcept
         *  @brief get stack trace info to stack_trace_msg
         */
        inline void get_stack_trace_info(void) noexcept;

    public:
        /**
         *  @fn    yException(int max_stack_item = 16) noexcept
         *  @brief Default constructor
         */
        yException(int max_stack_item = 16) noexcept; 

        /**
         *  @fn    yException(const std::string & exception_msg, int max_stack_item = 16) noexcept
         *  @brief Default constructor(overide)
         *  @param exception_msg the exception msg.
         */
        yException(const std::string & exception_msg, int max_stack_item = 16) noexcept; 

        /**
         *  @fn    virtual ~yException() noexcept
         *  @brief Default destructor
         */
        virtual ~yException() noexcept; 

        /**
         *  @fn    virtual const char* what() const noexcept override
         *  @brief get the exception msg-str.
         *  @return the exception msg-str
         */
        virtual const char* what() const noexcept override;

        /**
         *  @fn    virtual const char * GetStackTrace(void) noexcept
         *  @brief get the exception stack-trace-info.
         *  @return the exception stack-trace-info
         */
        virtual const char * GetStackTrace(void) noexcept;


        YLIB_DECLARE_CLASSINFO_CONTENT(yException);
    };

}

#endif //__YLIB_CORE_YEXCEPTION_HPP__