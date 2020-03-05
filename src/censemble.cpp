#include "censemble.h"

bool CEnsemble::instanceFlag = false;
CEnsemble* CEnsemble::instance = NULL;

CEnsemble::CEnsemble()
{

}


CEnsemble::~CEnsemble(void)
{
	instanceFlag = false;

	if( instance )
	{
		delete instance ;
		instance = NULL;
	}
};
	
// 싱글턴 인스턴스를 반환할 멤버함수
CEnsemble* CEnsemble::getInstance()
{
	if(instance == NULL)
	{
		instance = new CEnsemble();
		instanceFlag = true;
	}
	return instance;
}

