/*
 * IndyDCPException.cpp
 *
 *  Created on: 2019. 1. 11.
 *      Author: Hanter Jung
 */

#include "IndyDCPException.h"

namespace NRMKIndy
{
namespace Service
{
namespace DCP
{

IndyDCPException::IndyDCPException(int code)
: _code(code)
{
}

IndyDCPException::IndyDCPException(const std::string & msg, int code)
: _msg(msg)
, _code(code)
{
}

IndyDCPException::~IndyDCPException()
{
}

const char* IndyDCPException::what() const throw()
{
	return _msg.c_str();
}


//
// Standard exception classes
//
INDYDCP_IMPLEMENT_EXCEPTION(DomainException, IndyDCPException)
INDYDCP_IMPLEMENT_EXCEPTION(RobotDeclareException, DomainException)

INDYDCP_IMPLEMENT_EXCEPTION(LogicException, IndyDCPException)

INDYDCP_IMPLEMENT_EXCEPTION(RunTimeException, IndyDCPException)
INDYDCP_IMPLEMENT_EXCEPTION(ConnectionException, RunTimeException)
INDYDCP_IMPLEMENT_EXCEPTION(TimeoutException, RunTimeException)

} /* namespace DCP */
} /* namespace Service */
} /* namespace NRMKIndy */
