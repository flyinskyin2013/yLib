/*
 * @Author: Sky
 * @Date: 2021-04-07 17:45:00
 * @LastEditors: Sky
 * @LastEditTime: 2021-04-08 09:55:13
 * @Description: 
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"

#include <iostream>

static void test_ytimer_callback(void * param)
{
    std::cout<<"test_ytimer_callback(): param = "<<*((int*)param)<<std::endl;
}


TEST_CASE( "Test yTimer apis" , "[yTimer_Apis]" ){

    yLib::yTimer timer;


    SECTION("test GetTime() ") {

        timer.GetTime();
        sleep(2);
        timer.GetTime(1);

        REQUIRE(timer.GetCalculatedTime() > 2000.f);
    }

    SECTION("test yTimer callback") {

        timer.SetTimerCB(test_ytimer_callback);

        int _test_param = 5;
        timer.StartTimer(500, &_test_param);

        sleep(5);

        do
        {
            timer.StopTimer();
            usleep(500);
            
        } while (timer.TimerRunFlag());//last thread stop        

    }

}