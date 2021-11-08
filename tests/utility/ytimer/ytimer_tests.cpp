/*
 * @Author: Sky
 * @Date: 2021-04-07 17:45:00
 * @LastEditors: Sky
 * @LastEditTime: 2021-11-10 09:44:18
 * @Description: 
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <iostream>

#include "test_common.hpp"
DEFINE_TEST_CASE_FOR_CLASS_INFO(yTimer)


static void test_ytimer_callback(void * param)
{
    std::cout<<"test_ytimer_callback(): param = "<<*((int*)param)<<std::endl;
}


TEST_CASE( "Test yTimer apis" , "[yTimer_Apis]" ){

    yLib::yTimer timer;


    SECTION("test GetTime() ") {

        timer.GetTime();

#if defined(_WIN32) || defined(_WIN64)

        ::Sleep(2000);

#elif defined(__linux__) || defined(__linux)
    
        sleep(2);
        
#elif defined(__unix__) || defined(__unix)

        
#endif //__unix__ || __unix        
        
        timer.GetTime(1);

        REQUIRE(timer.GetCalculatedTime() > 2000.f);
    }

    SECTION("test yTimer callback") {

        timer.SetTimerCB(test_ytimer_callback);

        int _test_param = 5;
        timer.StartTimer(500, &_test_param);

#if defined(_WIN32) || defined(_WIN64)

        ::Sleep(5000);

#elif defined(__linux__) || defined(__linux)
    
        sleep(5);
        
#elif defined(__unix__) || defined(__unix)

        
#endif //__unix__ || __unix     

        do
        {
            timer.StopTimer();
#if defined(_WIN32) || defined(_WIN64)

        ::Sleep(1);

#elif defined(__linux__) || defined(__linux)
    
        usleep(1000);
        
#elif defined(__unix__) || defined(__unix)

        
#endif //__unix__ || __unix     
            
        } while (timer.TimerRunFlag());//last thread stop        

    }

}