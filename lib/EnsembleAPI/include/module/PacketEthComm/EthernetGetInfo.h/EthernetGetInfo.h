#ifndef ETHERNET_GET_ACCEPTOR_H
#define ETHERNET_GET_ACCEPTOR_H

#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdint.h>

//#include <qdebug.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "EnsembleCommon.h"
#include "EthernetGetInfo.h"

using namespace boost::asio;
using boost::asio::ip::tcp;
using namespace std;


class  CEthernetGetInfo
{
public:

	static CEthernetGetInfo* getInstance();
	virtual ~CEthernetGetInfo() ;
	
	boost::asio::io_service* GetIoService(void) ;
private:

	CEthernetGetInfo(void) ;
	static bool instanceFlag;
	static CEthernetGetInfo* instance;

	boost::asio::io_service *m_io_service;
};

#endif
