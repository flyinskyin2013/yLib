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
#include "ylib.h"

class yStartParam{
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


void ystart(yStartParam & param);
#endif //_YSTART_HPP_