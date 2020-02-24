/*
 * IndyDCPConnectorCommands.cpp
 *
 *  Created on: 2019. 1. 14.
 *      Author: Hanter Jung
 */

#include "IndyDCPConnector.h"
#include <stdint.h>

namespace NRMKIndy
{
namespace Service
{
namespace DCP
{

// Macros for quickly implementing executing command with no req/res data.
#ifndef INDY_DCP_EXEC_CMD_NODATA
#define INDY_DCP_EXEC_CMD_NODATA(CMD_ID) 		\
	Data resData;																						\
	int resSize;																						\
																										\
	int result = _handleCommand(CMD_ID, resSize, resData);												\
	if (result == ERR_NONE && resSize != 0)																\
	{																									\
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,		\
				": response data size error : size=", toString(resSize)));								\
	}																									\
	return result;
#endif	//INDY_DCP_EXEC_CMD_NODATA

// Macros for quickly implementing executing command with one variable request data.
#ifndef INDY_DCP_EXEC_CMD_REQ_VAR
#define INDY_DCP_EXEC_CMD_REQ_VAR(CMD_ID, TYPE, VAR) 		\
	Data reqData, resData;																				\
	int reqSize, resSize;																				\
																										\
	reqSize = sizeof(TYPE);																				\
	memcpy(reqData.byte, &VAR, reqSize);																\
																										\
	int result = _handleCommand(CMD_ID, resSize, resData, reqSize, reqData);							\
	if (result == ERR_NONE && resSize != 0)																\
	{																									\
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,		\
				": response data size error : size=", toString(resSize)));								\
	}																									\
	return result;
#endif	//INDY_DCP_EXEC_CMD_REQ_ARR

// Macros for quickly implementing executing command with one variable response data.
#ifndef INDY_DCP_EXEC_CMD_RES_VAR
#define INDY_DCP_EXEC_CMD_RES_VAR(CMD_ID, TYPE, VAR) 		\
	Data resData;																						\
	int resSize;																						\
																										\
	int result = _handleCommand(CMD_ID, resSize, resData);												\
	if (result == ERR_NONE)																				\
	{																									\
		if ((size_t)resSize != sizeof(TYPE))															\
		{																								\
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,	\
					": response data size error : size=", toString(resSize)));							\
		}																								\
		memcpy(&VAR, resData.byte, resSize);															\
	}																									\
	return result;
#endif	//INDY_DCP_EXEC_CMD_RES_ARR

// Macros for quickly implementing executing command with one variable request and response data.
#ifndef INDY_DCP_EXEC_CMD_REQ_RES_VAR
#define INDY_DCP_EXEC_CMD_REQ_RES_VAR(CMD_ID, REQ_TYPE, REQ_VAR, RES_TYPE, RES_VAR) 		\
	Data reqData, resData;																				\
	int reqSize, resSize;																				\
																										\
	reqSize = sizeof(REQ_TYPE);																			\
	memcpy(reqData.byte, &REQ_VAR, reqSize);															\
																										\
	int result = _handleCommand(CMD_ID, resSize, resData, reqSize, reqData);							\
	if (result == ERR_NONE)																				\
	{																									\
		if ((size_t)resSize != sizeof(RES_TYPE))														\
		{																								\
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,	\
					": response data size error : size=", toString(resSize)));							\
		}																								\
		memcpy(&RES_VAR, resData.byte, resSize);														\
	}																									\
	return result;
#endif	//INDY_DCP_EXEC_CMD_REQ_RES_VAR

// Macros for quickly implementing executing command with array-type request data.
#ifndef INDY_DCP_EXEC_CMD_REQ_ARR
#define INDY_DCP_EXEC_CMD_REQ_ARR(CMD_ID, ARR_TYPE, ARR_VAR, ARR_LEN) 		\
	Data reqData, resData;																				\
	int reqSize, resSize;																				\
																										\
	reqSize = sizeof(ARR_TYPE)*ARR_LEN;																	\
	memcpy(reqData.byte, ARR_VAR, reqSize);																\
																										\
	int result = _handleCommand(CMD_ID, resSize, resData, reqSize, reqData);							\
	if (result == ERR_NONE && resSize != 0)																\
	{																									\
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,		\
				": response data size error : size=", toString(resSize)));								\
	}																									\
	return result;
#define INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_ID, ARR_TYPE, ARR_VAR) 										\
		INDY_DCP_EXEC_CMD_REQ_ARR(CMD_ID, ARR_TYPE, ARR_VAR, _dof)
#endif	//INDY_DCP_EXEC_CMD_REQ_ARR

// Macros for quickly implementing executing command with initializer-list-type request data.
#ifndef INDY_DCP_EXEC_CMD_REQ_INITLIST
#define INDY_DCP_EXEC_CMD_REQ_INITLIST(FUNC_ORIGIN, LIST_TYPE, LIST_VAR, LIST_LEN) 		\
	if (LIST_VAR.size() != (size_t)LIST_LEN)															\
	{																									\
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,		\
				": list size error : size=", toString(LIST_VAR.size())));								\
	}																									\
																										\
	LIST_TYPE * arr = new LIST_TYPE[LIST_LEN];															\
	int k=0;																							\
	for (auto item : LIST_VAR)																			\
		arr[k++] = item;																				\
	int code = FUNC_ORIGIN(arr);																		\
	delete[] arr;																						\
	return code;
#define INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(FUNC_ORIGIN, LIST_TYPE, LIST_VAR) 							\
		INDY_DCP_EXEC_CMD_REQ_INITLIST(FUNC_ORIGIN, LIST_TYPE, LIST_VAR, _dof)
#endif	//INDY_DCP_EXEC_CMD_REQ_INITLIST

// Macros for quickly implementing executing command with array-type response data.
#ifndef INDY_DCP_EXEC_CMD_RES_ARR
#define INDY_DCP_EXEC_CMD_RES_ARR(CMD_ID, ARR_TYPE, ARR_VAR, ARR_LEN) 		\
	Data resData;																						\
	int resSize;																						\
																										\
	int result = _handleCommand(CMD_ID, resSize, resData);												\
	if (result == ERR_NONE)																				\
	{																									\
		if ((size_t)resSize != sizeof(ARR_TYPE)*ARR_LEN)												\
		{																								\
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,	\
					": response data size error : size=", toString(resSize)));							\
		}																								\
		if (ARR_VAR == NULL)																			\
		{																								\
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,	\
					": null pointer return parameter"));												\
		}																								\
		memcpy(ARR_VAR, resData.byte, resSize);															\
	}																									\
	return result;
#define INDY_DCP_EXEC_CMD_RES_ARR_DOF(CMD_ID, ARR_TYPE, ARR_VAR) 										\
		INDY_DCP_EXEC_CMD_RES_ARR(CMD_ID, ARR_TYPE, ARR_VAR, _dof)
#endif	//INDY_DCP_EXEC_CMD_RES_ARR

// Macros for quickly implementing executing command with array-type response data.
#ifndef INDY_DCP_CHECK_LEVEL
#define INDY_DCP_CHECK_LEVEL(LEVEL, MIN, MAX)		\
	if (LEVEL < MIN)																					\
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,		\
					": invalid level : level=", toString(LEVEL)));										\
	else if (LEVEL > MAX)																				\
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,		\
					": invalid level : level=", toString(LEVEL)));
#endif	//INDY_DCP_CHECK_LEVEL

int IndyDCPConnector::check()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_CHECK);
}

int IndyDCPConnector::stopEmergency()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_EMERGENCY_STOP);
}

int IndyDCPConnector::resetRobot()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_RESET_ROBOT);
}

int IndyDCPConnector::setServoOnOff(const char * const servoVec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_SET_SERVO, char, servoVec);
}

int IndyDCPConnector::setServoOnOff(const bool * const servoVec)
{
	char * servoVec2 = new char[_dof];
	for (int i = 0; i < _dof; i++)
		servoVec2[i] = servoVec[i];
	int code = setServoOnOff(servoVec2);
	delete servoVec2;
	return code;
}

int IndyDCPConnector::setServoOnOff(std::initializer_list<char> servoVec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(setServoOnOff, char, servoVec);
}

int IndyDCPConnector::setBrakeOnOff(const char * const brakeVec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_SET_BRAKE, char, brakeVec);
}

int IndyDCPConnector::setBrakeOnOff(const bool * const brakeVec)
{
	char * brakeVec2 = new char[_dof];
	for (int i = 0; i < _dof; i++)
		brakeVec2[i] = brakeVec[i];
	int code = setBrakeOnOff(brakeVec2);
	delete brakeVec2;
	return code;
}

int IndyDCPConnector::setBrakeOnOff(std::initializer_list<char> brakeVec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(setBrakeOnOff, char, brakeVec);
}

int IndyDCPConnector::stopMotion()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_STOP);
}

int IndyDCPConnector::executeMoveCommand(const std::string & cmdName)
{
	Data reqData, resData;
	int reqSize, resSize;

	if (cmdName.size() > SIZE_DCP_DATA_TCP_MAX)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": too long cmd name : len=", toString(cmdName.size())));
	}

	reqSize = cmdName.size();
	memcpy(reqData.str, cmdName.c_str(), reqSize);

	int result = _handleCommand(CMD_MOVE, resSize, resData, reqSize, reqData);
	if (result == ERR_NONE && resSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resSize)));
	}
	return result;
}

int IndyDCPConnector::moveJointHome()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_MOVE_HOME);
}

int IndyDCPConnector::moveJointZero()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_MOVE_ZERO);
}

int IndyDCPConnector::moveJointTo(const double * const qvec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_JOINT_MOVE_TO, double, qvec);
}

int IndyDCPConnector::moveJointTo(std::initializer_list<double> qvec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(moveJointTo, double, qvec);
}

int IndyDCPConnector::moveJointBy(const double * const qvec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_JOINT_MOVE_BY, double, qvec);
}

int IndyDCPConnector::moveJointBy(std::initializer_list<double> qvec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(moveJointBy, double, qvec);
}

int IndyDCPConnector::moveTaskTo(const double * const pvec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_TASK_MOVE_TO, double, pvec, 6);
}

int IndyDCPConnector::moveTaskTo(std::initializer_list<double> pvec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST(moveTaskTo, double, pvec, 6);
}

int IndyDCPConnector::moveTaskBy(const double * const pvec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_TASK_MOVE_BY, double, pvec, 6);
}

int IndyDCPConnector::moveTaskBy(std::initializer_list<double> pvec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST(moveTaskBy, double, pvec, 6);
}

int IndyDCPConnector::startCurrProgram()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_START_CURRENT_PROGRAM);
}

int IndyDCPConnector::pauseProgram()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_PAUSE_CURRENT_PROGRAM);
}

int IndyDCPConnector::resumeProgram()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_RESUME_CURRENT_PROGRAM);
}

int IndyDCPConnector::stopProgram()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_STOP_CURRENT_PROGRAM);
}

int IndyDCPConnector::startRegisteredDefaultProgram()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_START_DEFAULT_PROGRAM);
}

int IndyDCPConnector::registerDefaultProgram(int idx)
{
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_REGISTER_DEFAULT_PROGRAM_IDX, int, idx);
}

int IndyDCPConnector::getRegisteredDefaultProgram(int & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_REGISTERED_DEFAULT_PROGRAM_IDX, int, ret);
}

int IndyDCPConnector::isRobotRunning(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_ROBOT_RUNNING, bool, ret);
}

int IndyDCPConnector::isRobotReady(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_READY, bool, ret);
}

int IndyDCPConnector::isEmergencyStopped(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_EMG, bool, ret);
}

int IndyDCPConnector::isCollided(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_COLLIDED, bool, ret);
}

int IndyDCPConnector::isErrorState(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_ERR, bool, ret);
}

int IndyDCPConnector::isBusy(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_BUSY, bool, ret);
}

int IndyDCPConnector::isMoveFinished(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_MOVE_FINISEHD, bool, ret);
}

int IndyDCPConnector::isHome(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_HOME, bool, ret);
}

int IndyDCPConnector::isZero(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_ZERO, bool, ret);
}

int IndyDCPConnector::isInResetting(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_IN_RESETTING, bool, ret);
}

int IndyDCPConnector::isDirectTeachingMode(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_DIRECT_TECAHING, bool, ret);
}

int IndyDCPConnector::isTeachingMode(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_TEACHING, bool, ret);
}

int IndyDCPConnector::isProgramRunning(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_PROGRAM_RUNNING, bool, ret);
}

int IndyDCPConnector::isProgramPaused(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_PROGRAM_PAUSED, bool, ret);
}

int IndyDCPConnector::isContyConnected(bool & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_IS_CONTY_CONNECTED, bool, ret);
}

int IndyDCPConnector::changeToDirectTeachingMode()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_CHANGE_DIRECT_TEACHING);
}

int IndyDCPConnector::finishDirectTeachingMode()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_FINISH_DIRECT_TEACHING);
}

int IndyDCPConnector::addJointWaypointSet(const double * const qvec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_JOINT_PUSH_BACK_WAYPOINT_SET, double, qvec);
}

int IndyDCPConnector::addJointWaypointSet(std::initializer_list<double> qvec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(addJointWaypointSet, double, qvec);
}

int IndyDCPConnector::removeJointWaypointSet()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_JOINT_POP_BACK_WAYPOINT_SET);
}

int IndyDCPConnector::clearJointWaypointSet()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_JOINT_CLEAR_WAYPOINT_SET);
}

int IndyDCPConnector::executeJointWaypointSet()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_JOINT_EXECUTE_WAYPOINT_SET);
}

int IndyDCPConnector::addTaskWaypointSet(const double * const pvec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR_DOF(CMD_TASK_PUSH_BACK_WAYPOINT_SET, double, pvec);
}

int IndyDCPConnector::addTaskWaypointSet(std::initializer_list<double> pvec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST_DOF(addTaskWaypointSet, double, pvec);
}

int IndyDCPConnector::removeTaskWaypointSet()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_TASK_POP_BACK_WAYPOINT_SET);
}

int IndyDCPConnector::clearTaskWaypointSet()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_TASK_CLEAR_WAYPOINT_SET);
}

int IndyDCPConnector::executeTaskWaypointSet()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_TASK_EXECUTE_WAYPOINT_SET);
}

int IndyDCPConnector::setDefaultTCP(const double * const tcpVec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_SET_DEFAULT_TCP, double, tcpVec, 6);
}

int IndyDCPConnector::setDefaultTCP(std::initializer_list<double> tcpVec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST(setDefaultTCP, double, tcpVec, 6);
}

int IndyDCPConnector::resetDefaultTCP()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_RESET_DEFAULT_TCP);
}

int IndyDCPConnector::setTCPCompensation(const double * const tcpVec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_SET_COMP_TCP, double, tcpVec, 6);
}

int IndyDCPConnector::setTCPCompensation(std::initializer_list<double> tcpVec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST(setTCPCompensation, double, tcpVec, 6);
}

int IndyDCPConnector::resetTCPCompensation()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_RESET_COMP_TCP);
}

int IndyDCPConnector::setReferenceFrame(const double * const refFrameVec)
{
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_SET_REFFRAME, double, refFrameVec, 6);
}

int IndyDCPConnector::setReferenceFrame(std::initializer_list<double> refFrameVec)
{
	INDY_DCP_EXEC_CMD_REQ_INITLIST(setReferenceFrame, double, refFrameVec, 6);
}

int IndyDCPConnector::resetReferenceFrame()
{
	INDY_DCP_EXEC_CMD_NODATA(CMD_RESET_REFFRAME);
}

int IndyDCPConnector::setCollisionDetectionLevel(int level)
{
	INDY_DCP_CHECK_LEVEL(level, 1, 9);
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_COLLISION_LEVEL, int, level);
}

int IndyDCPConnector::setJointBoundaryLevel(int level)
{
	INDY_DCP_CHECK_LEVEL(level, 1, 9);
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_JOINT_BOUNDARY, int, level);
}

int IndyDCPConnector::setTaskBoundaryLevel(int level)
{
	INDY_DCP_CHECK_LEVEL(level, 1, 9);
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_TASK_BOUNDARY, int, level);
}

int IndyDCPConnector::setJointBlendingRadius(double radius)
{
	if (radius < 0.0)
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid radius : radius=", toString(radius)));
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_JOINT_BLEND_RADIUS, double, radius);
}

int IndyDCPConnector::setTaskBlendingRadius(double radius)
{
	if (radius < 0.0)
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid radius : radius=", toString(radius)));
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_TASK_BLEND_RADIUS, double, radius);
}

int IndyDCPConnector::setJointWaypointTime(double time)
{
	if (time <= 0.0)
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid time : time=", toString(time)));
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_JOINT_WTIME, double, time);
}

int IndyDCPConnector::setTaskWaypointTime(double time)
{
	if (time <= 0.0)
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid time : time=", toString(time)));
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_TASK_WTIME, double, time);
}

int IndyDCPConnector::setTaskBaseMode(int mode)
{
	if (mode != 0 && mode != 1)
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid tbase mode : mode=", toString(mode)));
	INDY_DCP_EXEC_CMD_REQ_VAR(CMD_SET_TASK_CMODE, int, mode);
}

int IndyDCPConnector::getDefaultTCP(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_DEFAULT_TCP, double, ret, 6);
}

int IndyDCPConnector::getTCPCompensation(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_COMP_TCP, double, ret, 6);
}

int IndyDCPConnector::getReferenceFrame(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_REFFRAME, double, ret, 6);
}

int IndyDCPConnector::getCollisionDetectionLevel(int & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_COLLISION_LEVEL, int, ret);
}

int IndyDCPConnector::getJointBoundaryLevel(int & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_JOINT_BOUNDARY, int, ret);
}

int IndyDCPConnector::getTaskBoundaryLevel(int & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_TASK_BOUNDARY, int, ret);
}

int IndyDCPConnector::getJointBlendingRadius(double & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_JOINT_BLEND_RADIUS, double, ret);
}

int IndyDCPConnector::getTaskBlendingRadius(double & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_TASK_BLEND_RADIUS, double, ret);
}

int IndyDCPConnector::getJointWaypointTime(double & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_JOINT_WTIME, double, ret);
}

int IndyDCPConnector::getTaskWaypointTime(double & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_TASK_WTIME, double, ret);
}

int IndyDCPConnector::getTaskBaseMode(int & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_TASK_CMODE, int, ret);
}

int IndyDCPConnector::getRobotRunningTime(double & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_RUNNING_TIME, double, ret);
}
int IndyDCPConnector::getControlMode(int & ret)
{
	INDY_DCP_EXEC_CMD_RES_VAR(CMD_GET_CMODE, int, ret);
}

int IndyDCPConnector::getJointServoState(char * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_JOINT_STATE, char, ret, _dof*2);
}

int IndyDCPConnector::getJointServoState(char * const retServo, char * const retBrake)
{
	char * ret = new char[_dof*2];
	int code = getJointServoState(ret);
	if (code == ERR_NONE)
	{
		for (int k=0; k<_dof; k++)
		{
			retServo[k] = ret[k];
			retBrake[k] = ret[_dof+k];
		}
	}
	delete[] ret;
	return code;
}

int IndyDCPConnector::getJointServoState(bool * const retServo, bool * const retBrake)
{
	char * ret = new char[_dof * 2];
	int code = getJointServoState(ret);
	if (code == ERR_NONE)
	{
		for (int k=0; k<_dof; k++)
		{
			retServo[k] = ret[k];
			retBrake[k] = ret[_dof+k];
		}
	}
	delete[] ret;
	return code;
}

int IndyDCPConnector::getJointPosition(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR_DOF(CMD_GET_JOINT_POSITION, double, ret);
}

int IndyDCPConnector::getJointVelocity(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR_DOF(CMD_GET_JOINT_VELOCITY, double, ret);
}

int IndyDCPConnector::getTaskPosition(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_TASK_POSITION, double, ret, 6);
}

int IndyDCPConnector::getTaskVelocity(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_TASK_VELOCITY, double, ret, 6);
}

int IndyDCPConnector::getTorque(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR_DOF(CMD_GET_TORQUE, double, ret);
}

int IndyDCPConnector::getLastEmergencyInfo(int & retCode, int * const retIntArgs, double * const retDoubleArgs)
{
	Data resData;
	int resSize;

	int result = _handleCommand(CMD_GET_LAST_EMG_INFO, resSize, resData);
	if (result == ERR_NONE)
	{
		if ((size_t)resSize != sizeof(int)*4 + sizeof(double)*3 /*4*4+8*3=16+24=40*/)
		{
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": response data size error : size=", toString(resSize)));
		}
		if (retIntArgs == NULL || retDoubleArgs == NULL)
		{
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": null pointer return parameter"));
		}

		memcpy(&retCode, resData.byte, sizeof(int));
		memcpy(retIntArgs, resData.byte+sizeof(int), sizeof(int)*3);
		memcpy(retDoubleArgs, resData.byte+(sizeof(int)*4), sizeof(double)*3);
	}
	return result;
}

int IndyDCPConnector::getSmartDigitalInput(int idx, char & ret)
{
	INDY_DCP_EXEC_CMD_REQ_RES_VAR(CMD_GET_SMART_DI, int, idx, char, ret);
}

int IndyDCPConnector::getSmartDigitalInputs(char * const & ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_SMART_DIS, char, ret, 32);
}

int IndyDCPConnector::setSmartDigitalOutput(int idx, char val)
{
	Data reqData, resData;
	int reqSize, resSize;

	reqSize = sizeof(int) + sizeof(char); /*5*/
	memcpy(reqData.byte, &idx, sizeof(int));
	memcpy(reqData.byte+sizeof(int), &val, sizeof(char));
//	*(int*)(reqData.byte) = idx;
//	*(char*)(reqData.byte+sizeof(int)) = val;

	int result = _handleCommand(CMD_SET_SMART_DO, resSize, resData, reqSize, reqData);
	if (result == ERR_NONE && resSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resSize)));
	}
	return result;
}

int IndyDCPConnector::setSmartDigitalOutputs(const char * const val)
{
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_SET_SMART_DOS, char, val, 32);
}

int IndyDCPConnector::getSmartAnalogInput(int idx, int & ret)
{
	INDY_DCP_EXEC_CMD_REQ_RES_VAR(CMD_GET_SMART_AI, int, idx, int, ret);
}

int IndyDCPConnector::setSmartAnalogOutput(int idx, int val)
{
	int intArr[2] = {idx, val};
	INDY_DCP_EXEC_CMD_REQ_ARR(CMD_SET_SMART_AO, int, intArr, 2);
}

int IndyDCPConnector::getSmartDigitalOutput(int idx, char & ret)
{
	INDY_DCP_EXEC_CMD_REQ_RES_VAR(CMD_GET_SMART_DO, int, idx, char, ret);
}

int IndyDCPConnector::getSmartDigitalOutputs(char * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_SMART_DOS, char, ret, 32);
}

int IndyDCPConnector::getSmartAnalogOutput(int idx, int & ret)
{
	INDY_DCP_EXEC_CMD_REQ_RES_VAR(CMD_GET_SMART_AO, int, idx, int, ret);
}

int IndyDCPConnector::getRobotCanFTSensorRaw(int * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_EXTIO_FTCAN_ROBOT_RAW, int, ret, 6);
}

int IndyDCPConnector::getRobotCanFTSensorTransformed(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_EXTIO_FTCAN_ROBOT_TRANS, double, ret, 6);
}

int IndyDCPConnector::getCBCanFTSensorRaw(int * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_EXTIO_FTCAN_CB_RAW, int, ret, 6);
}

int IndyDCPConnector::getCBCanFTSensorTransformed(double * const ret)
{
	INDY_DCP_EXEC_CMD_RES_ARR(CMD_GET_EXTIO_FTCAN_CB_TRANS, double, ret, 6);
}

int IndyDCPConnector::readDirectVariable(int type, int addr, void * ret)
{
	Data reqData, resData;
	int reqSize, resSize;

	if (addr < 0 || addr > 999) {
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid addr : addr=", toString(addr)));
	}

	reqData.int2dArr[0] = type;
	reqData.int2dArr[1] = addr;
	reqSize = sizeof(int)*2;

	int result = _handleCommand(CMD_READ_DIRECT_VARIABLE, resSize, resData, reqSize, reqData);
	if (result == ERR_NONE)
	{
		switch(type)
		{
		case DirectVariableType::DIRECT_VAR_TYPE_BYTE:
			if (resSize != sizeof(uint8_t))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(uint8_t*)ret = resData.byteVal;
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_WORD:
			if (resSize != sizeof(int16_t))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(int16_t*)ret = resData.wordVal;
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_DWORD:
			if (resSize != sizeof(int32_t))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(int32_t*)ret = resData.dwordVal;
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_LWORD:
			if (resSize != sizeof(int64_t))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(int64_t*)ret = resData.lwordVal;
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_FLOAT:
			if (resSize != sizeof(float))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(float*)ret = resData.floatVal;
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_DFLOAT:
			if (resSize != sizeof(double))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(double*)ret = resData.doubleVal;
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_MODBUS_REG:
			if (resSize != sizeof(uint16_t))
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			*(uint16_t*)ret = resData.uwordVal;
			break;

		default:
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,	\
					": invalid type : type=", toString(type)));
		}
	}
	return result;
}

int IndyDCPConnector::readDirectVariables(int type, int addr, int len, void * ret)
{
	Data reqData, resData;
	int reqSize, resSize;

	if (addr < 0 || addr > 999) {
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid addr : addr=", toString(addr)));
	}
	else if (len < 1 || len > 20)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid len : len=", toString(len)));
	}
	else if (addr + len > 1000)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid range : addr=", toString(addr), ", len=", toString(len)));
	}

	reqData.int3dArr[0] = type;
	reqData.int3dArr[1] = addr;
	reqData.int3dArr[2] = len;
	reqSize = sizeof(int)*3;

	int result = _handleCommand(CMD_READ_DIRECT_VARIABLES, resSize, resData, reqSize, reqData);
	if (result == ERR_NONE)
	{
		switch(type)
		{
		case DirectVariableType::DIRECT_VAR_TYPE_BYTE:
			if (resSize != sizeof(uint8_t)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((uint8_t*)ret)[k] = resData.byteArr[k];
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_WORD:
			if (resSize != sizeof(int16_t)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((int16_t*)ret)[k] = resData.wordArr[k];
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_DWORD:
			if (resSize != sizeof(int32_t)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((int32_t*)ret)[k] = resData.dwordArr[k];
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_LWORD:
			if (resSize != sizeof(int64_t)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((int64_t*)ret)[k] = resData.lwordArr[k];
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_FLOAT:
			if (resSize != sizeof(float)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((float*)ret)[k] = resData.floatArr[k];
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_DFLOAT:
			if (resSize != sizeof(double)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((double*)ret)[k] = resData.doubleArr[k];
			break;

		case DirectVariableType::DIRECT_VAR_TYPE_MODBUS_REG:
			if (resSize != sizeof(uint16_t)*len)
				throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
						": response data size error : size=", toString(resSize)));
			for (int k=0; k<len; k++)
				((uint16_t*)ret)[k] = resData.uwordArr[k];
			break;

		default:
			throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": invalid type : type=", toString(type)));
		}
	}
	return result;
}

int IndyDCPConnector::writeDirectVariable(int type, int addr, void * ptrVal)
{
	Data reqData, resData;
	int reqSize, resSize;

	if (addr < 0 || addr > 999) {
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid addr : addr=", toString(addr)));
	}

	reqData.int2dArr[0] = type;
	reqData.int2dArr[1] = addr;
	char * ptrCurrData = (char *)reqData.charArr + (sizeof(int)*2);

	int varSize = 0;
	switch(type)
	{
	case DirectVariableType::DIRECT_VAR_TYPE_BYTE:
		varSize = sizeof(uint8_t);
		*(uint8_t *)ptrCurrData = *(uint8_t const *)ptrVal;
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_WORD:
		varSize = sizeof(int16_t);
		*(int16_t *)ptrCurrData = *(int16_t const *)ptrVal;
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_DWORD:
		varSize = sizeof(int32_t);
		*(int32_t *)ptrCurrData = *(int32_t const *)ptrVal;
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_LWORD:
		varSize = sizeof(int64_t);
		*(int64_t *)ptrCurrData = *(int64_t const *)ptrVal;
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_FLOAT:
		varSize = sizeof(float);
		*(float *)ptrCurrData = *(float const *)ptrVal;
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_DFLOAT:
		varSize = sizeof(double);
		*(double *)ptrCurrData = *(double const *)ptrVal;
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_MODBUS_REG:
		varSize = sizeof(uint16_t);
		*(uint16_t *)ptrCurrData = *(uint16_t const *)ptrVal;
		break;

	default:
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid type : type=", toString(type)));
	}

	reqSize = sizeof(int)*2 + varSize;

	int result = _handleCommand(CMD_WRITE_DIRECT_VARIABLE, resSize, resData, reqSize, reqData);
	if (result == ERR_NONE)
	{
		if (resSize != 0)
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": response data size error : size=", toString(resSize)));
	}
	return result;
}

int IndyDCPConnector::writeDirectVariables(int type, int addr, int len, void * ptrVal)
{
	Data reqData, resData;
	int reqSize, resSize;

	if (addr < 0 || addr > 999) {
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid addr : addr=", toString(addr)));
	}
	else if (len < 1 || len > 20)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid len : len=", toString(len)));
	}
	else if (addr + len > 1000)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid range : addr=", toString(addr), ", len=", toString(len)));
	}

	reqData.int3dArr[0] = type;
	reqData.int3dArr[1] = addr;
	reqData.int3dArr[2] = len;
	char * ptrCurrData = (char*)reqData.charArr + (sizeof(int)*3);

	int varSize = 0;
	switch(type)
	{
	case DirectVariableType::DIRECT_VAR_TYPE_BYTE:
		varSize = sizeof(uint8_t);
		for (int k=0; k<len; k++)
			((uint8_t *)ptrCurrData)[k] = ((uint8_t const *)ptrVal)[k];
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_WORD:
		varSize = sizeof(int16_t);
		for (int k=0; k<len; k++)
			((int16_t *)ptrCurrData)[k] = ((int16_t const *)ptrVal)[k];
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_DWORD:
		varSize = sizeof(int32_t);
		for (int k=0; k<len; k++)
			((int32_t *)ptrCurrData)[k] = ((int32_t const *)ptrVal)[k];
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_LWORD:
		varSize = sizeof(int64_t);
		for (int k=0; k<len; k++)
			((int64_t *)ptrCurrData)[k] = ((int64_t const *)ptrVal)[k];
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_FLOAT:
		varSize = sizeof(float);
		for (int k=0; k<len; k++)
			((float *)ptrCurrData)[k] = ((float const *)ptrVal)[k];
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_DFLOAT:
		varSize = sizeof(double);
		for (int k=0; k<len; k++)
			((double *)ptrCurrData)[k] = ((double const *)ptrVal)[k];
		break;

	case DirectVariableType::DIRECT_VAR_TYPE_MODBUS_REG:
		varSize = sizeof(uint16_t);
		for (int k=0; k<len; k++)
			((uint16_t *)ptrCurrData)[k] = ((uint16_t const *)ptrVal)[k];
		break;

	default:
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": invalid type : type=", toString(type)));
	}

	reqSize = sizeof(int)*3 + varSize*len;

	int result = _handleCommand(CMD_WRITE_DIRECT_VARIABLES, resSize, resData, reqSize, reqData);
	if (result == ERR_NONE)
	{
		if (resSize != 0)
			throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
					": response data size error : size=", toString(resSize)));
	}
	return result;
}



int IndyDCPConnector::moveExtTrajBinaryData(const unsigned char * const data, int dataSize)
{
	unsigned char * resExtData = NULL;
	int resExtDataSize;

	int result = _handleExtCommand(EXT_CMD_MOVE_TRAJ_BY_DATA, resExtDataSize, resExtData, dataSize, data);
	if (result == ERR_NONE && resExtDataSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resExtDataSize)));
	}
	return result;
}

int IndyDCPConnector::moveExtTrajTextData(const unsigned char * const data, int dataSize)
{
	unsigned char * resExtData = NULL;
	int resExtDataSize;

	int result = _handleExtCommand(EXT_CMD_MOVE_TRAJ_BY_TXT_DATA, resExtDataSize, resExtData, dataSize, data);
	if (result == ERR_NONE && resExtDataSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resExtDataSize)));
	}
	return result;
}

int IndyDCPConnector::moveExtTrajBinaryFile(const std::string & fileName)
{
	unsigned char *reqExtData = NULL, *resExtData = NULL;
	int reqExtDataSize, resExtDataSize;

	reqExtData = new unsigned char[fileName.size()+1];
	memcpy(reqExtData, fileName.c_str(), fileName.size());
	reqExtData[fileName.size()] = '\0';
	reqExtDataSize = fileName.size()+1;

	int result = _handleExtCommand(EXT_CMD_MOVE_TRAJ_BY_FILE, resExtDataSize, resExtData, reqExtDataSize, reqExtData);
	if (result == ERR_NONE && resExtDataSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resExtDataSize)));
	}
	return result;
}

int IndyDCPConnector::moveExtTrajTextFile(const std::string & fileName)
{
	unsigned char *reqExtData = NULL, *resExtData = NULL;
	int reqExtDataSize, resExtDataSize;

	reqExtData = new unsigned char[fileName.size()+1];
	memcpy(reqExtData, fileName.c_str(), fileName.size());
	reqExtData[fileName.size()] = '\0';
	reqExtDataSize = fileName.size()+1;

	int result = _handleExtCommand(EXT_CMD_MOVE_TRAJ_BY_TXT_FILE, resExtDataSize, resExtData, reqExtDataSize, reqExtData);
	if (result == ERR_NONE && resExtDataSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resExtDataSize)));
	}
	return result;
}

int IndyDCPConnector::moveJointWaypointSet(const double * wpSet, int wpSetLen)
{
	unsigned char * resExtData = NULL;
	int resExtDataSize;

	const unsigned char * reqExtData = reinterpret_cast<const unsigned char *>(wpSet);
	int reqExtDataSize = wpSetLen * _dof * sizeof(double);

	int result = _handleExtCommand(EXT_CMD_JMOVE_ABS_WAYPOINT_SET, resExtDataSize, resExtData, reqExtDataSize, reqExtData);
	if (result == ERR_NONE && resExtDataSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resExtDataSize)));
	}
	return result;
}

int IndyDCPConnector::moveJointWaypointSet(std::initializer_list<double> wpQvec)
{
	if (wpQvec.size() % _dof != 0)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": qvec list size is not multiple of DOF : size=", toString(wpQvec.size())));
	}

	int wpSetLen = wpQvec.size() / _dof;
	double * wpSet = new double[wpQvec.size()];
	int k=0;
	for (auto q : wpQvec)
	{
		wpSet[k++] = q;
	}

	int ret = moveJointWaypointSet(wpSet, wpSetLen);
	delete [] wpSet;
	return ret;
}

int IndyDCPConnector::moveTaskWaypointSet(const double * wpSet, int wpSetLen)
{
	unsigned char * resExtData = NULL;
	int resExtDataSize;

	const unsigned char * reqExtData = reinterpret_cast<const unsigned char *>(wpSet);
	int reqExtDataSize = wpSetLen * 6 * sizeof(double);

	int result = _handleExtCommand(EXT_CMD_TMOVE_ABS_WAYPOINT_SET, resExtDataSize, resExtData, reqExtDataSize, reqExtData);
	if (result == ERR_NONE && resExtDataSize != 0)
	{
		throw DomainException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": response data size error : size=", toString(resExtDataSize)));
	}
	return result;

}

int IndyDCPConnector::moveTaskWaypointSet(std::initializer_list<double> wpPvec)
{
	if (wpPvec.size() % 6 != 0)
	{
		throw LogicException(IndyDCPException::buildErrorMessage("IndyDCPConnector::", __func__,
				": qvec list size is not multiple of 6 : size=", toString(wpPvec.size())));
	}

	int wpSetLen = wpPvec.size() / 6;
	double * wpSet = new double[wpPvec.size()];
	int k=0;
	for (auto p : wpPvec)
	{
		wpSet[k++] = p;
	}

	int ret = moveTaskWaypointSet(wpSet, wpSetLen);
	delete [] wpSet;
	return ret;
}


} /* namespace DCP */
} /* namespace Service */
} /* namespace NRMKIndy */
