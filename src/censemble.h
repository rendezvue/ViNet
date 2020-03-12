#ifndef CENSEMBLE_H
#define CENSEMBLE_H

#include "EnsembleAPI.h"

#include <qdebug.h>

class CEnsemble
{
public:
	// 싱글턴 인스턴스를 반환할 멤버함수
	static CEnsemble* getInstance();

	// 소멸자, instanceFlag를 false로 변경
 	virtual ~CEnsemble() ;
private:

	/**
	 * 생성자
	 * 외부에서 인스턴스를 생성할 수 없도록 private 영역에 정의하고 구현함
	 * 원리: private 영역에 생성자가 존재하므로 외부에서 SingletonClass()를 호출할 수 없음
	 */
    CEnsemble(void) ;
	
	// 싱글턴 인스턴스가 생성되었는지 여부
	static bool instanceFlag;

	// 싱글턴 인스턴스
    static CEnsemble* instance;
	
public:
	//CEnsembleAPI m_cls_api ;

	bool CheckDevice(const std::string ip, const int port) ;
	int New(const std::string ip, const int port) ;
	CEnsembleAPI *GetDevice(const std::string ip, const int port) ;
	CEnsembleAPI *GetDevice(const int index) ;
	int GetSize(void) ;
	std::string GetDeviceJobInfo(const int index=-1) ;

	CEnsembleAPI *SelectDevice(const std::string ip, const int port) ;
	CEnsembleAPI *GetSelectDevice(void) ;
	
private:
	std::list<CEnsembleAPI *> m_list_ensembles ;
	CEnsembleAPI *m_select_device ;
	
};

#endif // CENSEMBLE_H
