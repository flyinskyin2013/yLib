/*
 * @Author: Sky
 * @Date: 2019-11-20 15:29:40
 * @LastEditors: Sky
 * @LastEditTime: 2020-05-28 14:51:35
 * @Description: 
 */
#ifndef _YLIB_H_
#define _YLIB_H_

#ifdef _WIN32

    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用
#endif //_WIN32

#ifdef _WIN32

    //core
    #include <core/ylog.hpp>
    #include <core/yjson.hpp>
    #include <core/ysharedmemory.hpp>
    #include <core/ybasicvalue.hpp>
    #include <core/yxml.hpp>
    #include <core/yconfig.hpp>
    #include <core/yjson.hpp>
    #include <core/yexception.hpp>
    #include <core/yhttp.hpp>

    //basic_algorithm
    #include <basic_algorithm/ysequence_list.hpp>

    //in test
    #include <core/yallocator.hpp>
#elif __linux__ || __linux
    
    //core
    #include <core/ycommon.hpp>
    #include <core/ycurl.hpp>
    #include <core/ylog.hpp>
    #include <core/yshell.hpp>
    #include <core/yxml.hpp>
    #include <core/yconfig.hpp>
    #include <core/yjson.hpp>
    #include <core/ybasicvalue.hpp>
    #include <core/yexception.hpp>
    #include <core/yhttp.hpp>
    

    //basic_algorithm
    #include <basic_algorithm/ysequence_list.hpp>

    //in test
    #include <core/yallocator.hpp>

#elif __unix__ || __unix

#endif //__unix__ || __unix



#endif //_YLIB_H_