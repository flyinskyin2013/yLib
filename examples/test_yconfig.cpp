/*
 * @Author: Sky
 * @Date: 2018-10-23 11:09:50
 * @LastEditors: Sky
 * @LastEditTime: 2020-12-07 11:41:49
 * @Description: 
 */


#include "ylib.hpp"


int main (int argc, char * argv[]){

    yLib::yConfig config;

    config.ReadFile("test.conf");
    /* old api
    std::cout << "read int value :"<< config.yConfigGetIntValue("test.int") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetBoolValue("test.bool") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetFloatValue("test.float") << std::endl;
    std::cout << "read int value :"<< config.yConfigGetStringValue("test.string") << std::endl;
    */
    std::cout << "read int value :"<< (int)config.GetValue("test.int") << std::endl;
    std::cout << "read bool value :"<< (bool)config.GetValue("test.bool") << std::endl;
    std::cout << "read float value :"<< (float)config.GetValue("test.float") << std::endl;
    std::cout << "read string value :"<< (std::string)config.GetValue("test.string") << std::endl;

    yLib::yLog::I("set node");
    yLib::yConfigValue input_value;
    input_value = "test yConfigSetValue ok";
    config.SetValue("test.string", input_value);
    input_value = 6666;
    config.SetValue("test.int", input_value);

    yLib::yLog::I("add node");
    input_value = "test  yConfigAddNode  ok";
    config.AddNode(".", "AddNode", yLib::yConfigValue::STRING_TYPE, input_value);
    config.AddNode(".", "AddNodeGroup", yLib::yConfigValue::GROUP_TYPE);
    input_value = 23333;
    config.AddNode("AddNodeGroup", "add_sub_node", yLib::yConfigValue::INT32_TYPE, input_value);

    yLib::yLog::I("output");
    config.WriteFile("out.cfg");
    return 0;
}