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

bool CEnsemble::CheckDevice(const std::string ip, const int port)
{
	bool b_exist = false ;
	
	std::list<CEnsembleAPI*>::iterator itor_ensemble;// = list1.begin();      // 따로, 또는 이렇게 .
	for(itor_ensemble=m_list_ensembles.begin(); itor_ensemble != m_list_ensembles.end();itor_ensemble++)
	{
		if( itor_ensemble )
		{
			std::string check_str_ip ;
			int check_port ;
			itor_ensemble->Ensemble_Network_GetInfo(&check_str_ip, &check_port) ;

			if( check_str_ip == ip && check_port == port )
			{
				b_exist = true ;
				break ;
			}
		}
	}
	
	return b_exist ;
}

void CEnsemble::New(const std::string ip, const int port)
{
	if( CheckDevice() == false )
	{		
		CEnsembleAPI *p_cls_api = new CEnsembleAPI ;

		p_cls_api.Ensemble_Network_Connect(ip, port) ;

		m_list_ensembles.push(p_cls_api) ;
	}
}



