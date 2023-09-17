/*
Copyright (c) 2018 - 2023 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Author: Sky
 * @Date: 2023-04-08 13:47:44
 * @LastEditors: Sky
 * @LastEditTime: 2023-04-08 14:23:01
 * @Description: 
 */


#include "core/ythreadpool.hpp"

using namespace yLib;


yThreadPool::yThreadPool(uint64_t max_thread_num) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yThreadPool)
{
    core_thread_num = max_thread_num;

    for (size_t _i = 0; _i < core_thread_num; _i++){

        thread_vec.emplace_back(&yThreadPool::woker_thread_fn, this);
    }

    stop_flag = false;
}

yThreadPool::~yThreadPool()
{
    stop_flag = true;

    condition.notify_all();

    for (auto & _t:thread_vec)
        _t.join();
}

int8_t yThreadPool::add_task(yThreadTask * task) noexcept
{
    return 0;
}


void yThreadPool::woker_thread_fn()
{
    while(true){

        std::function<void()> task;
        {

            std::unique_lock<std::mutex> lock(queue_cxx11_mtx);

            condition.wait(lock, [this](){return !task_queue_cxx11.empty() || stop_flag;});

            if (stop_flag && task_queue_cxx11.empty())//thread pool exit
                break;

            task = std::move(task_queue_cxx11.front());

            task_queue_cxx11.pop();
        }

        //execute task
        task();
    }
}

bool yThreadPool::set_core_thread_num(uint64_t num) noexcept
{
    return 0;
}
bool yThreadPool::set_max_thread_num(uint64_t num) noexcept
{
    return 0;
}
bool yThreadPool::set_keep_alive_max_time(uint64_t time) noexcept
{
    return 0;
}
bool yThreadPool::set_task_queue_num(uint64_t num) noexcept
{
    return 0;
}