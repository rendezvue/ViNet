#ifndef CSETTOOLINFO_H
#define CSETTOOLINFO_H

#include <string>

class CSetToolInfo
{
public:
    CSetToolInfo();

public:
	void SetParentId(const std::string id) ;
	std::string GetParentId(void) ;

	void SetId(const std::string id) ;
	std::string GetId(void) ;

private:
	std::string m_str_parent_id ;
    std::string m_str_id ;
};

#endif // CSETTOOLINFO_H
