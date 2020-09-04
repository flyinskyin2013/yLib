/*
 * @Author: Sky
 * @Date: 2020-08-19 15:10:20
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-04 17:37:52
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "ytest_element_data.cpp"

// SCENARIO(""){


//     GIVEN(""){


//         WHEN(""){

            
//         }
//     }
// }

TEST_CASE( "Test yStack" , "[yStack_Apis]" ){

    yLib::yStack<int, yLib::yBasicAllocator<int>> my_ystack_int;
    yLib::yStack<yTestElementData, yLib::yBasicAllocator<yTestElementData>> my_ystack_userdefine;

    SECTION("test push(&) and push(&&) /get_size/get_capacity ") {

        int _val = 1;
        my_ystack_int.push(_val);
        _val = 2;
        my_ystack_int.push(_val);
        _val = 3;
        my_ystack_int.push(_val);
        _val = 4;
        my_ystack_int.push(_val);
        _val = 5;
        my_ystack_int.push(_val);

        REQUIRE(my_ystack_int.get_size() == 5);
        
        REQUIRE(my_ystack_int.get_capacity() == 32);

        yTestElementData _user_val(-1);

        my_ystack_userdefine.push(_user_val);

        _user_val = 1;
        my_ystack_userdefine.push(_user_val);

        _user_val = 2;
        my_ystack_userdefine.push(_user_val);

        _user_val = 3;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_ystack_userdefine.push(std::move(_user_val));

        REQUIRE(my_ystack_userdefine.get_size() == 6);
        
        REQUIRE(my_ystack_userdefine.get_capacity() == 32);

    }
    
    SECTION("test pop()/get_size/get_capacity ") {

        int _val = 1;
        my_ystack_int.push(_val);
        _val = 2;
        my_ystack_int.push(_val);
        _val = 3;
        my_ystack_int.push(_val);
        _val = 4;
        my_ystack_int.push(_val);
        _val = 5;
        my_ystack_int.push(_val);

        my_ystack_int.pop();
        my_ystack_int.pop();
        my_ystack_int.pop();

        REQUIRE(my_ystack_int.get_size() == 2);
        
        REQUIRE(my_ystack_int.get_capacity() == 32);

        my_ystack_int.pop();
        my_ystack_int.pop();

        REQUIRE(my_ystack_int.get_size() == 0);
        


        yTestElementData _user_val(-1);

        my_ystack_userdefine.push(_user_val);

        _user_val = 1;
        my_ystack_userdefine.push(_user_val);

        _user_val = 2;
        my_ystack_userdefine.push(_user_val);

        _user_val = 3;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_ystack_userdefine.push(std::move(_user_val));

        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();

        REQUIRE(my_ystack_userdefine.get_size() == 3);
        
        REQUIRE(my_ystack_userdefine.get_capacity() == 32);

        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        REQUIRE(my_ystack_userdefine.get_size() == 0);
    }

    SECTION("test front() ") {

        int _val = 1;
        my_ystack_int.push(_val);
        _val = 2;
        my_ystack_int.push(_val);
        _val = 3;
        my_ystack_int.push(_val);
        _val = 4;
        my_ystack_int.push(_val);
        _val = 5;
        my_ystack_int.push(_val);

        my_ystack_int.pop();
        my_ystack_int.pop();
        my_ystack_int.pop();

        _val = 2;
        int _tmp_ret = my_ystack_int.front();
        REQUIRE( _tmp_ret == _val);

        my_ystack_int.pop();

        _val = 1;
        _tmp_ret = my_ystack_int.front();
        REQUIRE(my_ystack_int.front() == _val);
        


        yTestElementData _user_val(-1);

        my_ystack_userdefine.push(_user_val);

        _user_val = 1;
        my_ystack_userdefine.push(_user_val);

        _user_val = 2;
        my_ystack_userdefine.push(_user_val);

        _user_val = 3;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_ystack_userdefine.push(std::move(_user_val));

        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        
        _user_val = 2;
        REQUIRE(my_ystack_userdefine.front() == _user_val);

        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        _user_val = -1;
        REQUIRE(my_ystack_userdefine.front() == _user_val);
    }

    SECTION("test tail() ") {

        int _val = 1;
        my_ystack_int.push(_val);
        _val = 2;
        my_ystack_int.push(_val);
        _val = 3;
        my_ystack_int.push(_val);
        _val = 4;
        my_ystack_int.push(_val);
        _val = 5;
        my_ystack_int.push(_val);

        my_ystack_int.pop();
        my_ystack_int.pop();
        my_ystack_int.pop();

        _val = 1;
        REQUIRE((int)my_ystack_int.tail() == _val);
        

        yTestElementData _user_val(-1);

        my_ystack_userdefine.push(_user_val);

        _user_val = 1;
        my_ystack_userdefine.push(_user_val);

        _user_val = 2;
        my_ystack_userdefine.push(_user_val);

        _user_val = 3;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_ystack_userdefine.push(std::move(_user_val));

        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();

        _user_val = -1;
        REQUIRE((yTestElementData)my_ystack_userdefine.tail() == _user_val);

    }

    SECTION("test resize_capacity() ") {

        int _val = 1;
        my_ystack_int.push(_val);
        _val = 2;
        my_ystack_int.push(_val);
        _val = 3;
        my_ystack_int.push(_val);
        _val = 4;
        my_ystack_int.push(_val);
        _val = 5;
        my_ystack_int.push(_val);
        my_ystack_int.resize_capacity(64);

        REQUIRE(my_ystack_int.get_size() == 5);
        REQUIRE(my_ystack_int.get_capacity() == 64);

        my_ystack_int.pop();
        my_ystack_int.pop();
        my_ystack_int.pop();
        my_ystack_int.pop();
        my_ystack_int.pop();
        REQUIRE(my_ystack_int.get_size() == 0);
        REQUIRE(my_ystack_int.get_capacity() == 64);

        yTestElementData _user_val(-1);

        my_ystack_userdefine.push(_user_val);

        _user_val = 1;
        my_ystack_userdefine.push(_user_val);

        _user_val = 2;
        my_ystack_userdefine.push(_user_val);

        _user_val = 3;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_ystack_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_ystack_userdefine.push(std::move(_user_val));

        my_ystack_userdefine.resize_capacity(64);

        REQUIRE(my_ystack_userdefine.get_size() == 6);
        REQUIRE(my_ystack_userdefine.get_capacity() == 64);

        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        my_ystack_userdefine.pop();
        
        REQUIRE(my_ystack_userdefine.get_size() == 0);
        REQUIRE(my_ystack_userdefine.get_capacity() == 64);
    }
}

