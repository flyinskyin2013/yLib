#include "yxml.h"


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

    std::string file1 = "test.xml";
    xml.yXml_Write(file1);
    return 0;
}