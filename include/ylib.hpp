/*
 * @Author: Sky
 * @Date: 2019-11-20 15:29:40
 * @LastEditors: Sky
 * @LastEditTime: 2019-11-29 11:36:59
 * @Description: 
 */
#ifndef _YLIB_H_
#define _YLIB_H_


#ifdef _WIN32 || _WIN64

#include <core/ylog.hpp>
#include <core/ysharedmemory.hpp>

#elif __linux__ || __linux
    
#include <core/ycommon.hpp>
#include <core/ycurl.hpp>
#include <core/ylog.hpp>
#include <core/yshell.hpp>
#include <core/yxml.hpp>
#include <core/yconfig.hpp>
#include <core/yjson.hpp>

#elif __unix__ || __unix

#endif //__unix__ || __unix



#endif //_YLIB_H_