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