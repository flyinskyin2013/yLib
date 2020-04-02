/*
 * @Author: Sky
 * @Date: 2019-09-25 15:46:23
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-25 16:53:24
 * @Description: 
 */

#include "ylib.hpp"

#include <iostream>
#include <vector>
#include <list>

class tmp_data{

    public:
    tmp_data(){std::cout<<"constructor"<<std::endl;}
    ~tmp_data(){std::cout<<"destructor"<<std::endl;}
    tmp_data(const tmp_data & data){std::cout<<"copy-constructor"<<std::endl;}
    tmp_data & operator=(const tmp_data & data){std::cout<<"assignment-copy-constructor"<<std::endl;}
    tmp_data(tmp_data && data){std::cout<<"move-constructor"<<std::endl;}
    tmp_data & operator=(tmp_data && data){std::cout<<"assignment-move-constructor"<<std::endl;}
};

int main(int argc, char *argv[])
{

    // tmp_data a; //constructor
    // tmp_data b = a; //copy-constructor
    // tmp_data c; //constructor
    // c = b; //std::cout<<"assignment-copy-constructor"<<std::endl;
    // tmp_data d; //constructor
    // tmp_data e(std::move(d)); //move-constructor
    // tmp_data f; //constructor
    // f = std::move(e); //assignment-move-constructor

    //std::vector<tmp_data> vec_lst;
    std::list<tmp_data> vec_lst;

    tmp_data no_opti; ////constructor

    //vec_lst.reserve(3); //for optimization

    vec_lst.push_back(no_opti);//copy-constructor

    vec_lst.push_back(tmp_data()); //constructor, move-constructor
    //destructor
    vec_lst.push_back(std::move(tmp_data()));
    //destructor
    
    //destructor * 3 : vec(3), no_opti

    return 0;
}