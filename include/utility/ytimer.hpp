/*
 * @Author: Sky
 * @Date: 2021-04-07 16:29:46
 * @LastEditors: Sky
 * @LastEditTime: 2021-08-31 16:28:11
 * @Description: 
 */
#ifndef __YLIB_UTILITY_YTIMER_HPP__
#define __YLIB_UTILITY_YTIMER_HPP__


#include "core/ycommon.hpp"
#include "utility/ylog.hpp"


#include <chrono>
#include <cstdint>
#include <functional>
#include <thread>
#include <memory>

namespace yLib
{

    

    /**
     *  @class yTimer
     *  @brief This is class to calculate the time that is between time0 and time1.
     *  Besides, it can be as a timer to call a function regularly.
     */
    class __YLIB_CLASS_DECLSPEC__ yTimer MACRO_PUBLIC_INHERIT_YOBJECT
    {
    private:
        /* data */
    public:
        typedef void(*yTimerCallback)(void* params);
           /**
        * @fn  yTimer() noexcept
        * @brief default constructor
        * @param 
        * @return 
        * @warning 
        */
        yTimer() noexcept;

        /**
         * @fn  ~yTimer() noexcept
         * @brief default destructor
         * @param 
         * @return 
         * @warning destructor will block utill timer-thread exit. the block-time depends on StartTimer()'s parameter(time).
         */
        ~yTimer() noexcept;

        /**
         * @fn  void GetTime(uint8_t time_id = 0) noexcept
         * @brief get the current clock for time_id
         * @param time_id the time id, it only can input 0 or 1, if the input is greater 1, we will do nothing.
         * @return none
         * @warning 
         */
        void GetTime(uint8_t time_id = 0) noexcept;

        /**
         * @fn  float GetCalculatedTime(void) noexcept
         * @brief parse json object from memory.
         * @return the time between time0 and time1
         * @warning 
         */
        float GetCalculatedTime(void) noexcept;

        /**
         * @fn  void SetTimerCB(std::function<void(void*)> cb) noexcept
         * @brief set the cb.
         * @param cb the timer callback
         * @return none
         * @warning 
         */
        void SetTimerCB(std::function<void(void*)> cb) noexcept;

        /**
         * @fn  void StartTimer(uint64_t time, void* cb_params = nullptr) noexcept
         * @brief start the timer, then it will call cb regularly.
         * @param time the toggle time(ms), we suggest it should be greater than 5ms.
         * @param cb_params the cb's param.
         * @return none.
         * @warning 
         */
        void StartTimer(uint64_t time, void* cb_params = nullptr) noexcept;

        /**
         * @fn  void StopTimer(void) noexcept
         * @brief stop the timer
         * @return none
         * @warning 
         */
        void StopTimer(void) noexcept;


        /**
         * @fn  bool TimerRunFlag(void) noexcept
         * @brief get the timer's run-status
         * @return the timer's run-status 
         * @retval true timer is running.
         * @retval false timer is stop.
         * @warning 
         */
        bool TimerRunFlag(void) noexcept;
    
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> time0;
        std::chrono::time_point<std::chrono::high_resolution_clock> time1;

        std::shared_ptr<std::thread> timer_thread;
        std::function<void(void*)> timer_callback;

        uint64_t timer_toggle_time;

        bool timer_thread_run_flag;
        bool timer_thread_loop_flag;

        void timer_thread_context(void* params);

        YLIB_DECLARE_CLASSINFO_CONTENT(yTimer);
    };

}





#endif //__YLIB_UTILITY_YTIMER_HPP__