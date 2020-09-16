/*
 * @Author: Sky
 * @Date: 2020-08-19 15:10:29
 * @LastEditors: Sky
 * @LastEditTime: 2020-09-16 14:55:45
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"
#include "ytest_element_data.cpp"

TEST_CASE( "Test yQueue" , "[yQueue_Apis]" ){

    yLib::yQueue<int, yLib::yBasicAllocator<int>> my_yqueue_int;
    yLib::yQueue<yTestElementData, yLib::yBasicAllocator<yTestElementData>> my_yqueue_userdefine;

    SECTION("test push(&) and push(&&) /get_size/get_capacity ") {

        int _val = 1;
        my_yqueue_int.push(_val);
        _val = 2;
        my_yqueue_int.push(_val);
        _val = 3;
        my_yqueue_int.push(_val);
        _val = 4;
        my_yqueue_int.push(_val);
        _val = 5;
        my_yqueue_int.push(_val);

        REQUIRE(my_yqueue_int.get_size() == 5);
        
        REQUIRE(my_yqueue_int.get_capacity() == 32);

        yTestElementData _user_val(-1);

        my_yqueue_userdefine.push(_user_val);

        _user_val = 1;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 2;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 3;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_yqueue_userdefine.push(std::move(_user_val));

        REQUIRE(my_yqueue_userdefine.get_size() == 6);
        
        REQUIRE(my_yqueue_userdefine.get_capacity() == 32);

    }
    
    SECTION("test pop()/get_size/get_capacity ") {

        int _val = 1;
        my_yqueue_int.push(_val);
        _val = 2;
        my_yqueue_int.push(_val);
        _val = 3;
        my_yqueue_int.push(_val);
        _val = 4;
        my_yqueue_int.push(_val);
        _val = 5;
        my_yqueue_int.push(_val);

        my_yqueue_int.pop();
        my_yqueue_int.pop();
        my_yqueue_int.pop();

        REQUIRE(my_yqueue_int.get_size() == 2);
        
        REQUIRE(my_yqueue_int.get_capacity() == 32);

        my_yqueue_int.pop();
        my_yqueue_int.pop();

        REQUIRE(my_yqueue_int.get_size() == 0);
        


        yTestElementData _user_val(-1);

        my_yqueue_userdefine.push(_user_val);

        _user_val = 1;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 2;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 3;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_yqueue_userdefine.push(std::move(_user_val));

        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();

        REQUIRE(my_yqueue_userdefine.get_size() == 3);
        
        REQUIRE(my_yqueue_userdefine.get_capacity() == 32);

        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        REQUIRE(my_yqueue_userdefine.get_size() == 0);
    }

    SECTION("test front() ") {

        int _val = 1;
        my_yqueue_int.push(_val);
        _val = 2;
        my_yqueue_int.push(_val);
        _val = 3;
        my_yqueue_int.push(_val);
        _val = 4;
        my_yqueue_int.push(_val);
        _val = 5;
        my_yqueue_int.push(_val);

        _val = 1;
        REQUIRE( my_yqueue_int.front() == _val);

        my_yqueue_int.pop();
        my_yqueue_int.pop();
        my_yqueue_int.pop();

        _val = 4;

        REQUIRE( my_yqueue_int.front() == _val);

        my_yqueue_int.pop();

        _val = 5;
        REQUIRE(my_yqueue_int.front() == _val);
        


        yTestElementData _user_val(-1);

        my_yqueue_userdefine.push(_user_val);

        _user_val = 1;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 2;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 3;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_yqueue_userdefine.push(std::move(_user_val));

        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        
        _user_val = 3;
        REQUIRE(my_yqueue_userdefine.front() == _user_val);

        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        _user_val = 5;
        REQUIRE(my_yqueue_userdefine.front() == _user_val);
    }

    SECTION("test tail() ") {

        int _val = 1;
        my_yqueue_int.push(_val);
        REQUIRE((int)my_yqueue_int.tail() == _val);

        _val = 2;
        my_yqueue_int.push(_val);
        REQUIRE((int)my_yqueue_int.tail() == _val);

        _val = 3;
        my_yqueue_int.push(_val);
        REQUIRE((int)my_yqueue_int.tail() == _val);

        _val = 4;
        my_yqueue_int.push(_val);
        REQUIRE((int)my_yqueue_int.tail() == _val);

        _val = 5;
        my_yqueue_int.push(_val);
        REQUIRE((int)my_yqueue_int.tail() == _val);

        my_yqueue_int.pop();
        my_yqueue_int.pop();
        my_yqueue_int.pop();

        _val = 5;
        REQUIRE((int)my_yqueue_int.tail() == _val);
        

        yTestElementData _user_val(-1);

        my_yqueue_userdefine.push(_user_val);

        _user_val = 1;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 2;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 3;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_yqueue_userdefine.push(std::move(_user_val));

        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();

        _user_val = 5;
        REQUIRE((yTestElementData)my_yqueue_userdefine.tail() == _user_val);

    }

    SECTION("test resize_capacity() ") {

        int _val = 1;
        my_yqueue_int.push(_val);
        _val = 2;
        my_yqueue_int.push(_val);
        _val = 3;
        my_yqueue_int.push(_val);
        _val = 4;
        my_yqueue_int.push(_val);
        _val = 5;
        my_yqueue_int.push(_val);
        my_yqueue_int.resize_capacity(64);

        REQUIRE(my_yqueue_int.get_size() == 5);
        REQUIRE(my_yqueue_int.get_capacity() == 64);

        my_yqueue_int.pop();
        my_yqueue_int.pop();
        my_yqueue_int.pop();
        my_yqueue_int.pop();
        my_yqueue_int.pop();
        REQUIRE(my_yqueue_int.get_size() == 0);
        REQUIRE(my_yqueue_int.get_capacity() == 64);

        yTestElementData _user_val(-1);

        my_yqueue_userdefine.push(_user_val);

        _user_val = 1;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 2;
        my_yqueue_userdefine.push(_user_val);

        _user_val = 3;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 4;
        my_yqueue_userdefine.push(std::move(_user_val));

        _user_val = 5;
        my_yqueue_userdefine.push(std::move(_user_val));

        my_yqueue_userdefine.resize_capacity(64);

        REQUIRE(my_yqueue_userdefine.get_size() == 6);
        REQUIRE(my_yqueue_userdefine.get_capacity() == 64);

        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        my_yqueue_userdefine.pop();
        
        REQUIRE(my_yqueue_userdefine.get_size() == 0);
        REQUIRE(my_yqueue_userdefine.get_capacity() == 64);
    }
}