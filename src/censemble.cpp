#include "censemble.h"

bool CEnsemble::instanceFlag = false;
CEnsemble* CEnsemble::instance = NULL;

CEnsemble::CEnsemble() :
	m_select_device(NULL)
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

	std::list<CEnsembleAPI*>::iterator itor;// = list1.begin();      // 따로, 또는 이렇게 .
	for(itor=m_list_ensembles.begin(); itor != m_list_ensembles.end();)
	{
		//delete
		if( (*itor) != NULL )
		{
			delete (*itor) ;
			(*itor) = NULL ;
		}
		
		m_list_ensembles.erase(itor++); //erase의 경우 후위형 연산자로만 증가시켜야함
	}

	m_list_ensembles.clear();
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

CEnsembleAPI *CEnsemble::GetDevice(const int index)
{
	CEnsembleAPI *p_device = NULL ;

	if( index >= 0 )
	{
		int count = 0 ;
		std::list<CEnsembleAPI*>::iterator itor_ensemble;// = list1.begin();      // 따로, 또는 이렇게 .
		for(itor_ensemble=m_list_ensembles.begin(); itor_ensemble != m_list_ensembles.end();itor_ensemble++)
		{
			if( index == count )
			{
		        CEnsembleAPI* p_dev = (*itor_ensemble) ;
				p_device = p_dev ;
				break ;
			}

			count++ ;
		}
	}

	return p_device ;
}

std::string CEnsemble::GetDeviceJobInfo(const int index)
{
	std::string str_xml_device_info = "<?xml version='1.0' encoding='UTF-8' standalone='no' ?>" ;
	str_xml_device_info += "<Ensembles>" ;
	
	if( index < 0 )		//all
	{
		const int size_device = m_list_ensembles.size();

		qDebug("GetDeviceJobInfo : all : size_device=%d", size_device) ;

		for( int i=0 ; i<size_device ; i++ )
		{
			CEnsembleAPI *p_device = GetDevice(i) ;

			if( p_device )
			{
				qDebug("GetDeviceJobInfo : all : %d device", i) ;
				
				str_xml_device_info += p_device->Ensemble_Project_Get_List() ;
			}
		}
	}
	else
	{
		CEnsembleAPI *p_device = GetDevice(index) ;

		if( p_device )
		{
			qDebug("GetDeviceJobInfo : %d device", index) ;
			
			str_xml_device_info += p_device->Ensemble_Project_Get_List() ;
		}
	}
	str_xml_device_info += "</Ensembles>" ;

	return str_xml_device_info ;
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

CEnsembleAPI *CEnsemble::SelectDevice(const std::string ip, const int port)
{
	m_select_device = GetDevice(ip, port) ;

	return m_select_device ;
}

CEnsembleAPI *CEnsemble::GetSelectDevice(void)
{
	return m_select_device ;
}

