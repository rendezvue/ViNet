#ifndef CPARSINGADDABLEJOBLIST_H
#define CPARSINGADDABLEJOBLIST_H

#include <string>
#include <vector>
#include "pugixml.hpp"

#include <QDebug>

typedef struct _AddableJobInfo
{	
	int type ;
	std::string name ;
	std::string description ;
}AddableJobInfo;

class CParsingAddableJobList
{
public:
    CParsingAddableJobList();

	std::vector<AddableJobInfo> GetAddableJobList(const std::string str_xml) ;
};

#endif // CPARSINGADDABLEJOBLIST_H