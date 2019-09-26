/*
 * @Author: Sky
 * @Date: 2019-09-20 17:06:37
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-20 20:35:34
 * @Description: 
 */

#include "yexception.hpp"

static  yLib::yExceptionTable g_yexception_table[]={
    
    REGISTER_EXCEPTION(yLib::yExceptionIdx::IDX_RESERVE_EXCEPTION, Reserved exception of yLib.),
    REGISTER_EXCEPTION(yLib::yExceptionIdx::IDX_YEXCEPTION, Basic exception of yLib.),
};



yLib::yException::yException() noexcept MACRO_INIT_YOBJECT_PROPERTY(yException) {


    _exception_msg  = g_yexception_table[yLib::yExceptionIdx::IDX_YEXCEPTION]._exception_msg;
}

yLib::yException::yException(/* args */std::string exception_msg) noexcept MACRO_INIT_YOBJECT_PROPERTY(yException) {

    _exception_msg = exception_msg;
}

yLib::yException::~yException() noexcept{

    
}

const char * yLib::yException::what() noexcept {

    return _exception_msg.c_str();
}