/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 15:59:28
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 10:07:11
 * @FilePath: \yLib\tests\ysharedmemory\ysharedmemory_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"

#include "test_common.hpp"

#include <iostream>


DEFINE_TEST_CASE_FOR_CLASS_INFO(ySharedMemory)

TEST_CASE( "Test ySharedMemory_W apis" , "[ySharedMemory_W_Apis]" ){

    SECTION("test other apis ") {
        
        yLib::ySharedMemory::__yshmparam__ _shm_param_for_test;

        yLib::ySharedMemory::yShmParam _shm_param;

        #ifdef _WIN32
        _shm_param.shm_name = CONVERT_STR_TO_YLIB_STD_STRING(TestSHM);
        #elif __linux__ || __linux
        _shm_param.shm_key = -1;
        _shm_param.shm_flag = ~IPC_CREAT;
        #endif 

        
        // ENOENT No segment exists for the given key, and IPC_CREAT was not specified.
        yLib::ySharedMemory _shm(30, _shm_param);


        #ifdef _WIN32
        _shm_param.shm_name = CONVERT_STR_TO_YLIB_STD_STRING(TestSHM);
        #elif __linux__ || __linux
        _shm_param.shm_flag = 0;
        _shm_param.shm_key = 0x654321;
        #endif 

        yLib::ySharedMemory _shm1(30, _shm_param);

        // EINVAL A segment for the given key exists, but size is greater than the size of that segment.
        yLib::ySharedMemory _shm2(40, _shm_param);

        // delete shm
        yLib::ySharedMemory _shm3(30, _shm_param, true);
    }

    SECTION("test write ") {

        yLib::ySharedMemory::yShmParam _shm_param;

        _shm_param.shm_key = 0x123456;
        
        yLib::ySharedMemory _shm(30, _shm_param);

        const char * const _test_str = "Hello ySharedMemory";
        ::memcpy(_shm.GetShmDataPtr(), _test_str, sizeof("Hello ySharedMemory"));
    }
}