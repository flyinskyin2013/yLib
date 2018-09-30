

#include "ylib.h"


int main (int argc, char * argv[]){

    yLib::yConfig config;

    config.yConfigReadFile("test.conf");

    std::cout << "read int value :"<< config.yConfigGetIntValue("test.int") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetBoolValue("test.bool") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetFloatValue("test.float") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetStringValue("test.string") << std::endl;

    return 0;
}