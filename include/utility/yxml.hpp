/*
Copyright (c) 2018 - 2021 flyinskyin2013 All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



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


#include "core/ylog.hpp"
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