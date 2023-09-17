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
 * @Date: 2019-09-20 17:06:37
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-27 13:44:42
 * @Description: 
 */

#include "core/yexception.hpp"

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#ifdef _WIN32
    //SymGetSymFromAddr64，SymGetLineFromAddr64
#elif __linux__ || __linux

    #ifdef __ANDROID__
        //android don't have execinfo.h
    #else

        #include <execinfo.h>

    #endif //

#endif

#include <stdlib.h>

#ifdef __cplusplus
}
#endif //__cplusplus



YLIB_IMPLEMENT_CLASSINFO_CONTENT(yException)

yLib::yException::yException(int max_stack_item) noexcept 
{

    exception_msg = "";
    max_stack_item_num = max_stack_item;
    get_stack_trace_info();
}

yLib::yException::yException(const std::string & exception_msg, int max_stack_item) noexcept 
{

    this->exception_msg = exception_msg;
    max_stack_item_num = max_stack_item;
    get_stack_trace_info();
}

yLib::yException::~yException() noexcept
{
}

const char* yLib::yException::what() const noexcept 
{

    return exception_msg.c_str();
}

const char * yLib::yException::GetStackTrace(void) noexcept 
{

    return stack_trace_msg.c_str();
}

inline void yLib::yException::get_stack_trace_info(void) noexcept
{

#ifdef _WIN32

    //SymGetSymFromAddr64，SymGetLineFromAddr64
#elif __linux__ || __linux

    #ifdef __ANDROID__
        //android don't have execinfo.h
    #else

        // #include <execinfo.h>

        // int backtrace(void **buffer, int size);

        // -rdynamic put all symbols to .dynsym section
        // char **backtrace_symbols(void *const *buffer, int size);

        // mangle/demangle(c++filt)
        void * _buf[max_stack_item_num];
        char ** _stack_infos;

        int _num_addr = ::backtrace(_buf, max_stack_item_num);

        _stack_infos = ::backtrace_symbols(_buf, _num_addr);
        if (NULL == _stack_infos){

            stack_trace_msg = "";
            return;
        }

        for(int _i = 0; _i < _num_addr; _i ++){

            stack_trace_msg += std::string(_stack_infos[_i]) + "\n";
        }

        ::free(_stack_infos);

    #endif //


    
#endif 
}