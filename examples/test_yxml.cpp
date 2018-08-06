#include "ylib.h"


int main (int argc, char * argv[]){

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
    return 0;
}