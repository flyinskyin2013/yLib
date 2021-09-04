/*
 * @Author: Sky
 * @Date: 2020-07-14 18:11:07
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 20:01:52
 * @Description: 
 */ 
#ifndef __YLIB_UTILITY_YUTILITY_HPP__
#define __YLIB_UTILITY_YUTILITY_HPP__

    #if defined(_WIN32) || defined(_WIN64)

        #include "yhttp.hpp"
        // #include "yjson.hpp"
        #include "ylog.hpp"
        // #include "yconfig.hpp"
        #include "ytimer.hpp"

    #elif defined(__linux__) || defined(__linux)
        
        // #include "yconfig.hpp"
        #include "yhttp.hpp"
        // #include "yjson.hpp"
        #include "ylog.hpp"
        #include "yshell.hpp"
        // #include "yxml.hpp"
        #include "ytimer.hpp"
        
    #elif defined(__unix__) || defined(__unix)

    #endif //__unix__ || __unix

#endif //__YLIB_UTILITY_YUTILITY_HPP__