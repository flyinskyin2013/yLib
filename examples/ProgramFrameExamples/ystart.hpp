/*
 * @Author: Sky
 * @Date: 2018-10-23 11:14:19
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-07 15:03:57
 * @Description: 
 */

#ifndef _YSTART_HPP_
#define _YSTART_HPP_


#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <error.h>

#ifdef __cplusplus
}
#endif //__cplusplus

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

#include "ylib.hpp"

class __YLIB_DEPRECATED_ATTRIBUTE__ yStartParam{
    public:

};

typedef void * (*ThreadRoutine)(void *);
class yThread{

    public:
    std::string th_name;
    pthread_t th_id;
    pthread_attr_t *th_attr = NULL;
    ThreadRoutine th_routine;
    void * th_param;

    yThread():th_attr(NULL), th_param(NULL){}
    yThread(const yThread & th){ //bug:attr and param do not deep-copy
        
        yLib::yLog::I("deep copy");
        th_name = th.th_name;
        th_id = th.th_id;
        th_attr = th.th_attr;
        th_routine = th.th_routine;
        th_param = th.th_param;
    }
    yThread(yThread && th){

        yLib::yLog::I("rvalue,deep copy");
        th_name = th.th_name;
        th_id = th.th_id;
        th_attr = th.th_attr;
        th_routine = th.th_routine;
        th_param = th.th_param;

        //rvalue
        th.th_attr = NULL;
        th.th_param = NULL;
    }
    yThread & operator = (yThread & th ){ //bug:attr and param do not deep-copy
        
        yLib::yLog::I("operator,deep copy");
        if ( this == &th ){
            return *this;
        }
        th_name = th.th_name;
        th_id = th.th_id;
        th_attr = th.th_attr;
        th_routine = th.th_routine;
        th_param = th.th_param;
        return *this;
    }

    yThread & operator = (yThread && th ){ 

         yLib::yLog::I("operator,rvalue,deep copy");
        if ( this == &th ){
            return *this;
        }
        th_name = th.th_name;
        th_id = th.th_id;
        th_attr = th.th_attr;
        th_routine = th.th_routine;
        th_param = th.th_param;
        return *this;
    }
};

__YLIB_DEPRECATED_ATTRIBUTE__ void ystart(yStartParam & param);




//new entrance


typedef int (*MainContextCallBack)(void * param);

template<class T>
class yMainContext{
    private:
    yMainContext(){}
    public:
    ~yMainContext(){}

    public:
    //Don't use std::function(it needs c++11)
    static int RunMainContext(const MainContextCallBack main_context, T & main_context_param);
};

template<class T>
int yMainContext<T>::RunMainContext(const MainContextCallBack main_context , T & main_context_param){

    return main_context(static_cast<void*>(&main_context_param));;
}

//new test example
int ystart_test(void * param);
#endif //_YSTART_HPP_