#include "cnetworkinfo.h"

CNetworkInfo::CNetworkInfo()
{

}

CNetworkInfo::~CNetworkInfo(void)
{
}

void CNetworkInfo::SetNetworkInfo(const std::string ip, const int port)
{
	m_str_network_ip = ip ;
	m_network_port = port ;
}

void CNetworkInfo::GetNetworkInfo(std::string* out_ip, int* out_port)
{
	if( out_ip ) (*out_ip) = m_str_network_ip ;
	if( out_port ) (*out_port) = m_network_port ;
}

std::string CNetworkInfo::GetNetworkInfo_Ip_Address(void)
{
	return m_str_network_ip ;
}

int CNetworkInfo::GetNetworkInfo_Port(void) 
{
	return m_network_port ;
}

