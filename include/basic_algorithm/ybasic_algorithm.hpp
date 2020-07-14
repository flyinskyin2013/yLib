/*
 * @Author: Sky
 * @Date: 2020-07-14 16:48:42
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-14 16:56:35
 * @Description: 
 */ 

#ifndef __YBASIC_ALGORITHM_HPP__
#define __YBASIC_ALGORITHM_HPP__

#if defined(_WIN32) || defined(_WIN64)

    #pragma warning(disable:4067) //warning C4067: 预处理器指令后有意外标记 - 应输入换行符
    #pragma warning(disable:4251) //warning C4251: xxxxx需要有dll接口由xxxx的客户端使用

    //basic_algorithm
    #include "ybinary_tree.hpp"
    #include "ydynamic_search.hpp"
    #include "yexternal_sorting.hpp"
    #include "ygeneralized_list.hpp"
    #include "ygraph_toolbox.hpp"
    #include "yinternal_sorting.hpp"
    #include "ylinear_list.hpp"
    #include "ylinked_list.hpp"
    #include "ymemory_manage.hpp"
    #include "yqueue.hpp"
    #include "ysparse_matrix.hpp"
    #include "ystack.hpp"
    #include "ystatic_search.hpp"
    #include "ystring_toolbox.hpp"

#elif defined(__linux__) || defined(__linux)
    
    //basic_algorithm
    #include "ybinary_tree.hpp"
    #include "ydynamic_search.hpp"
    #include "yexternal_sorting.hpp"
    #include "ygeneralized_list.hpp"
    #include "ygraph_toolbox.hpp"
    #include "yinternal_sorting.hpp"
    #include "ylinear_list.hpp"
    #include "ylinked_list.hpp"
    #include "ymemory_manage.hpp"
    #include "yqueue.hpp"
    #include "ysparse_matrix.hpp"
    #include "ystack.hpp"
    #include "ystatic_search.hpp"
    #include "ystring_toolbox.hpp"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix



#endif //__YBASIC_ALGORITHM_HPP__