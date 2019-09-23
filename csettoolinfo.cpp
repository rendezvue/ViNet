#include "csettoolinfo.h"

CSetToolInfo::CSetToolInfo()
{

}

void CSetToolInfo::SetParentId(const std::string id)
{
    m_str_parent_id = id ;
}

std::string CSetToolInfo::GetParentId(void)
{
    return m_str_parent_id ;
}

void CSetToolInfo::SetId(const std::string id)
{
    m_str_id = id ;
}

std::string CSetToolInfo::GetId(void)
{
    return m_str_id ;
}

