/*
 * @Author: Sky
 * @Date: 2019-09-20 10:11:02
 * @LastEditors: Sky
 * @LastEditTime: 2019-11-28 19:09:22
 * @Description: 
 */

#include "ylib.hpp"

#include <iostream>

//LOG_TAIL will add current filename , function-name, line-number
//such as str:<<  FileName=log.cpp  LineNum=40  FuncName=void a::b(log4cpp::Category&)
int main (int argc, char * argv[]){

    std::cout<<" =======================test yLib::yLog==========================="<<std::endl;

    yLib::yLog::I( "class call" );
    yLib::yLog::D( ("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::W(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
    yLib::yLog::E(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);

    yLib::yLog::SetLog4cpp(true);//open log4cpp
    
    std::string log_idx0 = "test_ylog0";
    std::string log_idx1 = "test_ylog1";
    
    yLib::yLog::SetLog4cppSubCategory(log_idx0);
    yLib::yLog::SetLog4cppSubCategory(log_idx1);

    yLib::yLog::SetyLogLogLevel(yLib::yLogLevel::_DISABLE_WARN_LOG_LEVEL_);
    yLib::yLog::SetyLogLogLevel(log_idx0,yLib::yLogLevel::_DISABLE_INFO_LOG_LEVEL_);
    yLib::yLog::SetyLogLogLevel(log_idx1,yLib::yLogLevel::_DISABLE_DEBUG_LOG_LEVEL_);

    yLib::yLog::SetLog4cppLogLevel(yLib::yLogLevel::_DISABLE_ERROR_LOG_LEVEL_);
    yLib::yLog::SetLog4cppLogLevel(log_idx0, yLib::yLogLevel::_DISABLE_INFO_LOG_LEVEL_);
    yLib::yLog::SetLog4cppLogLevel(log_idx1,yLib::yLogLevel::_DISABLE_DEBUG_LOG_LEVEL_);

	int i = 0;
    while(i < 10){

        i++;
        
        yLib::yLog::I("object call" );
        yLib::yLog::E(("object call A %d, %f test ylog" + LOG_TAIL).c_str(), 123, 54.5555);
        yLib::yLog::I("class call" );
        yLib::yLog::D(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
        yLib::yLog::W(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);
        yLib::yLog::E(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 1234, 66.5555);

        yLib::yLog::I(log_idx0, "object call" );
        yLib::yLog::E(log_idx0, ("object call A %d, %f test_ylog0" + LOG_TAIL).c_str(), 123, 54.5555);
        yLib::yLog::I(log_idx0, "class call" );
        yLib::yLog::D(log_idx0, ("class call A %d, %f test_ylog0" + LOG_TAIL).c_str(), 1234, 66.5555);
        yLib::yLog::W(log_idx0, ("class call A %d, %f test_ylog0" + LOG_TAIL).c_str(), 1234, 66.5555);
        yLib::yLog::E(log_idx0, ("class call A %d, %f test_ylog0" + LOG_TAIL).c_str(), 1234, 66.5555);

        yLib::yLog::I(log_idx1, "object call" );
        yLib::yLog::E(log_idx1, ("object call A %d, %f test_ylog1" + LOG_TAIL).c_str(), 123, 54.5555);
        yLib::yLog::I(log_idx1, "class call" );
        yLib::yLog::D(log_idx1, ("class call A %d, %f test_ylog1" + LOG_TAIL).c_str(), 1234, 66.5555);
        yLib::yLog::W(log_idx1, ("class call A %d, %f test_ylog1" + LOG_TAIL).c_str(), 1234, 66.5555);
        yLib::yLog::E(log_idx1, ("class call A %d, %f test_ylog1" + LOG_TAIL).c_str(), 1234, 66.5555);
    }
    
    yLib::yLog::SetLog4cpp(false);
    yLib::yLog::I("object call" );
    yLib::yLog::E(("object call A %d, %f test ylog" + LOG_TAIL).c_str(), 123456, 88.5555);
    yLib::yLog::I("class call" );
    yLib::yLog::D(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 123456, 88.5555);
    yLib::yLog::W(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 123456, 88.5555);
    yLib::yLog::E(("class call A %d, %f test ylog" + LOG_TAIL).c_str(), 123456, 88.5555);


//#define _TEST_ISSUE_USE_XML_LOG_AT_SAME_TIME_SEGMENT_FAULT_ 1
#ifdef _TEST_ISSUE_USE_XML_LOG_AT_SAME_TIME_SEGMENT_FAULT_
    yLib::yXML xml;

    std::string file = "test.xml";
    xml.yXml_Read(file);

    std::string node_name = "abc_xml";
    std::string node_prop_name = "id";
    std::string node_prop_val = "aaa";
    std::string child_node_name = "testval";
    std::string child_node_val = "you are aaaaaa";

    xml.yXml_Set_Val(node_name, node_prop_name, node_prop_val, child_node_name, child_node_val);

    node_name = "regist_device";
    node_prop_name = "id";
    node_prop_val = "hhh";
    child_node_name = "serial_num";
    xml.yXml_Get_Val(node_name, node_prop_name, node_prop_val, child_node_name, child_node_val);

    yLib::yLog::I(("serial_num = "+child_node_val).c_str());

    std::string file1 = "test.xml";
    xml.yXml_Write(file1);
#endif //_TEST_ISSUE_USE_XML_LOG_AT_SAME_TIME_SEGMENT_FAULT_
    return 0;
}
