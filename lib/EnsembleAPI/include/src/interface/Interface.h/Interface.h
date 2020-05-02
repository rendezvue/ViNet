#ifndef INTERFACE_H
#define INTERFACE_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include "EthernetClient.h"

//nclude <qdebug.h>

#include <string>

#include "EnsembleCommon.h"

#define SEND_DATA_SIZE		1024

class CInterface 
{
public:
	CInterface(void) ;
	~CInterface(void);

	int CreateSocket(const char* ip, const unsigned int port);
	void DeleteSocket();

protected :
	boost::mutex mutex;

    CEthernetClient m_cls_eth_client;
	
	unsigned char* m_p_send_data ;
	unsigned int m_i_send_data_size ;

	std::string m_ip;
	unsigned int m_port ;
};

#endif
