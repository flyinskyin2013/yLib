/*
 * @Author: Sky
 * @Date: 2021-04-07 16:33:17
 * @LastEditors: Sky
 * @LastEditTime: 2021-04-09 16:01:00
 * @Description: 
 */

#include "utility/ytimer.hpp"

using namespace yLib;

yTimer::yTimer(/* args */) noexcept
MACRO_INIT_YOBJECT_PROPERTY(yTimer),
timer_toggle_time(0),
timer_thread_run_flag(false),
timer_thread_loop_flag(false)
{
}

yTimer::~yTimer() noexcept
{

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


#elif defined(__linux__) || defined(__linux)
    
        usleep(1000*timer_toggle_time);
        
#elif defined(__unix__) || defined(__unix)

        ::Sleep(timer_toggle_time);
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
        usleep(500);
        
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