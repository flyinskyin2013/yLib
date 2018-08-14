#include "ylib.h"

//LOG_TAIL will add current filename , function-name, line-number
//such as str:<<  FileName=log.cpp  LineNum=40  FuncName=void a::b(log4cpp::Category&)
int main (int argc, char * argv[]){

    yLib::yLog::I( "class call" );
    yLib::yLog::D( ("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::W(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::E(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);

    yLib::yLog test;
    test.I(("object call"+ LOG_TAIL).c_str());
    test.D(("object call A %d, %f test ylog"+ LOG_TAIL).c_str(), 123, 54.5555);
    test.W(("object call A %d, %f test ylog"+ LOG_TAIL).c_str(), 123, 54.5555);


    yLib::yLog::I("class call");
    yLib::yLog::D(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::W(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::E(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);

    yLib::yLog log4(true);
    log4.I("object call" );
    log4.E(("object call A %d, %f test ylog" + LOG_TAIL).c_str(), 123, 54.5555);
    yLib::yLog::I("class call" );
    yLib::yLog::D(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::W(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::E(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);

    return 0;
}