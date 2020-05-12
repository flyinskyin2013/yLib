/*
 * @Author: Sky
 * @Date: 2018-08-22 15:39:32
 * @LastEditors: Sky
 * @LastEditTime: 2020-04-14 15:05:56
 * @Description: 
 */

#include "ystart.hpp"

//The vector of total thread
std::vector<yThread> g_thread_vector;

// #define yCreateThread(name, attr,routine, param) \
//     do{ \
//         yThread _thread; \
//         int ret = 0; \
//         _thread.th_name = name; \
//         _thread.th_attr = attr; \
//         _thread.th_routine = routine; \
//         _thread.th_param = param; \
//         g_thread_vector.push_back(_thread); \
//         if ( 0 > (ret = pthread_create(&_thread.th_id, _thread.th_attr, _thread.th_routine, _thread.th_param)) ){ \
//             std::string msg = "Thread: " + _thread.th_name + " Create failed."; \
//             yLib::yLog::E(msg.c_str()); \
//         } \
//         else{ \
//             std::string msg = "thread create successully, id is " + std::to_string(_thread.th_id); \
//             yLib::yLog::I(msg.c_str()); \
//          } \
//     }while(0)

void yCreateThread(std::string & name, pthread_attr_t * attr, ThreadRoutine routine, void * param);
void yCleanThread(void);

void * test_thread(void * arg){

    //
    void * ret;
    yLib::yLog::I(" test thread successfully.");
    //pthread_detach(getpid());
    return ret;
}


void ystart(yStartParam & param){

/************************************/
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    std::string th_name = "TestThread";
    yCreateThread(th_name, &attr, test_thread, NULL);


    sleep(1);
/************************************/



    //clean all joinable threads.
    yCleanThread();

/************************************/
    // TODO
    pthread_attr_destroy(&attr);
/************************************/


    //exit sub process
    ::exit(0);
}

void ystart_test(void * param){

    yStartParam & tmp_param = *(yStartParam *)param;
    /************************************/
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    std::string th_name = "TestThread";
    yCreateThread(th_name, &attr, test_thread, NULL);


    sleep(1);
/************************************/



    //clean all joinable threads.
    yCleanThread();

/************************************/
    // TODO
    pthread_attr_destroy(&attr);
/************************************/


    //exit sub process
    ::exit(0);

}




void yCreateThread(std::string & name, pthread_attr_t * attr, ThreadRoutine routine, void * param){

    yThread _thread; 
    int ret = 0; 
    _thread.th_name = name; 
    _thread.th_attr = attr; 
    _thread.th_routine = routine; 
    _thread.th_param = param; 
    
    if ( 0 > (ret = pthread_create(&_thread.th_id, _thread.th_attr, _thread.th_routine, _thread.th_param)) ){ 
        
        yLib::yLog::E("Thread: %s create failed.", _thread.th_name.c_str()); 
    } 
    else{ 
    
        yLib::yLog::I("Thread %s create successully, id is %u", _thread.th_name.c_str(), _thread.th_id); 
        g_thread_vector.push_back(std::move(_thread)); 
    } 
}

void yCleanThread(void){

    if ( g_thread_vector.size() == 0 )
        return ;
    for ( int i = 0; i < g_thread_vector.size(); i ++ ){

        if ( NULL != g_thread_vector[i].th_attr ){//deal with attr is not NULL

            int detachstate = 0;
            int l_ret =  pthread_attr_getdetachstate(g_thread_vector[i].th_attr, &detachstate);
            if ( 0 == l_ret ){

                if ( PTHREAD_CREATE_DETACHED == detachstate ){

                    yLib::yLog::I("Current thread detached.");
                    continue;
                }
            }
            else{

                yLib::yLog::E("pthread_attr_getdetachstate,error num is %d", errno);
            }

        }
        void * ret;
        int l_ret = 0;

        yLib::yLog::I("g_thread_vector size %d, th id is %u", g_thread_vector.size(), g_thread_vector[i].th_id);
        if ( 0 != ( l_ret = pthread_join(g_thread_vector[i].th_id, &ret) )  ){


            if( l_ret == ESRCH ){

                yLib::yLog::E("pthread_join,No thread with the ID thread could be found.");
            }
            else if ( l_ret == EINVAL ){

                yLib::yLog::E("pthread_join,Another thread is already waiting to join with this thread.");
            }
            else if ( l_ret == EDEADLK ){

                yLib::yLog::E("pthread_join,A deadlock was detected (e.g., two threads tried to join with  each  other);  or  thread specifies the calling thread.");
            }
            else if ( l_ret == EINVAL ){

                yLib::yLog::E("pthread_join,thread is not a joinable thread");
            }
            else{

                yLib::yLog::E("pthread_join,unkown error");
            }

        }
        else{
            
            yLib::yLog::I( "Thread %s is exited.", g_thread_vector[i].th_name.c_str() );
        }

    }
}




