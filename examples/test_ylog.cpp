#include "ylog.h"



int main (int argc, char * argv[]){

    yLib::yLog test;
    test.I("A %d, %f test ylog", 123, 54.5555);
    test.D("A %d, %f test ylog", 123, 54.5555);
    test.W("A %d, %f test ylog", 123, 54.5555);
    test.E("A %d, %f test ylog", 123, 54.5555);
    return 0;
}