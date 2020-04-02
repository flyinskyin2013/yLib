/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:53
 * @LastEditors: Sky
 * @LastEditTime: 2018-10-23 11:09:53
 * @Description: 
 */

#include "ylib.hpp"



int main (int argc, char * argv[]){

    yLib::yCurl test;
    std::string in="";
    std::string pa="";
    std::string ip="www.baidu.com";
    test.yCurl_Set_Param(ip);
    test.yCurl_Get(in,pa);
    return 0;
}