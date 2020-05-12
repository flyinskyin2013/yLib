/*
 * @Description: 
 * @Author: Sky
 * @Date: 2020-03-26 16:00:12
 * @LastEditors: Sky
 * @LastEditTime: 2020-04-03 17:34:35
 * @FilePath: \yLib\tests\yxml\yxml_tests.cpp
 * @Github: https://github.com/flyinskyin2013/yLib
 */
#include "catch2/catch.hpp"
#include "ylib.hpp"


TEST_CASE( "Test yXml apis" , "[yException_Apis]" ){

    SECTION("test ReadFromXmlFile() UpdateXmlValue() GetXmlValue() ") {

        yLib::yXml _xml;
        std::string xml_file = "test.xml";
        _xml.ReadFromXmlFile(xml_file);
        
        std::string parent_node_name;
        std::string node_name = "root_num_val";
        std::string node_val;
        std::string prop_node_name = "";
        std::string prop_node_val = "";

        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "12345" ));
        
        node_name = "root_val";
        prop_node_name = "name";
        prop_node_val = "root_str_val";
        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "test str root val" ));

        node_name = "root_val";
        prop_node_name = "prop";
        prop_node_val = "root_num_val";
        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "test num root val" ));


        yLib::yXmlOptsEx _opts_ex;

        _opts_ex.jump_times = 0;
        _opts_ex.parent_node_name = "sub_node";
        _opts_ex.parent_prop_name = "name";
        _opts_ex.parent_prop_val = "test";

        node_name = "test_node_num_val";
        prop_node_name = "";
        prop_node_val = "";
        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val, _opts_ex));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "first 72834" ));

        _opts_ex.jump_times = 1;
        _opts_ex.parent_node_name = "third_sub_node";
        _opts_ex.parent_prop_name = "name";
        _opts_ex.parent_prop_val = "test";

        node_name = "test_node_num_val";
        prop_node_name = "";
        prop_node_val = "";
        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val, _opts_ex));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "second 72834" ));

        _opts_ex.jump_times = 1;
        _opts_ex.parent_node_name = "sub_node";
        _opts_ex.parent_prop_name = "id";
        _opts_ex.parent_prop_val = "test_id";

        node_name = "test_node_num_val";
        prop_node_name = "id";
        prop_node_val = "1";
        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val, _opts_ex));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "third 72834" ));  


        
        _opts_ex.jump_times = 4;
        _opts_ex.parent_node_name = "sub_node";
        _opts_ex.parent_prop_name = "";
        _opts_ex.parent_prop_val = "";

        node_name = "test_node_str_val";
        prop_node_name = "id";
        prop_node_val = "1";
        REQUIRE(0 == _xml.GetXmlValue(node_name, prop_node_name, prop_node_val, node_val, _opts_ex));
        REQUIRE_THAT(node_val.c_str(), Catch::Equals ( "fourth node str val" ));  

    }
}