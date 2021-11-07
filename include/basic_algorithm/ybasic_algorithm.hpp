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