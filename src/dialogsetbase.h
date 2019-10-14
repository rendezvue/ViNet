#ifndef DIALOGSETBASE_H
#define DIALOGSETBASE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "ImgDec.h"
#include "EnsembleCommon.h"

//API
#include "EnsembleAPI.h"

//opencv
#include "opencv2/opencv.hpp"

//boost 
#include <boost/thread.hpp>									
#include <boost/thread/detail/thread.hpp>

#include "dialogchangename.h"

#include "csetuserregion.h"


namespace Ui {
class DialogSetBase;
}

class DialogSetBase : public QDialog, public CSetUserRegion
{
    Q_OBJECT

public:
    explicit DialogSetBase(QWidget *parent = 0);
    ~DialogSetBase();

    void SetId(const std::string id) ;
    std::string GetId(void) ;
	
private:
    Ui::DialogSetBase *ui;

    std::string m_str_id ;

    void updatePicture(cv::Mat image, cv::Rect rect_user=cv::Rect()) ;
	cv::Mat m_image ;

	CSetUserRegion m_cls_set_user_region ;
	
protected :
	void showEvent(QShowEvent *ev) override;
#if 1
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#endif

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
    //void OnSliderSetFeatureLevel(int level) ;
    void OnSliderSetFeatureLevel(void) ;
    void OnSliderMove(int value) ;
	void OnButtonSelectObject(void) ;
	void OnButtonResetObject(void) ;

    void OnButtonSetDetectOptionMargin(void) ;
    void OnButtonSetDetectOptionThreshold(void) ;
    void OnButtonSetDetectOptionCount(void) ;
    void OnButtonGetDetectOptionMargin(void) ;
    void OnButtonGetDetectOptionThreshold(void) ;
    void OnButtonGetDetectOptionCount(void) ;

    void OnCheckFeatureUseCustomOption(bool checked) ;

signals:
	void UpdateBaseImage(void);
	void UpdateBaseName(QString name) ;
	

};

#endif // DIALOGSETBASE_H
