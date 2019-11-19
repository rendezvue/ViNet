#ifndef CSETUSERREGION_H
#define CSETUSERREGION_H

//opencv
#include "opencv2/opencv.hpp"

enum SetBaseStatus
{
	NORMAL,
    SET_AREA,
    SET_ZOOM ,
    SET_MASK ,
    SET_OBJECT,
    SET_REF_POINT ,    
	SET_ERASE
};

typedef struct _UserRegionRect
{
	int x = 0 ;
	int y = 0 ;
	int width = 0 ;
	int height = 0 ;
} UserRegionRect;

	
class CSetUserRegion
{
public:
    CSetUserRegion();

	int GetStatus(void) ;
	cv::Rect GetRect(void) ;

	void SetStatus(const int status)  ;
	
	//ctrl
	cv::Rect StartSetRegion(const int x, const int y);
    cv::Rect MoveSetRegion(const int x, const int y);
    cv::Rect EndSetRegion(void);

private:
	unsigned int m_set_status ;
	UserRegionRect m_rect_set_rect ;

	cv::Rect UserRegionRect_2_cvRect(const UserRegionRect user_region_rect) ;
};

#endif // CSETUSERREGION_H
