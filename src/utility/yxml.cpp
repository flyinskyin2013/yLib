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
 * @Date: 2018-10-23 11:09:25
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2021-09-05 16:22:23
 * @Description: 
 */

#include "utility/yxml.hpp"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#include <libxml/parser.h>
#include <libxml/tree.h>

#ifdef __cplusplus
}
#endif //__cplusplus




static xmlNodePtr __yXml_Tree_Traversal_Get__(xmlNodePtr ptr_node, \
    std::string &node_name, \
	std::string &node_val_, \
    std::string &node_prop_name, \
    std::string &node_prop_val, \
    int8_t & set_val_suc_, \
	int32_t * is_found_times = 0, \
	const yLib::yXmlOptsEx & opts_ex_ = yLib::yXmlOptsEx()){

	while ( NULL != ptr_node ){

		//yLib::yLog::I((char *)ptr_node->name);
		if (!xmlStrcmp(ptr_node->name, BAD_CAST("comment"))){
			//Attention:
			//we deal with xml-comment-line
			//if xml node_name is comment,we do nothing
		}
		
		if (!xmlStrcmp(ptr_node->name, BAD_CAST(node_name.c_str()))){
			
			bool _check_prop_name_result = false;

			//first check main prop 
			if ( "" != node_prop_name ){//first check prop 

				xmlChar *prop_val = xmlGetProp(ptr_node, BAD_CAST(node_prop_name.c_str()));
				if ( !xmlStrcmp(prop_val, BAD_CAST(node_prop_val.c_str())) ){

					_check_prop_name_result = true;
				}

				xmlFree(prop_val);//free prop-name
			}
			else{

				_check_prop_name_result = true;
			}

			bool _second_check_result = true;
			//second check opts_ex_
			if (-233333 != opts_ex_.jump_times && 0 <= opts_ex_.jump_times && _check_prop_name_result){//After check node_prop

				xmlNodePtr _ptr_parent = ptr_node->parent;

				if ( !xmlStrcmp(_ptr_parent->name, BAD_CAST(opts_ex_.parent_node_name.c_str()) )){//check parent

					if ( "" == opts_ex_.parent_prop_name ){

						if ( *is_found_times != opts_ex_.jump_times ){

							_check_prop_name_result = false;
							(*is_found_times)++;
						}
					}
					else{

						xmlChar *prop_val = xmlGetProp(_ptr_parent, BAD_CAST(opts_ex_.parent_prop_name.c_str()));
						if ( 0 != xmlStrcmp(prop_val, BAD_CAST(opts_ex_.parent_prop_val.c_str())) ){//check children prop

							_check_prop_name_result = false;
						}
						else{

							if ( *is_found_times != opts_ex_.jump_times ){

								_check_prop_name_result = false;
								(*is_found_times)++;
							}
						}
						xmlFree(prop_val);//free prop-name
					}


				}
				else{

					_second_check_result = false;
				}
			}
			
			if (_check_prop_name_result && _second_check_result){
				

				//char str[1024]{0};
				//sprintf(str, "%s", ((char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode)));
				//yLib::yLog::I(("read node val is " + std::string(str)).c_str());

				if (XML_ELEMENT_NODE == ptr_node->type){

					node_val_ = (char*)XML_GET_CONTENT(ptr_node->children);

					set_val_suc_ = 1;
					//child_node_val = (char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode);
					//xmlNodeGetContent

					return 0;//return when set val successfully
				}

			}

		}
		
		if ( NULL != ptr_node->xmlChildrenNode )
			__yXml_Tree_Traversal_Get__(ptr_node->xmlChildrenNode, node_name, \
					node_val_, \
					node_prop_name, \
					node_prop_val, \
					set_val_suc_, \
					is_found_times, \
					opts_ex_);
					
		ptr_node = ptr_node->next;
	}

	return NULL;
}

static int8_t __yXml_Tree_Traversal_Update__(xmlNodePtr ptr_node, \
    std::string &node_name, \
	std::string &node_val_, \
    std::string &node_prop_name, \
    std::string &node_prop_val, \
	int8_t & set_val_suc_, \
	int32_t * is_found_times = 0, \
	const yLib::yXmlOptsEx & opts_ex_ = yLib::yXmlOptsEx()){

	while ( NULL != ptr_node ){

		//yLib::yLog::I((char *)ptr_node->name);
		if (!xmlStrcmp(ptr_node->name, BAD_CAST("comment"))){
			//Attention:
			//we deal with xml-comment-line
			//if xml node_name is comment,we do nothing
		}
		
		if (!xmlStrcmp(ptr_node->name, BAD_CAST(node_name.c_str()))){
			
			bool _check_prop_name_result = false;

			if ( "" != node_prop_name ){

				xmlChar *name = xmlGetProp(ptr_node, BAD_CAST(node_prop_name.c_str()));
				if ( !xmlStrcmp(name, BAD_CAST(node_prop_val.c_str())) ){

					_check_prop_name_result = true;
				}

				xmlFree(name);//free prop-name
			}
			else{

				_check_prop_name_result = true;
			}


			bool _second_check_result = true;
			//second check opts_ex_
			if (-233333 != opts_ex_.jump_times && 0 <= opts_ex_.jump_times && _check_prop_name_result){//

				xmlNodePtr _ptr_parent = ptr_node->parent;

				if ( !xmlStrcmp(_ptr_parent->name, BAD_CAST(opts_ex_.parent_node_name.c_str()) )){//check parent

					if ( "" == opts_ex_.parent_prop_name ){

						if ( *is_found_times != opts_ex_.jump_times ){

							_check_prop_name_result = false;
							(*is_found_times)++;
						}
					}
					else{

						xmlChar *prop_val = xmlGetProp(_ptr_parent, BAD_CAST(opts_ex_.parent_prop_name.c_str()));
						if ( 0 != xmlStrcmp(prop_val, BAD_CAST(opts_ex_.parent_prop_val.c_str())) ){//check children prop

							_check_prop_name_result = false;
						}
						else{

							if ( *is_found_times != opts_ex_.jump_times ){

								_check_prop_name_result = false;
								(*is_found_times)++;
							}
						}
						xmlFree(prop_val);//free prop-name
					}


				}
				else{

					_second_check_result = false;
				}
			}

			if (_check_prop_name_result && _second_check_result){
				

				//char str[1024]{0};
				//sprintf(str, "%s", ((char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode)));
				//yLib::yLog::I(("read node val is " + std::string(str)).c_str());

				xmlNodeSetContent(ptr_node, BAD_CAST(node_val_.c_str()));

				set_val_suc_ = 1;
				//child_node_val = (char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode);
				//xmlNodeGetContent

				return 0;//return when set val successfully
			}

		}
		
		if ( NULL != ptr_node->xmlChildrenNode )
			__yXml_Tree_Traversal_Update__(ptr_node->xmlChildrenNode, node_name, \
					node_val_, \
					node_prop_name, \
					node_prop_val, \
					set_val_suc_, \
					is_found_times, \
					opts_ex_);
					
		ptr_node = ptr_node->next;
	}

	return 0;
}


static int8_t __yXml_Tree_Traversal_Add__(xmlNodePtr ptr_node, \
	std::string &parent_node_name_, \
    std::string &node_name, \
	std::string &node_val_, \
    std::string &node_prop_name, \
    std::string &node_prop_val, \
	int8_t & set_val_suc_, \
	int32_t * is_found_times = 0, \
	const yLib::yXmlOptsEx & opts_ex_ = yLib::yXmlOptsEx()){


	while ( NULL != ptr_node ){


		//yLib::yLog::I((char *)ptr_node->name);
		if (!xmlStrcmp(ptr_node->name, BAD_CAST("comment"))){
			//Attention:
			//we deal with xml-comment-line
			//if xml node_name is comment,we do nothing
		}
		
		if (!xmlStrcmp(ptr_node->name, BAD_CAST(parent_node_name_.c_str()))){
			
			bool _check_prop_name_result = false;

			if ( "" != node_prop_name ){

				xmlChar *name = xmlGetProp(ptr_node, BAD_CAST(node_prop_name.c_str()));
				if ( !xmlStrcmp(name, BAD_CAST(node_prop_val.c_str())) ){

					_check_prop_name_result = true;
				}

				xmlFree(name);//free prop-name
			}
			else{

				_check_prop_name_result = true;
			}


			bool _second_check_result = true;
			//second check opts_ex_
			if (-233333 != opts_ex_.jump_times && 0 <= opts_ex_.jump_times && _check_prop_name_result){//

				xmlNodePtr _ptr_parent = ptr_node->parent;

				if ( !xmlStrcmp(_ptr_parent->name, BAD_CAST(opts_ex_.parent_node_name.c_str()) )){//check parent

					if ( "" == opts_ex_.parent_prop_name ){

						if ( *is_found_times != opts_ex_.jump_times ){

							_check_prop_name_result = false;
							(*is_found_times)++;
						}
					}
					else{

						xmlChar *prop_val = xmlGetProp(_ptr_parent, BAD_CAST(opts_ex_.parent_prop_name.c_str()));
						if ( 0 != xmlStrcmp(prop_val, BAD_CAST(opts_ex_.parent_prop_val.c_str())) ){//check children prop

							_check_prop_name_result = false;
						}
						else{

							if ( *is_found_times != opts_ex_.jump_times ){

								_check_prop_name_result = false;
								(*is_found_times)++;
							}
						}
						xmlFree(prop_val);//free prop-name
					}


				}
				else{

					_second_check_result = false;
				}
			}

			
			if (_check_prop_name_result && _second_check_result){
				
				//char str[1024]{0};
				//sprintf(str, "%s", ((char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode)));
				//yLib::yLog::I(("read node val is " + std::string(str)).c_str());

				//TODO
				if ( "__comment__" == node_name ){//the node is comment

					xmlNodePtr _comment_ptr =  xmlNewComment(BAD_CAST(node_val_.c_str()));
					xmlAddChild(ptr_node,_comment_ptr);
				}
				else{
					
					xmlNewChild(ptr_node, NULL,BAD_CAST(node_name.c_str()), BAD_CAST(node_val_.c_str()));
					xmlNewProp(ptr_node, BAD_CAST(node_prop_name.c_str()), BAD_CAST(node_prop_val.c_str()));
				}
				set_val_suc_ = 1;
				//child_node_val = (char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode);
				//xmlNodeGetContent

				return 0;//return when set val successfully
			}

		}

		if ( NULL != ptr_node->xmlChildrenNode )
			__yXml_Tree_Traversal_Add__(ptr_node->xmlChildrenNode, \
					parent_node_name_, \
					node_name, \
					node_val_, \
					node_prop_name, \
					node_prop_val, \
					set_val_suc_, \
					is_found_times, \
					opts_ex_);
					
		ptr_node = ptr_node->next;
	}


}


//yXml class 
yLib::yXml::yXml() MACRO_INIT_YOBJECT_PROPERTY(yXml){

	
}

yLib::yXml::~yXml(){

	if (nullptr != xmlfile_pdoc_ptr)
		xmlFreeDoc(xmlfile_pdoc_ptr);
}

int8_t yLib::yXml::ReadFromXmlFile(std::string & xml_file_){

	    /*
	* this initialize the library and check potential ABI mismatches
	* between the version it was compiled for and the actual shared
	* library used.
	*/
	LIBXML_TEST_VERSION

	/*****************打开xml文档********************/

	xmlKeepBlanksDefault(0) ;//libxml2 global variable . 保证格式正确
	xmlIndentTreeOutput = 1 ;// indent .with \n 

	xmlfile_pdoc_ptr = NULL;
	xmlfile_pdoc_ptr = xmlReadFile(xml_file_.c_str(), "UTF-8", XML_PARSE_NOBLANKS | XML_PARSE_RECOVER);//libxml只能解析UTF-8格式数据

	if (xmlfile_pdoc_ptr == NULL)
	{

		yLib::yLog::E("ReadFromXmlFile():can't read xml file!");
		return -1;
	}

	/*****************释放资源********************/
	//xmlSaveFile("avr_parameter.xml", pdoc);
	//xmlSaveFileEnc();
	// xmlSaveFormatFileEnc(_xmlfile_path.c_str(), pdoc, "UTF-8", 1);
    return 0;
}

int8_t yLib::yXml::UpdateXmlValue(std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, const std::string &node_val_, const yXmlOptsEx & opts_ex_){

	xmlNodePtr _proot = NULL, _pcur = NULL;
	/*****************获取xml文档对象的根节对象********************/
	_proot = xmlDocGetRootElement(xmlfile_pdoc_ptr);
	
	if (_proot == NULL)
	{
		yLib::yLog::E("UpdateXmlValue(): get xml root node failed.");
		return -1;
	}

	int8_t set_ret = 0;
	int32_t is_found_times = 0;
	
	__yXml_Tree_Traversal_Update__(_proot, node_name_, const_cast<std::string&>(node_val_), node_prop_name_, node_prop_val_, set_ret, &is_found_times, opts_ex_);

	if (1 == set_ret)
    	return 0;
	else 
		return -1;
}

int8_t yLib::yXml::GetXmlValue(std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, std::string &node_val_, const yXmlOptsEx & opts_ex_) const{

	xmlNodePtr _proot = NULL, _pcur = NULL;
	/*****************获取xml文档对象的根节对象********************/
	_proot = xmlDocGetRootElement(xmlfile_pdoc_ptr);
	
	if (_proot == NULL)
	{
		yLib::yLog::E("GetXmlValue(): get xml root node failed.");
		return -1;
	}

	int8_t set_ret = 0;
	int32_t is_found_times = 0;

	__yXml_Tree_Traversal_Get__(_proot, node_name_, node_val_, node_prop_name_, node_prop_val_, set_ret, &is_found_times, opts_ex_);

	if (1 == set_ret)
    	return 0;
	else 
		return -1;
}

int8_t yLib::yXml::AddXmlValue(std::string & parent_node_name_, std::string &node_name_, std::string &node_prop_name_, std::string &node_prop_val_, const std::string &node_val_, const yXmlOptsEx & opts_ex_){

	xmlNodePtr _proot = NULL, _pcur = NULL;
	/*****************获取xml文档对象的根节对象********************/
	_proot = xmlDocGetRootElement(xmlfile_pdoc_ptr);
	
	if (_proot == NULL)
	{
		yLib::yLog::E("GetXmlValue(): get xml root node failed.");
		return -1;
	}

	int8_t set_ret = 0;
	int32_t is_found_times = 0;
	__yXml_Tree_Traversal_Add__(_proot, parent_node_name_, node_name_, const_cast<std::string&>(node_val_), node_prop_name_, node_prop_val_, set_ret, &is_found_times, opts_ex_);
	
	if (1 == set_ret)
    	return 0;
	else 
		return -1;
}


int8_t yLib::yXml::WriteToXmlFile(std::string & xml_file_){

    if ( 0 > xmlSaveFormatFileEnc(xml_file_.c_str(), xmlfile_pdoc_ptr, "UTF-8", 1) ){
		
		yLib::yLog::E("WriteToXmlFile():can't write xml file!");
		return -1;
	}

    return 0;
}
