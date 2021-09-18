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
        std::function<void(void*)> task;

        yThreadTask(std::function<void(void*)> task, void * task_args = nullptr) noexcept :task(task), task_args(task_args){}
    public:
        yThreadTask(void) noexcept {}
        virtual ~yThreadTask() noexcept{}

        virtual void task(void) noexcept;


        static yThreadTask* create_task(std::function<void(void*)> task, void * task_args = nullptr);
    };
    

}

#endif //__YLIB_CORE_YTHREADTASK_HPP__