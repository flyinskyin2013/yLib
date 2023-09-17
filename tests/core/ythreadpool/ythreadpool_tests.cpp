/*
Copyright (c) 2018 - 2023 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * @Author: Sky
 * @Date: 2023-04-08 16:47:44
 * @LastEditors: Sky
 * @LastEditTime: 2023-04-08 18:23:01
 * @Description: 
 */


#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "test_common.hpp"

#include <cstdio>

struct RET_STRUCT{
    int a;
    std::string s;

    RET_STRUCT(int a, const std::string & s):a(a), s(s){}
};


// DEFINE_TEST_CASE_FOR_CLASS_INFO(yThreadPool)

TEST_CASE( "Test yThreadPool apis" , "[yThreadPool_Apis]" ){

    SECTION("test some apis ") {
#ifdef _WIN32


#elif __linux__ || __linux
    
        yLib::yThreadPool _th_pool(5);

        //func 0
        auto _func0 = [](){

            usleep(1000 * 100);
            printf("call _func0\n");
        };
        //func 1
        auto _func1 = [](){

            usleep(1000 * 300);
            printf("call _func1\n");
            return 123;
        };
        //func 2
        auto _func2 = [](){

            usleep(1000 * 600);
            printf("call _func2\n");
            return std::string("123");
        };

        //func 3
        auto _func3 = [](){

            usleep(1000 * 900);
            printf("call _func3\n");
            return RET_STRUCT(123, "123");
        };

        //push task, test out of order
        for(int _i = 0; _i < 10; _i++){
            _th_pool.add_task(_func0);
            _th_pool.add_task(_func1);
            _th_pool.add_task(_func2);
            _th_pool.add_task(_func3);
        }

        //push task, test result
        auto _func0_ret = _th_pool.add_task(_func0);
        auto _func1_ret = _th_pool.add_task(_func1);
        auto _func2_ret = _th_pool.add_task(_func2);
        auto _func3_ret = _th_pool.add_task(_func3);

        _func0_ret.get();

        REQUIRE(_func1_ret.get() == 123);

        REQUIRE_THAT(_func2_ret.get(), Catch::Equals ( "123" ));

        RET_STRUCT _ret = _func3_ret.get();
        REQUIRE(_ret.a == 123);
        REQUIRE_THAT(_ret.s.c_str(), Catch::Equals ( "123" ));

        // REQUIRE_THAT(_com.ConvertErrnoToStr(EINVAL), Catch::Equals ("Invalid argument"));

        
        printf("thread pool exit done\n");

#endif 
    }
}