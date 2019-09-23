#include "csetuserregion.h"

CSetUserRegion::CSetUserRegion() :
	m_set_status(SetBaseStatus::NORMAL)
{

}

int CSetUserRegion::GetStatus(void)
{
	return m_set_status ;
}

void CSetUserRegion::SetStatus(const int status) 
{
	m_set_status = status ;
}

cv::Rect CSetUserRegion::GetRect(void)
{
	return UserRegionRect_2_cvRect(m_rect_set_rect) ;
}

cv::Rect CSetUserRegion::StartSetRegion(const int x, const int y)
{
	m_rect_set_rect.width = 0 ;
    m_rect_set_rect.height = 0 ;
    m_rect_set_rect.x = x ;
    m_rect_set_rect.y = y ;

	return UserRegionRect_2_cvRect(m_rect_set_rect) ;
}

cv::Rect CSetUserRegion::UserRegionRect_2_cvRect(const UserRegionRect user_region_rect)
{
	cv::Rect rect ;

	if( user_region_rect.width < 0 )
	{
		rect.x = user_region_rect.x + user_region_rect.width ;
		rect.width = -user_region_rect.width ;
	}
	else
	{
		rect.x = user_region_rect.x ;
		rect.width = user_region_rect.width ;
	}

	if( user_region_rect.height < 0 )
	{
		rect.y = user_region_rect.y + user_region_rect.height ;
		rect.height = -user_region_rect.height ;
	}
	else
	{
		rect.y = user_region_rect.y ;
		rect.height = user_region_rect.height ;
	}

	return rect ;
}

cv::Rect CSetUserRegion::MoveSetRegion(const int x, const int y)
{
	int width = (int)x - (int)m_rect_set_rect.x ;
	int height = (int)y - (int)m_rect_set_rect.y ;

#if 0
	if( width > 0 )
	{
		m_rect_set_rect.width = width ;
	}
	else
	{
		m_rect_set_rect.x = x ;
		m_rect_set_rect.width = abs(width) ;
	}		
	
	if( height > 0 )
	{
		m_rect_set_rect.height = height ;
	}
	else
	{
		m_rect_set_rect.y = y ;
		m_rect_set_rect.height = abs(height) ;
	}	
#else
	m_rect_set_rect.width = width ;
	m_rect_set_rect.height = height ;

#endif

	return UserRegionRect_2_cvRect(m_rect_set_rect) ;
}

cv::Rect CSetUserRegion::EndSetRegion(void)
{
	cv::Rect ret_rect = UserRegionRect_2_cvRect(m_rect_set_rect) ;

	//Reset region
	//m_rect_set_rect = cv::Rect(0,0,0,0) ;
	m_rect_set_rect.x = 0 ;
	m_rect_set_rect.y = 0 ;
	m_rect_set_rect.width = 0 ;
	m_rect_set_rect.height = 0 ;

	if( m_set_status > SetBaseStatus::NORMAL )
	{
		//Reset Status
		m_set_status = SetBaseStatus::NORMAL ;
	}
	
	return ret_rect ;
}


