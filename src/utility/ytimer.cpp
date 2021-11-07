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
 * @Date: 2021-04-07 16:33:17
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:27:46
 * @Description: 
 */

#include "utility/ytimer.hpp"

using namespace yLib;

YLIB_IMPLEMENT_CLASSINFO_CONTENT(yTimer)


yTimer::yTimer(/* args */) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yTimer),
timer_toggle_time(0),
timer_thread_run_flag(false),
timer_thread_loop_flag(false)
{
}

yTimer::~yTimer() noexcept
{
    do//wait timer thread exit
    {
        StopTimer();
#if defined(_WIN32) || defined(_WIN64)

        ::Sleep(1);

#elif defined(__linux__) || defined(__linux)

        usleep(1000);
    
#elif defined(__unix__) || defined(__unix)

    
#endif //__unix__ || __unix     
        
    } while (TimerRunFlag());//last thread stop       
}

void yTimer::GetTime(uint8_t time_id) noexcept
{
    switch (time_id)
    {
        case 0:{
            /* code */
            time0 = std::chrono::high_resolution_clock::now();
            break;
        }
        case 1:{
            /* code */
            time1 = std::chrono::high_resolution_clock::now();
            break;
        }
        default:{
            break;
        }
    }
    
    return ;
}

float yTimer::GetCalculatedTime(void) noexcept
{

    return (float)std::chrono::duration_cast<std::chrono::microseconds>(time1 - time0).count() / 1000;
}

void yTimer::SetTimerCB(std::function<void(void*)> cb) noexcept
{

    timer_callback = cb;
}

void yTimer::timer_thread_context(void* params)
{

    timer_thread_run_flag = true;
    timer_thread_loop_flag = true;

    while (timer_thread_loop_flag)
    {

#if defined(_WIN32) || defined(_WIN64)

        ::Sleep(timer_toggle_time);

#elif defined(__linux__) || defined(__linux)
    
        usleep(1000*timer_toggle_time);
        
#elif defined(__unix__) || defined(__unix)

        
#endif //__unix__ || __unix
        
        if (timer_callback != nullptr)
            timer_callback(params);
 
    }

    timer_thread_run_flag = false;
}

void yTimer::StartTimer(uint64_t time, void* cb_params) noexcept
{

    do
    {
        StopTimer();

#if defined(_WIN32) || defined(_WIN64)

        ::Sleep(1);

#elif defined(__linux__) || defined(__linux)
    
        usleep(1000);
        
#elif defined(__unix__) || defined(__unix)

        
#endif //__unix__ || __unix
        
    } while (timer_thread_run_flag);//last thread stop

    timer_toggle_time = time;

    timer_thread = std::make_shared<std::thread>(&yTimer::timer_thread_context, this, cb_params);

    timer_thread->detach();

}

void yTimer::StopTimer(void) noexcept
{
    timer_thread_loop_flag = false;
}


bool yTimer::TimerRunFlag(void) noexcept
{

    return timer_thread_run_flag;
}