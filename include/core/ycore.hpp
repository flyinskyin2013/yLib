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
 * @Date: 2020-07-14 16:43:40
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-26 18:17:12
 * @Description: 
 */ 

#ifndef __YLIB_CORE_YCORE_HPP__
#define __YLIB_CORE_YCORE_HPP__

#if defined(_WIN32) || defined(_WIN64)

    //core
    #include "ycommon.hpp"
    #include "ybasicvalue.hpp"
    #include "yexception.hpp"
    #include "ylib_extra_infos.hpp"
    #include "yobject.hpp"
    #include "ylog/ylog.hpp"
    #include "ythreadpool.hpp"
    
    //in test
    #include "yallocator.hpp"

#elif defined(__linux__) || defined(__linux)
    
    //core
    #include "ycommon.hpp"
    #include "ybasicvalue.hpp"
    #include "yexception.hpp"
    #include "ylib_extra_infos.hpp"
    #include "yobject.hpp"
    #include "ylog/ylog.hpp"
    #include "ythreadpool.hpp"
    
    //in test
    #include "yallocator.hpp"
    
#elif defined(__unix__) || defined(__unix)

#endif //__unix__ || __unix


#endif //__YLIB_CORE_YCORE_HPP__