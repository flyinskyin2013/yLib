/*
 * @Author: Sky
 * @Date: 2020-08-19 15:10:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-08-19 15:16:56
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "ytest_element_data.cpp"

TEST_CASE( "Test yQueue" , "[yQueue_Apis]" ){

    SECTION("test push(&) and push(&&) ") {

        // yLib::yLinearList<int, yLib::yBasicAllocator<int>> sq_list0(1);
        // int _val = 1;
        // sq_list0.sq_insert(0, _val);
        // _val = 2;
        // sq_list0.sq_insert(1, _val);
        // _val = 3;
        // sq_list0.sq_insert(2, _val);
        // _val = 4;
        // sq_list0.sq_insert(3, _val);
        // _val = 5;
        // sq_list0.sq_insert(4, _val);

        // for (int i = 0; i < 5; i++)//compare val
        //     REQUIRE(i+1 == sq_list0.sq_get_value(i));

        // REQUIRE(sq_list0.sq_get_size() == 5);
        
        // REQUIRE(sq_list0.sq_get_capacity() == 32);
    }
    
    SECTION("test pop() ") {


    }

    SECTION("test front() ") {


    }

    SECTION("test tail() ") {


    }

    SECTION("test get_size() ") {


    }

    SECTION("test get_capacity() ") {


    }

    SECTION("test resize_capacity() ") {


    }
}