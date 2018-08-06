#include "ylib.h"



int main (int argc, char * argv[]){

    yLib::yLog::I("class call");
    yLib::yLog::D("class call A %d, %f test ylog", 1234, 66.5555);
    yLib::yLog::W("class call A %d, %f test ylog", 1234, 66.5555);
    yLib::yLog::E("class call A %d, %f test ylog", 1234, 66.5555);

    yLib::yLog test;
    test.I("object call");
    test.D("object call A %d, %f test ylog", 123, 54.5555);
    test.W("object call A %d, %f test ylog", 123, 54.5555);


    yLib::yLog::I("class call");
    yLib::yLog::D("class call A %d, %f test ylog", 1234, 66.5555);
    yLib::yLog::W("class call A %d, %f test ylog", 1234, 66.5555);
    yLib::yLog::E("class call A %d, %f test ylog", 1234, 66.5555);


    test.I("object call");
    test.E("object call A %d, %f test ylog", 123, 54.5555);


    return 0;
}