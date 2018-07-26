#include "ycurl.h"



int main (int argc, char * argv[]){

    yLib::yCurl test;
    std::string in="";
    std::string pa="";
    std::string ip="www.baidu.com";
    test.yCurl_Set_Param(ip);
    test.yCurl_Get(in,pa);
    return 0;
}