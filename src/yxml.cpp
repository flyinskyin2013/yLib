#include "yxml.h"

yLib::yXML::yXML(){


}

yLib::yXML::~yXML(){

	if ( nullptr != _xmlfile_pdoc_ ){

		xmlFreeDoc(_xmlfile_pdoc_);
		xmlCleanupParser();
		xmlMemoryDump();
	}
}

int yLib::yXML::yXml_Read(std::string & xml_path){

    /*
	* this initialize the library and check potential ABI mismatches
	* between the version it was compiled for and the actual shared
	* library used.
	*/
	LIBXML_TEST_VERSION

	/*****************打开xml文档********************/

	xmlKeepBlanksDefault(0) ;//libxml2 global variable . 保证格式正确
	xmlIndentTreeOutput = 1 ;// indent .with \n 

	_xmlfile_pdoc_ = xmlReadFile(xml_path.c_str(), "UTF-8", XML_PARSE_RECOVER);//libxml只能解析UTF-8格式数据

	if (_xmlfile_pdoc_ == NULL)
	{

		yLib::yLog::E("error:can't open file!\n");
		return -1;
	}

	/*****************释放资源********************/
	//xmlSaveFile("avr_parameter.xml", pdoc);
	//xmlSaveFileEnc();
	// xmlSaveFormatFileEnc(_xmlfile_path.c_str(), pdoc, "UTF-8", 1);
    return 0;
}
static xmlNodePtr yXml_Tree_Traversal(xmlNodePtr ptr_node, \
    std::string &node_name, \
    std::string &node_prop_name, \
    std::string &node_prop_val, \
    std::string & child_node_name){

	xmlNodePtr child_node;


	while ( NULL != ptr_node ){

		//yLib::yLog::I((char *)ptr_node->name);
		if (!xmlStrcmp(ptr_node->name, BAD_CAST("comment"))){
			//Attention:
			//we deal with xml-comment-line
			//if xml node_name is comment,we do nothing
		}
		if (!xmlStrcmp(ptr_node->name, BAD_CAST(node_name.c_str()))){
			
			xmlChar *name = xmlGetProp(ptr_node, BAD_CAST(node_prop_name.c_str()));
			if ( !xmlStrcmp(name, BAD_CAST(node_prop_val.c_str())) ){

				yLib::yLog::E(("find prop "+node_prop_val).c_str());

				xmlNodePtr pcur_children = ptr_node->xmlChildrenNode;

				while (pcur_children != NULL)
				{
					if (!xmlStrcmp(pcur_children->name, BAD_CAST(child_node_name.c_str())))
					{
						char str[1024]{0};
						sprintf(str, "%s", ((char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode)));
						yLib::yLog::I(("read node val is " + std::string(str)).c_str());
						//xmlNodeSetContent(pcur_children->children, BAD_CAST(child_node_val.c_str()));
						//child_node_val = (char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode);
						//xmlNodeGetContent
						return pcur_children;
						break;//break while loop
					}

					pcur_children = pcur_children->next;
				}

				break; // break while loop
			}

		}
		if ( NULL != ptr_node->xmlChildrenNode )
			yXml_Tree_Traversal(ptr_node->xmlChildrenNode, node_name, \
					node_prop_name, node_prop_val, \
					child_node_name);
		ptr_node = ptr_node->next;
	}

	return NULL;
}

static void yXml_Tree_Traversal(xmlNodePtr ptr_node, \
    std::string &node_name, \
    std::string &node_prop_name, \
    std::string &node_prop_val, \
    std::string & child_node_name, \
    std::string &child_node_val){

    xmlNodePtr child_node;


	while ( NULL != ptr_node ){

		//yLib::yLog::I((char *)ptr_node->name);
		if (!xmlStrcmp(ptr_node->name, BAD_CAST("comment"))){
			//Attention:
			//we deal with xml-comment-line
			//if xml node_name is comment,we do nothing
		}
		if (!xmlStrcmp(ptr_node->name, BAD_CAST(node_name.c_str()))){
			
			xmlChar *name = xmlGetProp(ptr_node, BAD_CAST(node_prop_name.c_str()));
			if ( !xmlStrcmp(name, BAD_CAST(node_prop_val.c_str())) ){

				yLib::yLog::E(("find prop "+node_prop_val).c_str());

				xmlNodePtr pcur_children = ptr_node->xmlChildrenNode;

				while (pcur_children != NULL)
				{
					if (!xmlStrcmp(pcur_children->name, BAD_CAST(child_node_name.c_str())))
					{
						char str[1024]{0};
						sprintf(str, "%s", ((char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode)));
						yLib::yLog::I(("read node val is " + std::string(str)).c_str());
						xmlNodeSetContent(pcur_children->children, BAD_CAST(child_node_val.c_str()));
						//child_node_val = (char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode);
						//xmlNodeGetContent
						break;//break while loop
					}

					pcur_children = pcur_children->next;
				}

				break; // break while loop
			}

		}
		if ( NULL != ptr_node->xmlChildrenNode )
			yXml_Tree_Traversal(ptr_node->xmlChildrenNode, node_name, \
					node_prop_name, node_prop_val, \
					child_node_name, child_node_val);
		ptr_node = ptr_node->next;
	}


}

int yLib::yXML::yXml_Set_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string & child_node_val) const {

	xmlNodePtr proot = NULL, pcur = NULL;
	/*****************获取xml文档对象的根节对象********************/
	proot = xmlDocGetRootElement(_xmlfile_pdoc_);

	if (proot == NULL)
	{
		yLib::yLog::E("error: file is empty!\n");
		return -1;
	}

	pcur = proot->xmlChildrenNode;
    yXml_Tree_Traversal(pcur, node_name, \
        node_prop_name, node_prop_val, \
        child_node_name, child_node_val);

    return 0;
}

int yLib::yXML::yXml_Get_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string & child_node_val) const {

	xmlNodePtr proot = NULL, pcur = NULL;
	/*****************获取xml文档对象的根节对象********************/
	proot = xmlDocGetRootElement(_xmlfile_pdoc_);

	if (proot == NULL)
	{
		yLib::yLog::E("error: file is empty!\n");
		return -1;
	}

	pcur = proot->xmlChildrenNode;
	xmlNodePtr find_val;
    find_val = yXml_Tree_Traversal(pcur, node_name, \
        node_prop_name, node_prop_val, \
        child_node_name);

	if ( NULL != find_val ){

		child_node_val = (char*)XML_GET_CONTENT(find_val->xmlChildrenNode);
	}
	else{

		yLib::yLog::W("Can not find appropriate result");
	}

    return 0;
}

int yLib::yXML::yXml_Write(std::string & xml_path){

    xmlSaveFormatFileEnc(xml_path.c_str(), _xmlfile_pdoc_, "UTF-8", 1);
    return 0;
}