/*
 * @Author: Sky
 * @Date: 2019-09-20 16:50:06
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 16:21:50
 * @Description: 
 */
#ifndef __YLIB_UTILIY_YXML_HPP__
#define __YLIB_UTILIY_YXML_HPP__

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
#include "core/ycommon.hpp"
namespace yLib{

    typedef struct __yxml_opts_ex__{
        
        std::string parent_node_name = "";
        std::string parent_prop_name = "";
        std::string parent_prop_val = "";

        int32_t jump_times = -233333;//magic num
    } yXmlOptsEx;

    class __YLIB_CLASS_DECLSPEC__ yXml MACRO_PUBLIC_INHERIT_YOBJECT{

        public:
        yXml();
        ~yXml();

        int8_t ReadFromXmlFile(std::string & xml_file_);
        //When node_prop_name_ is "",we will jump this operation that is check-node_prop_name_.
        //When node_name_ is "__comment__", the type of new created node is comment.
        //About opts_ex_ param : Defaultly, we will process the first node that name is parent_node_name_.
        int8_t AddXmlValue(std::string & parent_node_name_, std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, const std::string &node_val_, const yXmlOptsEx & opts_ex_ = yXmlOptsEx());
        //When node_prop_name_ is "",we will jump this operation that is check-node_prop_name_.
        //About opts_ex_ param : Defaultly, we will process the first node that name is node_name_.
        int8_t UpdateXmlValue(std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, const std::string &node_val_, const yXmlOptsEx & opts_ex_ = yXmlOptsEx());
        //When node_prop_name_ is "",we will jump this operation that is check-node_prop_name_.
        //About opts_ex_ param : Defaultly, we will process the first node that name is node_name_.
        int8_t GetXmlValue(std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_,  std::string &node_val_, const yXmlOptsEx & opts_ex_ = yXmlOptsEx()) const;
        int8_t WriteToXmlFile(std::string & xml_file_);

        private:
        xmlDocPtr xmlfile_pdoc_ptr = NULL;
    };
}

#endif //__YLIB_UTILIY_YXML_HPP__