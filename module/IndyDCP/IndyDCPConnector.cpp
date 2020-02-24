/*
 * IndyDCPConnector.cpp
 *
 *  Created on: 2019. 1. 10.
 *      Author: Hanter Jung
 */

#include "IndyDCPConnector.h"

namespace NRMKIndy
{
namespace Service
{
namespace DCP
{

uint32_t IndyDCPConnector::getStatusBits()
{
	return _statusBits;
}

bool IndyDCPConnector::getStatus(uint32_t statusType)
{
	return (bool)(_statusBits & statusType);
}

bool IndyDCPConnector::connect()
{
#if defined (WINDOWS)
	SOCKADDR_IN serverAddr;
	serverAddr.sin_addr.s_addr = inet_addr(_serverIP.c_str());
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);

	_mtx.lock();
	_sockFd = ::socket(AF_INET, SOCK_STREAM, 0 /*IPPROTO_TCP*/);
	if (_sockFd == INVALID_SOCKET)
	{
		_mtx.unlock();
		throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
			": open socket: ", toErrString(WSAGetLastError())));
		return false;
	}

	if (::connect(_sockFd, reinterpret_cast<SOCKADDR *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (_sockFd != INVALID_SOCKET) ::closesocket(_sockFd);
		_sockFd = INVALID_SOCKET;
		_connected = false;
		_mtx.unlock();

		if (err == ECONNREFUSED)
		{
			//Connection Refused by remote address
		}
		else
		{
			throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": connect: ", toErrString(err)));
		}
		return false;
	}
	else
	{
		//SET non-blocking sokcet (MSG_DONTWAIT)
		unsigned long arg = 1;
		int ioctrlErr = ioctlsocket(_sockFd, FIONBIO, &arg);
		if (ioctrlErr != 0)
		{
			if (_sockFd != INVALID_SOCKET) ::closesocket(_sockFd);
			_sockFd = INVALID_SOCKET;
			_connected = false;
			_mtx.unlock();
			throw RunTimeException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": cannot set socket option: ", toErrString(ioctrlErr)));
			return false;
		}

		_mtx.unlock();
		return true;
	}

#else
	struct sockaddr_in serverAddr;
	serverAddr.sin_addr.s_addr = inet_addr(_serverIP.c_str());
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);

	_mtx.lock();
	_sockFd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd < 0) {
		_mtx.unlock();
		throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": open socket: ", toErrString(errno)));
		return false;
	}

	if (::connect(_sockFd, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
		int err = errno;
		if (_sockFd >= 0) ::close(_sockFd);
		_sockFd = -1;
		_connected = false;
		_mtx.unlock();

		if (err == ECONNREFUSED)
		{
			//Connection Refused by remote address
		}
		else
		{
			throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": connect: ", toErrString(err)));
		}
		return false;
	}
	else
	{
		//SET non-blocking sokcet (MSG_DONTWAIT)
		int curFlags = fcntl(_sockFd, F_GETFL, 0);
		int fcntlErr = fcntl(_sockFd, F_SETFL, curFlags|O_NONBLOCK);
		if (fcntlErr < 0)
		{
			if (_sockFd >= 0) ::close(_sockFd);
			_sockFd = -1;
			_connected = false;
			_mtx.unlock();
			throw RunTimeException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": cannot set socket option"));
			return false;
		}

		_mtx.unlock();
		return true;
	}
#endif
}

void IndyDCPConnector::disconnect()
{
//	_mtx.unlock();

	_mtx.lock();
#if defined (WINDOWS)
	if (_sockFd != INVALID_SOCKET)
	{
		try {
			::closesocket(_sockFd);
		}
		catch (std::exception) {}
	}
	_sockFd = INVALID_SOCKET;
#else
	if (_sockFd >= 0)
	{
		try {
			::close(_sockFd);
		} catch (std::exception) {}
	}
	_sockFd = -1;
#endif
	_connected = false;
	_currTimeout = 0;
	_mtx.unlock();
}

void IndyDCPConnector::shutdown()
{
//	_mtx.unlock();

	_mtx.lock();
#if defined (WINDOWS)
	if (_sockFd != INVALID_SOCKET)
	{
		try {
			::shutdown(_sockFd, SD_BOTH);
		}
		catch (std::exception) {}
	}
	_sockFd = INVALID_SOCKET;
#else
	if (_sockFd >= 0)
	{
		try {
			::shutdown(_sockFd, SHUT_WR);
		} catch (std::exception) {}
	}
	_sockFd = -1;
#endif
	_connected = false;
	_currTimeout = 0;
	_mtx.unlock();
}

bool IndyDCPConnector::isConnected()
{
#if 0		//just connected
	return _connected;

#elif 0		//return with checking any error
	_mtx.lock();

	int err = 0;
	socklen_t len = sizeof (err);
	int retval = ::getsockopt (_sockFd, SOL_SOCKET, SO_ERROR, &err, &len);
	if (retval != 0) {
		if (_sockFd >= 0) ::close(_sockFd);
		_sockFd = -1;
		_connected = false;
		_mtx.unlock();
		throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": get socket err code: ", toErrString(errno)));
		return _connected;
	}
	if (err != 0) {
		if (_sockFd >= 0) ::close(_sockFd);
		_sockFd = -1;
		_connected = false;
		_mtx.unlock();
		throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": socket err: ", toErrString(err)));
		return _connected;
	}

	return _connected;

#else	//checking only connencted (keepalive)
	_mtx.lock();

#if defined (WINDOWS)
	char opt = 0;
	int len = sizeof(opt);
	int retval = ::getsockopt(_sockFd, SOL_SOCKET, SO_KEEPALIVE, &opt, &len);
	if (retval != 0) {
		if (_sockFd != INVALID_SOCKET) ::closesocket(_sockFd);
		_sockFd = INVALID_SOCKET;
		_connected = false;
		_mtx.unlock();
		throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
			": get connection err code: ", toErrString(WSAGetLastError())));
		return _connected;
	}
	if (opt == 0) {		//disconnected
		if (_sockFd != INVALID_SOCKET) ::closesocket(_sockFd);
		_sockFd = INVALID_SOCKET;
		_connected = false;
	}
#else
	char opt = 0;
	socklen_t len = sizeof (opt);
	int retval = ::getsockopt (_sockFd, SOL_SOCKET, SO_KEEPALIVE, &opt, &len);
	if (retval != 0) {
		if (_sockFd >= 0) ::close(_sockFd);
		_sockFd = -1;
		_connected = false;
		_mtx.unlock();
		throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": get connection err code: ", toErrString(errno)));
		return _connected;
	}
	if (opt == 0) {		//disconnected
		if (_sockFd >= 0) ::close(_sockFd);
		_sockFd = -1;
		_connected = false;
	}
#endif

	_mtx.unlock();
	return _connected;
#endif
}

void IndyDCPConnector::setTimeoutSeconds(double seconds)
{
	if (seconds <= 0.0)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": set timeout error: seconds=", toString(seconds)));
	}
	_limitTimeout = seconds;
}

int IndyDCPConnector::_handleCommand(int cmd,
		int & resDataSize, Data &resData,
		int reqDataSize, const Data & reqData)
{
	_mtx.lock();
	_currTimeout = 0;

	unsigned char writeBuff[1024];
	unsigned char readBuff[1024];
	bool isError = false;

	if (reqDataSize > SIZE_DCP_DATA_TCP_MAX || reqDataSize < 0)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": send data: data size error: size=", toString(reqDataSize)));
	}

	//Make Request Header
	HeaderCommand reqHeader;
	STRS_STRCPY(reqHeader.val.robotName, _robot.c_str());
	reqHeader.val.robotVersion[0] = '\0';
	reqHeader.val.stepInfo = _step;
	reqHeader.val.sof = 0x34;
	reqHeader.val.invokeId = ++_invokeCount;
	reqHeader.val.cmdId = cmd;
	reqHeader.val.dataSize = reqDataSize;

	//Send Packet to Socket
	memcpy(writeBuff, reqHeader.byte, SIZE_DCP_HEADER_COMMAND);
	if (reqDataSize > 0)
		memcpy(writeBuff+SIZE_DCP_HEADER_COMMAND, reqData.byte, reqDataSize);
	_sendMessage(writeBuff, SIZE_DCP_HEADER_COMMAND+reqDataSize);


	//Response Header, Data
	HeaderCommand resHeader;

	//Recv Header from Socket
	_recvMessage(readBuff, SIZE_DCP_HEADER_COMMAND);
	memcpy(resHeader.byte, readBuff, SIZE_DCP_HEADER_COMMAND);

	//Check Header
	if (strcmp(reqHeader.val.robotName, resHeader.val.robotName))
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: robot name error: name=", std::string(resHeader.val.robotName)));
	}
	else if (reqHeader.val.stepInfo != resHeader.val.stepInfo)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: step version error: version=", toString(resHeader.val.stepInfo)));
	}
	else if (reqHeader.val.invokeId != resHeader.val.invokeId)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: invoke id error: id=", toString(resHeader.val.invokeId)));
	}
	else if (resHeader.val.sof != 0x12)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: source of frame error: sof=", toString(resHeader.val.sof)));
	}
	else if (reqHeader.val.cmdId != resHeader.val.cmdId)
	{
		if (resHeader.val.cmdId == CMD_ERROR)
		{
			isError = true;
		}
		else
		{
			_mtx.unlock();
			disconnect();
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv header: different command id: cmd=", toString(resHeader.val.cmdId)));
		}
	}

	//Set Status Bits
	_statusBits = resHeader.val.status;

	//Recv Data from Socket
	resDataSize = resHeader.val.dataSize;
	if (resDataSize > SIZE_DCP_DATA_TCP_MAX || resDataSize < 0)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: data size error: size=", toString(resDataSize)));
	}
	else if (resDataSize > 0)
	{
		_recvMessage(readBuff, resDataSize);
		memcpy(resData.byte, readBuff, resDataSize);
	}

	int errorCode = ERR_NONE;
	if (isError)
	{
		if (resDataSize != sizeof(int)/*4*/)
		{
			_mtx.unlock();
			disconnect();
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv data: cmd error data size error: size=", toString(resDataSize)));
		}
		else
		{
			errorCode = resData.intVal;
		}
	}

	_currTimeout = 0;
	_mtx.unlock();
	return errorCode;
}

int IndyDCPConnector::_handleExtCommand(int extCmd,
		int & resExtDataSize, unsigned char *& resExtData,
		int reqExtDataSize, const unsigned char * reqExtData)
{
	_mtx.lock();
	_currTimeout = 0;

	unsigned char writeBuff[1024];
	unsigned char readBuff[1024];
	bool isError = false;

#if defined (WINDOWS)
	if (reqExtDataSize < 0 || reqExtDataSize > INT_MAX)
#else
	if (reqExtDataSize < 0 || reqExtDataSize > std::numeric_limits<int>::max())
#endif
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": send extended data: size error: size=", toString(reqExtDataSize)));
	}
	else if (reqExtData == NULL && reqExtDataSize > 0)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": send extended data: null point data"));
	}

	//Make Request Header & Data for ExtendedCommand
	HeaderCommand reqHeader;
	STRS_STRCPY(reqHeader.val.robotName, _robot.c_str());
	reqHeader.val.robotVersion[0] = '\0';
	reqHeader.val.stepInfo = _step;
	reqHeader.val.sof = 0x34;
	reqHeader.val.invokeId = ++_invokeCount;
	reqHeader.val.cmdId = CMD_FOR_EXTENDED;
	reqHeader.val.dataSize = sizeof(int)*2 /*8*/;
	int reqDataSize = reqHeader.val.dataSize;

	Data reqData;
	reqData.int2dArr[0] = extCmd;
	reqData.int2dArr[1] = reqExtDataSize;

	//Send Packet to Socket
	memcpy(writeBuff, reqHeader.byte, SIZE_DCP_HEADER_COMMAND);
	memcpy(writeBuff+SIZE_DCP_HEADER_COMMAND, reqData.byte, reqDataSize);
	_sendMessage(writeBuff, SIZE_DCP_HEADER_COMMAND+reqDataSize);

	//Send Extended Packet to Socket
	if (reqExtDataSize > 0)
		_sendMessage(reqExtData, reqExtDataSize);


	//Response Header, Data
	HeaderCommand resHeader;
	Data resData;
	int resDataSize;

	//Recv Header from Socket
	_recvMessage(readBuff, SIZE_DCP_HEADER_COMMAND);
	memcpy(resHeader.byte, readBuff, SIZE_DCP_HEADER_COMMAND);

	//Check Header
	if (strcmp(reqHeader.val.robotName, resHeader.val.robotName))
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: robot name error: name=", std::string(resHeader.val.robotName)));
	}
	else if (reqHeader.val.stepInfo != resHeader.val.stepInfo)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: step version error: version=", toString(resHeader.val.stepInfo)));
	}
	else if (reqHeader.val.invokeId != resHeader.val.invokeId)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: invoke id error: id=", toString(resHeader.val.invokeId)));
	}
	else if (resHeader.val.sof != 0x12)
	{
		_mtx.unlock();
		disconnect();
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": recv header: source of frame error: sof=", toString(resHeader.val.sof)));
	}
	else if (reqHeader.val.cmdId != resHeader.val.cmdId)
	{
		if (resHeader.val.cmdId == CMD_ERROR)
		{
			isError = true;
		}
		else
		{
			_mtx.unlock();
			disconnect();
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv header: different command id: cmd=", toString(resHeader.val.cmdId)));
		}
	}

	//Recv Data from Socket
	resDataSize = resHeader.val.dataSize;
	if (isError)
	{
		if (resDataSize != sizeof(int)/*4*/)
		{
			_mtx.unlock();
			disconnect();
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv data: cmd error data size error: size=", toString(resDataSize)));
		}
	}
	else
	{
		if (resDataSize != sizeof(int)*2 /*8*/)
		{
			_mtx.unlock();
			disconnect();
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv header: data size for extended cmd info error: size=", toString(resDataSize)));
		}
	}
	_recvMessage(readBuff, resDataSize);
	memcpy(resData.byte, readBuff, resDataSize);

	int errorCode = ERR_NONE;
	if (isError)
	{
		errorCode = resData.intVal;
	}
	else
	{
		//Recv Extended Data from Socket
		resExtDataSize = resData.int2dArr[1];
#if defined (WINDOWS)
		if (resExtDataSize < 0 || resExtDataSize > INT_MAX)
#else
		if (resExtDataSize < 0 || resExtDataSize > std::numeric_limits<int>::max())
#endif
		{
			_mtx.unlock();
			disconnect();
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv extended data: size error: size=", toString(resExtDataSize)));
		}
		else if (resData.int2dArr[0] != extCmd)
		{
			_mtx.unlock();
			disconnect();
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv header: different extended command id: extCmd=", toString(resData.int2dArr[0])));
		}
		else if (resExtData == NULL && resExtDataSize > 0)
		{
			_mtx.unlock();
			disconnect();
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv extended data: null point data buffer"));
		}

		if (resExtDataSize > 0)
			_recvMessage(resExtData, resExtDataSize);
	}

	_currTimeout = 0;
	_mtx.unlock();
	return errorCode;
}

bool IndyDCPConnector::_sendMessage(unsigned char const * buffer, size_t size)
{
	size_t curr = 0;

	while (curr < size)
	{
		if (_currTimeout > _limitTimeout)
		{
			_mtx.unlock();
			disconnect();
			throw TimeoutException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": send: timeout", toString(_currTimeout)));
		}

#if defined (WINDOWS)
		int sendLen = ::send(_sockFd, reinterpret_cast<const char *>(buffer)+curr, size - curr, 0);
		int err = WSAGetLastError();
#else
		int sendLen = ::send(_sockFd, buffer + curr, size - curr, MSG_DONTWAIT);
		int err = errno;
#endif

		if (sendLen == -1)
		{
			switch(err)
			{
#if defined (WINDOWS)
			case WSAEINVAL: case WSAEFAULT: case WSAENETRESET: case WSAECONNRESET: case WSAECONNABORTED:
#endif
			case EINVAL: case EBADF: case ECONNRESET: case ENXIO: case EPIPE:
			{
				_mtx.unlock();
				disconnect();
				throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": send: critical error: ", toErrString(err)));
			}
#if defined (WINDOWS)
			case WSA_IO_INCOMPLETE: case WSAENETUNREACH: case WSAENETDOWN:
#else
			case EDQUOT:
#endif
			case EFBIG: case EIO: case ENETDOWN: case ENETUNREACH: case ENOSPC:
            {
        		_mtx.unlock();
				disconnect();
            	throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
            			": send: resource failure: ", toErrString(err)));
            }
#if defined (WINDOWS)
			case WSAEINTR:
#endif
			case EINTR:	//interrupt...
			{
				uSleep(1e4);	//10ms
				_currTimeout += 0.01;
				continue;
			}
#if defined (WINDOWS)
			case WSATRY_AGAIN: case WSA_IO_PENDING: case WSAEWOULDBLOCK: case EWOULDBLOCK:
#endif
			case EAGAIN:	//temp error
			{
				uSleep(1e4);	//10ms
				_currTimeout += 0.01;
				continue;
			}
            default:
            {
        		_mtx.unlock();
				disconnect();
            	throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
            			": send: failed: ", toErrString(err)));
            }
			}
		}
		else if (sendLen == 0)
		{
			uSleep(1e4);	//10ms
			_currTimeout += 0.01;
			continue;
		}

		curr += sendLen;
		uSleep(5e3);	//5ms
		_currTimeout += 0.005;
	}
	return true;
}

bool IndyDCPConnector::_recvMessage(unsigned char * buffer, size_t size)
{
	size_t curr = 0;

	while (curr < size)
	{
		if (_currTimeout > _limitTimeout)
		{
			_mtx.unlock();
			disconnect();
			throw TimeoutException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv: timeout ", toString(_currTimeout)));
		}

#if defined (WINDOWS)
		int recvLen = ::recv(_sockFd, reinterpret_cast<char *>(buffer) + curr, size - curr, 0);
		int err = WSAGetLastError();
#else
		int recvLen = ::recv(_sockFd, buffer + curr, size - curr, MSG_DONTWAIT/*diff with MSG_WAITALL*/);
		int err = errno;
#endif

		if (recvLen == 0)
		{
			_mtx.unlock();
			shutdown();
			throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": recv: socket disconnected: ", toErrString(err)));
		}
		if (recvLen == -1)
		{
#if defined (WINDOWS)
			int err = WSAGetLastError();
#else
			int err = errno;
#endif
			switch(err)
			{
#if defined (WINDOWS)
			case WSAEINVAL: case WSAEFAULT: case WSAENETRESET: case WSAECONNRESET: case WSAECONNABORTED:
#endif
			case EINVAL: case EBADF: case ECONNRESET: case ENXIO: case EPIPE:
			{
				_mtx.unlock();
				disconnect();
				throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": recv: critical error: ", toErrString(err)));
			}
#if defined (WINDOWS)
			case WSA_IO_INCOMPLETE: case WSAENETUNREACH: case WSAENETDOWN:
#else
			case EDQUOT:
#endif
			case EFBIG: case EIO: case ENETDOWN: case ENETUNREACH: case ENOSPC:
			{
				_mtx.unlock();
				disconnect();
				throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": recv: resource failure: ", toErrString(err)));
			}
#if defined (WINDOWS)
			case WSAEINTR:
#endif
			case EINTR:	//interrupt...
			{
				uSleep(1e4);	//10ms
				_currTimeout += 0.01;
				continue;
			}
#if defined (WINDOWS)
			case WSATRY_AGAIN: case WSA_IO_PENDING: case WSAEWOULDBLOCK: case EWOULDBLOCK:
#endif
			case EAGAIN:	//temp error
			{
				uSleep(1e4);	//10ms
				_currTimeout += 0.01;
				continue;
			}
			default:	//else err code
			{
				_mtx.unlock();
				disconnect();
				throw ConnectionException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": recv: failed: ", toErrString(err)));
			}
			}
		}

		curr += recvLen;
		uSleep(5e3);	//5ms
		_currTimeout += 0.005;
	}
	return true;
}

IndyDCPConnector::IndyDCPConnector(const std::string & serverIP, const std::string & robotName, unsigned char stepVersion)
: _serverIP(serverIP)
, _robot(robotName)
, _dof(_checkRobotDOF(_robot))
, _axes(_checkRobotAxes(_robot))
, _step(stepVersion)
#if defined (WINDOWS)
, _sockFd(INVALID_SOCKET)
#else
, _sockFd(-1)
#endif
, _connected(false)
, _limitTimeout(DEFAULT_TIMEOUT)
, _invokeCount(0)
{
	if (_dof<=0 || _axes<=0) throw RobotDeclareException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
			": invalid robot type: ", _robot));
	if (_step!=0x02 && _step!=0x03) throw RobotDeclareException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
			": invalid STEP version: ", toString(_step)));
}

IndyDCPConnector::~IndyDCPConnector()
{
	disconnect();
}

int IndyDCPConnector::_checkRobotDOF(const std::string & _robot)
{
	if (!_robot.compare(ROBOT_INDYRP) || !_robot.compare(ROBOT_INDY7) || !_robot.compare(ROBOT_INDY15) ||
			!_robot.compare(ROBOT_INDY3) || !_robot.compare(ROBOT_INDY5) || !_robot.compare(ROBOT_INDY10) ||
			!_robot.compare(ROBOT_OPTI5) || !_robot.compare(ROBOT_OPTI10))
	{
		return 6;
	}
	else if (!_robot.compare(ROBOT_INDYRP2))
	{
		return 7;
	}
	else return -1;
}

int IndyDCPConnector::_checkRobotAxes(const std::string & _robot)
{
	return 6;
}

} /* namespace DCP */
} /* namespace Service */
} /* namespace NRMKIndy */
