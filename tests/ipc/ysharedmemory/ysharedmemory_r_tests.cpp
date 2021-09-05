/*
 * @Author: Sky
 * @Date: 2021-08-30 10:30:51
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 10:05:56
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"

#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(ySharedMemory)

TEST_CASE( "Test ySharedMemory_R apis" , "[ySharedMemory_R_Apis]" ){

    SECTION("test read ") {

        yLib::ySharedMemory::yShmParam _shm_param;

        #ifdef _WIN32
        _shm_param.shm_name = CONVERT_STR_TO_YLIB_STD_STRING(TestSHM);
        #elif __linux__ || __linux
        _shm_param.shm_key = 0x123456;
        #endif 
        yLib::ySharedMemory _shm(30, _shm_param, true);

        const char * const _test_str = "Hello ySharedMemory";

        char _tmp_buf[100];
        ::memset(_tmp_buf, 0, 100);

        ::memcpy(_tmp_buf, _shm.GetShmDataPtr(), sizeof("Hello ySharedMemory"));

        REQUIRE_THAT( _tmp_buf, Catch::Equals ( _test_str ));
    }
}