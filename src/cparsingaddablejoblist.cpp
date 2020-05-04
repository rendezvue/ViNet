#include "cparsingaddablejoblist.h"

CParsingAddableJobList::CParsingAddableJobList()
{

}

std::vector<AddableJobInfo> CParsingAddableJobList::GetAddableJobList(const std::string str_xml)
{
	std::vector<AddableJobInfo> ret ;

	qDebug("GetAddableJobList : %s", str_xml.c_str()) ;
	
	//Upate Job List
	//XML Parsing
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string((char *)(str_xml.c_str()));

	if (!result)
	{
		qDebug("Addable Job list xml parsing error") ;
	}
	else
	{
		for (pugi::xml_node tool: doc.child("Ability").child("Jobs").children("Job"))
		{
			int type = tool.attribute("Type").as_int();
			std::string str_name = tool.attribute("Name").value();
			std::string str_description = tool.attribute("Description").value();

			AddableJobInfo info ;
			info.type = type ;
			info.name = str_name ;
			info.description = str_description ;

			ret.push_back(info) ;
		}
	}
		
	return ret ;
}

