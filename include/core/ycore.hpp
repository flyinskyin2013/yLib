/*
 * @Author: Sky
 * @Date: 2020-07-14 16:43:40
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 18:33:02
 * @Description: 
 */ 

#ifndef __YLIB_CORE_YCORE_HPP__
#define __YLIB_CORE_YCORE_HPP__

#if defined(_WIN32) || defined(_WIN64)

    //core
    #include "ycommon.hpp"
    #include "ybasicvalue.hpp"
    #include "yexception.hpp"

    
    //in test
    #include "yallocator.hpp"

#elif defined(__linux__) || defined(__linux)
    
    //core
    #include "ycommon.hpp"
    #include "ybasicvalue.hpp"
    #include "yexception.hpp"

    
    //in test
    #include "yallocator.hpp"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix


#endif //__YLIB_CORE_YCORE_HPP__