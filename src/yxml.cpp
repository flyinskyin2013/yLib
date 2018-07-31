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

int yLib::yXML::yXml_Set_Val(std::string &node_name, std::string &node_prop_name, std::string &node_prop_val, std::string & child_node_name, std::string child_node_val){

	xmlNodePtr proot = NULL, pcur = NULL;
	/*****************获取xml文档对象的根节对象********************/
	proot = xmlDocGetRootElement(_xmlfile_pdoc_);

	if (proot == NULL)
	{
		yLib::yLog::E("error: file is empty!\n");
		return -1;
	}

	pcur = proot->xmlChildrenNode;
	while (pcur != NULL)
	{
		if (!xmlStrcmp(pcur->name, BAD_CAST(node_name.c_str())))
		{

			xmlChar *name = xmlGetProp(pcur, BAD_CAST(node_prop_name.c_str()));

			if ( !xmlStrcmp(name, BAD_CAST(node_prop_val.c_str())) ){

				yLib::yLog::E(("find prop"+node_prop_val).c_str());
				xmlNodePtr pcur_children = pcur->xmlChildrenNode;

				while (pcur_children != NULL)
				{
					if (!xmlStrcmp(pcur_children->name, BAD_CAST(child_node_name.c_str())))
					{
						char str[1024]{0};
						sprintf(str, "%s", ((char*)XML_GET_CONTENT(pcur_children->xmlChildrenNode)));
						yLib::yLog::I(("read node val is " + std::string(str)).c_str());
						xmlNodeSetContent(pcur_children->children, BAD_CAST(child_node_val.c_str()));
					}

					pcur_children = pcur_children->next;
				}
			}

			xmlFree(name);

		}

		pcur = pcur->next;
	}
    return 0;
}

int yLib::yXML::yXml_Write(std::string & xml_path){

    xmlSaveFormatFileEnc(xml_path.c_str(), _xmlfile_pdoc_, "UTF-8", 1);
    return 0;
}