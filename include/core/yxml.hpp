/*
 * @Author: Sky
 * @Date: 2019-09-20 16:50:06
 * @LastEditors: Sky
 * @LastEditTime: 2020-04-02 09:07:59
 * @Description: 
 */
#ifndef _YLIB_CORE_YXML_HPP_
#define _YLIB_CORE_YXML_HPP_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

typedef struct _xmlDoc xmlDoc;
typedef xmlDoc * xmlDocPtr;

#ifdef __cplusplus
}
#endif //__cplusplus

#include <string>


#include "ylog.hpp"
#include "ycommon.hpp"
namespace yLib{

    class __YLIB_DEPRECATED_ATTRIBUTE__ yXML MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        yXML();
        ~yXML();
        int yXml_Read(std::string & xml_path);
        int yXml_Set_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string &child_node_val);
        int yXml_Get_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string &child_node_val) const;
        int yXml_Write(std::string & xml_path);
        protected:
        private:
        xmlDocPtr _xmlfile_pdoc_;
    };


    class yXml MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        yXml();
        ~yXml();

        int8_t ReadFromXmlFile(std::string & xml_file_);
        int8_t SetXmlValue(std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, std::string & child_node_name_, std::string &child_node_val_);
        int8_t GetXmlValue(std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, std::string & child_node_name_, std::string &child_node_val_) const;
        int8_t WriteToXmlFile(std::string & xml_file_);

        private:
        xmlDocPtr xmlfile_pdoc_ptr = NULL;
    };
}

#endif //_YXML_H_