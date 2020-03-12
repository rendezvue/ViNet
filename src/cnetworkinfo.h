#ifndef CNETWORKINFO_H
#define CNETWORKINFO_H

#include <string>

class CNetworkInfo
{
public:
	CNetworkInfo(void) ;
 	virtual ~CNetworkInfo() ;

public:
	void SetNetworkInfo(const std::string ip, const int port) ;
	void GetNetworkInfo(std::string* out_ip, int* out_port) ;
	
private:
	std::string m_str_network_ip ;
	int m_network_port ;
};

#endif // CENSEMBLE_H
