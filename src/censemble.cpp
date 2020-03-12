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

	if( GetDevice(ip, port) )
	{
		b_exist = true ;
	}
		
	return b_exist ;
}

CEnsembleAPI *CEnsemble::GetDevice(const std::string ip, const int port)
{
	CEnsembleAPI *p_device = NULL ;
	
	std::list<CEnsembleAPI*>::iterator itor_ensemble;// = list1.begin();      // 따로, 또는 이렇게 .
	for(itor_ensemble=m_list_ensembles.begin(); itor_ensemble != m_list_ensembles.end();itor_ensemble++)
	{
        CEnsembleAPI* p_dev = (*itor_ensemble) ;

        if( p_dev )
		{
			std::string check_str_ip ;
			int check_port ;
            p_dev->Ensemble_Network_GetInfo(&check_str_ip, &check_port) ;
			
			if( check_str_ip == ip && check_port == port )
			{
                p_device = p_dev ;
				break ;
			}
		}
	}

	return p_device ;
}


int CEnsemble::New(const std::string ip, const int port)
{
    if( CheckDevice(ip, port) == false )
	{		
		CEnsembleAPI *p_cls_api = new CEnsembleAPI ;

        p_cls_api->Ensemble_Network_Connect(ip.c_str(), port) ;

        m_list_ensembles.push_back(p_cls_api) ;
	}

	return m_list_ensembles.size() ;
}



