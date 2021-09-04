/*
 * @Author: Sky
 * @Date: 2021-08-27 10:29:04
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-04 20:56:12
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "test_common.hpp"


#include <memory>

#ifdef _WIN32

// msvc compiler
DEFINE_TEST_CASE_FOR_CLASS_INFO(yBasicAllocator<char>)

#elif __linux__ || __linux

// gnu compiler
DEFINE_TEST_CASE_FOR_CLASS_INFO_DIFF(yBasicAllocator<char>, yBasicAllocator<c>)

#endif 


TEST_CASE( "Test yBasicAllocator apis" , "[yBasicAllocator_Apis]" ){

    SECTION("test some apis ") {
        
        yLib::yBasicAllocator<yLib::yObject> _alloctor;

        std::unique_ptr<char[]> _mem_pool = std::unique_ptr<char[]>(new (std::nothrow) char [1024]);
        if (nullptr == _mem_pool)
            return ;
        
        yLib::yObject * _obj = reinterpret_cast<yLib::yObject *>(_mem_pool.get());
        _alloctor.construct(_obj, yLib::yObject());

        REQUIRE_THAT( _obj->yLibGetClassInfo().class_name, Catch::Equals("yObject"));

        _alloctor.destroy(_obj);

        // REQUIRE_THAT( _alloctor.GetClassInfo().class_name, Catch::Equals("yBasicAllocator<N4yLib7yObjectE>"));
        
    }
}