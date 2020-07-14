/*
 * @Author: Sky
 * @Date: 2019-11-20 15:29:40
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 16:54:41
 * @Description: 
 */
#ifndef __YLIB_HPP__
#define __YLIB_HPP__

#if defined(_WIN32) || defined(_WIN64)

    //core
    #include "core/ycore.hpp"

    //basic_algorithm
    #include "basic_algorithm/ybasic_algorithm.hpp"

#elif defined(__linux__) || defined(__linux)
    
    //core
    #include "core/ycore.hpp"

    //basic_algorithm
    #include "basic_algorithm/ybasic_algorithm.hpp"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix



#endif //__YLIB_HPP__