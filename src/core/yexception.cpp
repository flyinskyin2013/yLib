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

#include <execinfo.h>
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

    // int backtrace(void **buffer, int size);

    // -rdynamic put all symbols to .dynsym section
    // char **backtrace_symbols(void *const *buffer, int size);

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
    
#endif 
}