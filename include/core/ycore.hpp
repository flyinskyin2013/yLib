/*
 * @Author: Sky
 * @Date: 2020-07-14 16:43:40
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 17:39:40
 * @Description: 
 */ 

#ifndef __CORE_YCORE_HPP__
#define __CORE_YCORE_HPP__

#if defined(_WIN32) || defined(_WIN64)

    //core
    #include "ycommon.hpp"
    #include "ycurl.hpp"
    #include "ylog.hpp"
    #include "yshell.hpp"
    #include "yxml.hpp"
    #include "yconfig.hpp"
    #include "yjson.hpp"
    #include "ybasicvalue.hpp"
    #include "yexception.hpp"
    #include "yhttp.hpp"
    
    //in test
    #include "yallocator.hpp"

#elif defined(__linux__) || defined(__linux)
    
    //core
    #include "ycommon.hpp"
    #include "ycurl.hpp"
    #include "ylog.hpp"
    #include "yshell.hpp"
    #include "yxml.hpp"
    #include "yconfig.hpp"
    #include "yjson.hpp"
    #include "ybasicvalue.hpp"
    #include "yexception.hpp"
    #include "yhttp.hpp"
    
    //in test
    #include "yallocator.hpp"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix


#endif //__CORE_YCORE_HPP__