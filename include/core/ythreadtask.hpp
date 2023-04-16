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
 * @Date: 2021-09-18 13:54:12
 * @LastEditors: Sky
 * @LastEditTime: 2021-09-18 14:06:11
 * @Description: 
 */

#ifndef __YLIB_CORE_YTHREADTASK_HPP__
#define __YLIB_CORE_YTHREADTASK_HPP__

#include "yobject.hpp"

#include <functional>

namespace yLib{

    class __YLIB_CLASS_DECLSPEC__ yThreadTask:
    YLIB_PUBLIC_INHERIT_YOBJECT
    {
    private:
        void * task_args;
        /* data */
        std::function<void(void*)> task_func;

        yThreadTask(std::function<void(void*)> task, void * task_args = nullptr) noexcept {}
    public:
        yThreadTask(void) noexcept {}
        virtual ~yThreadTask() noexcept{}

        virtual void task(void) noexcept{}


        static yThreadTask* create_task(std::function<void(void*)> task, void * task_args = nullptr);
    };
    

}

#endif //__YLIB_CORE_YTHREADTASK_HPP__