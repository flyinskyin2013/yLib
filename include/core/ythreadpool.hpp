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
 * @Date: 2021-09-18 13:47:44
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 14:23:01
 * @Description: 
 */

#ifndef __YLIB_CORE_YTHREADPOOL_HPP__
#define __YLIB_CORE_YTHREADPOOL_HPP__

#include "yobject.hpp"
#include "ythreadtask.hpp"

#include <cstdint>
#include <queue>

namespace yLib
{
    class __YLIB_CLASS_DECLSPEC__ yThreadPool:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
        std::queue<yThreadTask*> task_queue;
        uint64_t core_thread_num = 2;
        uint64_t max_thread_num = 5;
        uint64_t keep_alive_max_time = 60;//(s)
        uint64_t max_task_queue_size = 20;
    public:
        yThreadPool(void)noexcept;
        ~yThreadPool()noexcept;

        bool set_core_thread_num(uint64_t num) noexcept;
        bool set_max_thread_num(uint64_t num) noexcept;
        bool set_keep_alive_max_time(uint64_t time) noexcept;
        bool set_task_queue_num(uint64_t num) noexcept;
        
        int8_t add_task(yThreadTask * task) noexcept;    

    };
    
} // namespace yLib



#endif //__YLIB_CORE_YTHREADPOOL_HPP__