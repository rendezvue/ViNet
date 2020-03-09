#ifndef DIALOGSETDETECTPLANE_H
#define DIALOGSETDETECTPLANE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "ImgDec.h"
#include "EnsembleCommon.h"

#include "cmat2qimage.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

//opencv
#include "opencv2/opencv.hpp"

//boost 
#include <boost/thread.hpp>									
#include <boost/thread/detail/thread.hpp>

#include "dialogchangename.h"
#include "dialogsetcustomfeatureoption.h"

#include "csetuserregion.h"


namespace Ui {
class DialogSetDetectPlane;
}

class DialogSetDetectPlane : public QDialog, public CSetUserRegion
{
    Q_OBJECT

public:
    explicit DialogSetDetectPlane(QWidget *parent = 0);
    ~DialogSetDetectPlane();

	void SetId(const std::string id) ;
    std::string GetId(void) ;
	
private:
    Ui::DialogSetDetectPlane *ui;

	std::string m_str_id ;
	cv::Mat m_image ;

	void updatePicture(cv::Mat image, cv::Rect rect_user=cv::Rect()) ;
	void updatePictureCenterLine(cv::Mat image, cv::Point pt_user=cv::Point()) ;
	CSetUserRegion m_cls_set_user_region ;
	 
protected :
	void showEvent(QShowEvent *ev) override;
	void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonSetImage(void) ;
	void OnButtonSetDetectArea(void) ;
    void OnButtonClearDetectArea(void) ;
	void OnButtonZoomSet(void) ;
    void OnButtonZoomReset(void) ;
    void OnButtonNameChange(void) ;
	//mask
    void OnButtonMaskPush(void) ;
    void OnButtonMaskPop(void) ;
    void OnButtonMaskClear(void) ;
	//object
	void OnButtonSelectObject(void) ;
	void OnButtonResetObject(void) ;

signals:
	void UpdateBaseImage(void);
	void UpdateBaseName(QString name) ;

};

#endif // DIALOGSETDETECTPLANE_H
