#ifndef _YXML_H_
#define _YXML_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef __cplusplus
}
#endif //__cplusplus

#include <string>
#include "ylog.h"

namespace yLib{


    class yXML{

        public:
        yXML();
        ~yXML();
        int yXml_Read(std::string & xml_path);
        int yXml_Set_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string &child_node_val) const;
        int yXml_Get_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string &child_node_val) const;
        int yXml_Write(std::string & xml_path);
        protected:
        private:
        xmlDocPtr _xmlfile_pdoc_;
    };
}

#endif //_YXML_H_