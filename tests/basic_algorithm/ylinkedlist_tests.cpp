/*
 * @Author: Sky
 * @Date: 2020-07-02 15:09:31
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-17 11:30:25
 * @Description: 
 */ 


#include "catch2/catch.hpp"
#include "ylib.hpp"
#include <vector>


#include "ytest_element_data.cpp"


TEST_CASE( "Test yLinkedList" , "[yLinkedList_Apis]" ){

    SECTION("test insert*") {

        yLib::yLinkedList<int, yLib::yBasicAllocator<yLib::yLinkedListNode<int>>> lk_list;

        //insert
        int _val = 1;
        lk_list.insert_head(_val);
        _val = 2;
        lk_list.insert_head(_val);
        _val = 3;
        lk_list.insert_head(_val);
        _val = 4;
        lk_list.insert_tail(_val);
        _val = 5;
        lk_list.insert_tail(_val);


        REQUIRE(3 == lk_list.get_element(0));
        REQUIRE(2 == lk_list.get_element(1));
        REQUIRE(1 == lk_list.get_element(2));
        REQUIRE(4 == lk_list.get_element(3));
        REQUIRE(5 == lk_list.get_element(4));


        REQUIRE(lk_list.get_node_count() == 5);
        
    }



    SECTION("test remove*") {

        yLib::yLinkedList<int, yLib::yBasicAllocator<yLib::yLinkedListNode<int>>> lk_list;

        //insert
        int _val = 1;
        lk_list.insert_head(_val);
        _val = 2;
        lk_list.insert_head(_val);
        _val = 3;
        lk_list.insert_head(_val);
        _val = 0;
        lk_list.insert_head(_val);
        _val = -1;
        lk_list.insert_head(_val);
        _val = 4;
        lk_list.insert_tail(_val);
        _val = 5;
        lk_list.insert_tail(_val);
        _val = 6;
        lk_list.insert_tail(_val);
        _val = 7;
        lk_list.insert_tail(_val);


        REQUIRE(-1 == lk_list.get_element(0));
        REQUIRE(0 == lk_list.get_element(1));
        REQUIRE(3 == lk_list.get_element(2));
        REQUIRE(2 == lk_list.get_element(3));
        REQUIRE(1 == lk_list.get_element(4));
        REQUIRE(4 == lk_list.get_element(5));
        REQUIRE(5 == lk_list.get_element(6));
        REQUIRE(6 == lk_list.get_element(7));
        REQUIRE(7 == lk_list.get_element(8));
        REQUIRE(lk_list.get_node_count() == 9);

        lk_list.remove_head();
        lk_list.remove_tail();
        _val = 5;
        lk_list.remove(_val);
        _val = 2;
        lk_list.remove(_val);

        //0 3 1 4 6
        REQUIRE(0 == lk_list.get_element(0));
        REQUIRE(3 == lk_list.get_element(1));
        REQUIRE(1 == lk_list.get_element(2));
        REQUIRE(4 == lk_list.get_element(3));
        REQUIRE(6 == lk_list.get_element(4));


        REQUIRE(lk_list.get_node_count() == 5);
        
    }
    
    SECTION("test class-data-type") {

        yLib::yLinkedList<yTestElementData, yLib::yBasicAllocator<yLib::yLinkedListNode<yTestElementData>>> lk_list;

        //insert
        yTestElementData _val;
        _val = yTestElementData(1);
        lk_list.insert_head(_val);
        _val = yTestElementData(2);
        lk_list.insert_head(_val);
        _val = yTestElementData(3);
        lk_list.insert_head(_val);
        _val = yTestElementData(0);
        lk_list.insert_head(_val);
        _val = yTestElementData(-1);
        lk_list.insert_head(_val);
        _val = yTestElementData(4);
        lk_list.insert_tail(_val);
        _val = yTestElementData(5);
        lk_list.insert_tail(_val);
        _val = yTestElementData(6);
        lk_list.insert_tail(_val);
        _val = yTestElementData(7);
        lk_list.insert_tail(_val);


        REQUIRE(yTestElementData(-1) == lk_list.get_element(0));
        REQUIRE(yTestElementData(0)  == lk_list.get_element(1));
        REQUIRE(yTestElementData(3)  == lk_list.get_element(2));
        REQUIRE(yTestElementData(2)  == lk_list.get_element(3));
        REQUIRE(yTestElementData(1)  == lk_list.get_element(4));
        REQUIRE(yTestElementData(4)  == lk_list.get_element(5));
        REQUIRE(yTestElementData(5)  == lk_list.get_element(6));
        REQUIRE(yTestElementData(6)  == lk_list.get_element(7));
        REQUIRE(yTestElementData(7)  == lk_list.get_element(8));
        REQUIRE(lk_list.get_node_count() == 9);

        lk_list.reverse();

        REQUIRE(yTestElementData(7) == lk_list.get_element(0));
        REQUIRE(yTestElementData(6) == lk_list.get_element(1));
        REQUIRE(yTestElementData(5) == lk_list.get_element(2));
        REQUIRE(yTestElementData(4) == lk_list.get_element(3));
        REQUIRE(yTestElementData(1) == lk_list.get_element(4));
        REQUIRE(yTestElementData(2) == lk_list.get_element(5));
        REQUIRE(yTestElementData(3) == lk_list.get_element(6));
        REQUIRE(yTestElementData(0) == lk_list.get_element(7));
        REQUIRE(yTestElementData(-1) == lk_list.get_element(8));
        REQUIRE(lk_list.get_node_count() == 9);        
        
    }



        SECTION("test remove*") {

        yLib::yLinkedList<int, yLib::yBasicAllocator<yLib::yLinkedListNode<int>>> lk_list;

        //insert
        int _val = 1;
        lk_list.insert_head(_val);
        _val = 2;
        lk_list.insert_head(_val);
        _val = 3;
        lk_list.insert_head(_val);
        _val = 0;
        lk_list.insert_head(_val);
        _val = -1;
        lk_list.insert_head(_val);
        _val = 4;
        lk_list.insert_tail(_val);
        _val = 5;
        lk_list.insert_tail(_val);
        _val = 6;
        lk_list.insert_tail(_val);
        _val = 7;
        lk_list.insert_tail(_val);


        REQUIRE(-1 == lk_list.get_element(0));
        REQUIRE(0 == lk_list.get_element(1));
        REQUIRE(3 == lk_list.get_element(2));
        REQUIRE(2 == lk_list.get_element(3));
        REQUIRE(1 == lk_list.get_element(4));
        REQUIRE(4 == lk_list.get_element(5));
        REQUIRE(5 == lk_list.get_element(6));
        REQUIRE(6 == lk_list.get_element(7));
        REQUIRE(7 == lk_list.get_element(8));
        REQUIRE(lk_list.get_node_count() == 9);

        lk_list.reverse();

        REQUIRE(7 == lk_list.get_element(0));
        REQUIRE(6 == lk_list.get_element(1));
        REQUIRE(5 == lk_list.get_element(2));
        REQUIRE(4 == lk_list.get_element(3));
        REQUIRE(1 == lk_list.get_element(4));
        REQUIRE(2 == lk_list.get_element(5));
        REQUIRE(3 == lk_list.get_element(6));
        REQUIRE(0 == lk_list.get_element(7));
        REQUIRE(-1 == lk_list.get_element(8));
        REQUIRE(lk_list.get_node_count() == 9);        
        
    }
}