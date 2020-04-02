/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:50
 * @LastEditors: Sky
 * @LastEditTime: 2019-09-20 16:57:05
 * @Description: 
 */


#include "ylib.hpp"


int main (int argc, char * argv[]){

    yLib::yConfig config;

    config.yConfigReadFile("test.conf");
    /* old api
    std::cout << "read int value :"<< config.yConfigGetIntValue("test.int") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetBoolValue("test.bool") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetFloatValue("test.float") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetStringValue("test.string") << std::endl;
    */
    std::cout << "read int value :"<< (int)config.yConfigGetValue("test.int") << std::endl;
    std::cout << "read bool value :"<< (bool)config.yConfigGetValue("test.bool") << std::endl;
    std::cout << "read float value :"<< (float)config.yConfigGetValue("test.float") << std::endl;
    std::cout << "read string value :"<< (std::string)config.yConfigGetValue("test.string") << std::endl;

    yLib::yLog::I("set node");
    yLib::yConfigValue input_value;
    input_value = "test yConfigSetValue ok";
    config.yConfigSetValue("test.string", input_value);
    input_value = 6666;
    config.yConfigSetValue("test.int", input_value);

    yLib::yLog::I("add node");
    input_value = "test  yConfigAddNode  ok";
    config.yConfigAddNode(".", "AddNode", yLib::yConfigValueType::STRING_TYPE, input_value);
    config.yConfigAddNode(".", "AddNodeGroup", yLib::yConfigValueType::GROUP_TYPE);
    input_value = 23333;
    config.yConfigAddNode("AddNodeGroup", "add_sub_node", yLib::yConfigValueType::INT_TYPE, input_value);

    yLib::yLog::I("output");
    config.yConfigWriteFile("out.cfg");
    return 0;
}