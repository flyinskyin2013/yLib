/*
 * @Author: Sky
 * @Date: 2020-05-28 14:24:31
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-22 18:39:55
 * @Description: 
 */

#include "catch2/catch.hpp"
#include "ylib.hpp"
#include <vector>


#include "ytest_element_data.cpp"

TEST_CASE( "Test yLinearList" , "[yLinearList_Apis]" ){

    SECTION("test sq_insert ") {

        yLib::yLinearList<int, yLib::yBasicAllocator<int>> sq_list0(1);
        int _val = 1;
        sq_list0.sq_insert(0, _val);
        _val = 2;
        sq_list0.sq_insert(1, _val);
        _val = 3;
        sq_list0.sq_insert(2, _val);
        _val = 4;
        sq_list0.sq_insert(3, _val);
        _val = 5;
        sq_list0.sq_insert(4, _val);

        for (int i = 0; i < 5; i++)//compare val
            REQUIRE(i+1 == sq_list0.sq_get_value(i));

        REQUIRE(sq_list0.sq_get_size() == 5);
        
        REQUIRE(sq_list0.sq_get_capacity() == 32);
    }
    
    SECTION("test sq_delete ") {

        yLib::yLinearList<int, yLib::yBasicAllocator<int>> sq_list0(1);
        std::vector<int> test_set;
        for (int _i = 0; _i < 1000;_i ++){
         
            sq_list0.sq_insert(_i, _i);

            if (_i == 0 || _i == 123 || _i == 345|| _i == 567 || _i == 789 || _i == 999)         
                continue;

            test_set.push_back(_i);
        }

        int _val = 0;
        sq_list0.sq_delete(_val);

        
        _val = 123;
        sq_list0.sq_delete(_val);


        _val = 345;
        sq_list0.sq_delete(_val);

        _val = 567;
        sq_list0.sq_delete(_val);

        _val = 789;
        sq_list0.sq_delete(_val);

        _val = 999;
        sq_list0.sq_delete(_val);

        for (int i = 0; i < 993; i++)//compare val
            REQUIRE(test_set[i] == sq_list0.sq_get_value(i) );
            

        _val = 100000;
        REQUIRE(-1 == sq_list0.sq_delete(_val) );    

        REQUIRE(sq_list0.sq_get_size() == 994);
        
        REQUIRE(sq_list0.sq_get_capacity() == 4096);
    }


    SECTION("test sq_merge ") {

        yLib::yLinearList<int, yLib::yBasicAllocator<int>> sq_list1(1);
        yLib::yLinearList<int, yLib::yBasicAllocator<int>> sq_list2(1);
        yLib::yLinearList<int, yLib::yBasicAllocator<int>> sq_list3(1);

        for (int _i = 0; _i < 1000; _i++)//1k 0-999
            sq_list1.sq_insert(_i, _i);

        int _val = 0;
        for (int _i = 0; _i < 700; _i++){//700 500-1199
            _val = 500 + _i;
            sq_list2.sq_insert(_i, _val);
        }

        sq_list3.sq_merge(sq_list1, sq_list2, yLib::yLinearList<int, yLib::yBasicAllocator<int>>::TYPE_ASC_ORDER);

        for (int _i = 0; _i < 1700; _i ++){

            if (_i < 500){//0-499

                REQUIRE(_i == sq_list3.sq_get_value(_i));
            }
            if (_i >= 500 && _i < 1499){//500-999

                REQUIRE((500 + (_i - 500)/2) == sq_list3.sq_get_value(_i));
                REQUIRE((500 + (_i - 500)/2) == sq_list3.sq_get_value(_i + 1));
                
                _i ++;//same val
            }
            if (_i >= 1500){//1000-1199

                REQUIRE((1000 + (_i - 1500)) == sq_list3.sq_get_value(_i ));
            }
        }
        REQUIRE(sq_list3.sq_get_size() == 1700);
        REQUIRE(sq_list3.sq_get_capacity() == 8192);
    }







    //for class-data-type
    SECTION("test sq_insert class-data-type") {

        yLib::yLinearList<yTestElementData, yLib::yBasicAllocator<yTestElementData>> sq_list0(1);
        
        yTestElementData _val;
        _val = yTestElementData(1);
        sq_list0.sq_insert(0, _val);
        _val = yTestElementData(2);
        sq_list0.sq_insert(1, _val);
        _val = yTestElementData(3);
        sq_list0.sq_insert(2, _val);
        _val = yTestElementData(4);
        sq_list0.sq_insert(3, _val);
        _val = yTestElementData(5);
        sq_list0.sq_insert(4, _val);

        for (int i = 0; i < 5; i++)//compare val
            REQUIRE(i+1 == sq_list0.sq_get_value(i));

        REQUIRE(sq_list0.sq_get_size() == 5);
        //5*8 = 40  
        REQUIRE(sq_list0.sq_get_capacity() == 64);
    }
    
    SECTION("test sq_delete class-data-type") {

        yLib::yLinearList<yTestElementData, yLib::yBasicAllocator<yTestElementData>> sq_list0(1);
        std::vector<yTestElementData> test_set;
        for (int _i = 0; _i < 1000;_i ++){
            
            yTestElementData _tmp_val(_i);
            sq_list0.sq_insert(_i, _tmp_val);

            if (_i == 0 || _i == 123 || _i == 345|| _i == 567 || _i == 789 || _i == 999)         
                continue;

            test_set.push_back(_tmp_val);
        }

        yTestElementData _val;
        _val = yTestElementData(0);
        sq_list0.sq_delete(_val);

        
        _val = yTestElementData(123);
        sq_list0.sq_delete(_val);


        _val = yTestElementData(345);
        sq_list0.sq_delete(_val);

        _val = yTestElementData(567);
        sq_list0.sq_delete(_val);

        _val = yTestElementData(789);
        sq_list0.sq_delete(_val);

        _val = yTestElementData(999);
        sq_list0.sq_delete(_val);

        for (int i = 0; i < 993; i++)//compare val
            REQUIRE(test_set[i] == sq_list0.sq_get_value(i) );
            

        _val = yTestElementData(100000);
        REQUIRE(-1 == sq_list0.sq_delete(_val) );    

        REQUIRE(sq_list0.sq_get_size() == 994);
        //994*8
        REQUIRE(sq_list0.sq_get_capacity() == 8192);
    }


    SECTION("test sq_merge class-data-type") {

        yLib::yLinearList<yTestElementData, yLib::yBasicAllocator<yTestElementData>> sq_list1(1);
        yLib::yLinearList<yTestElementData, yLib::yBasicAllocator<yTestElementData>> sq_list2(1);
        yLib::yLinearList<yTestElementData, yLib::yBasicAllocator<yTestElementData>> sq_list3(1);

        for (int _i = 0; _i < 1000; _i++){//1k 0-999

            yTestElementData _tmp_val(_i);
            sq_list1.sq_insert(_i, _tmp_val);
        }

        int _val = 0;
        for (int _i = 0; _i < 700; _i++){//700 500-1199
            _val = 500 + _i;
            yTestElementData _tmp_val(_val);
            sq_list2.sq_insert(_i, _tmp_val);
        }

        sq_list3.sq_merge(sq_list1, sq_list2, yLib::yLinearList<yTestElementData, yLib::yBasicAllocator<yTestElementData>>::TYPE_ASC_ORDER);

        for (int _i = 0; _i < 1700; _i ++){

            if (_i < 500){//0-499

                REQUIRE(_i == sq_list3.sq_get_value(_i));
            }
            if (_i >= 500 && _i < 1499){//500-999

                REQUIRE((500 + (_i - 500)/2) == sq_list3.sq_get_value(_i));
                REQUIRE((500 + (_i - 500)/2) == sq_list3.sq_get_value(_i + 1));
                
                _i ++;//same val
            }
            if (_i >= 1500){//1000-1199

                REQUIRE((1000 + (_i - 1500)) == sq_list3.sq_get_value(_i ));
            }
        }
        REQUIRE(sq_list3.sq_get_size() == 1700);
        //1700*8  <  2^14
        REQUIRE(sq_list3.sq_get_capacity() == 16384);
    }

}