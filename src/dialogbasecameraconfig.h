#ifndef DIALOGBASECAMERACONFIG_H
#define DIALOGBASECAMERACONFIG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "cgetimagethread.h"
#include "cmat2qimage.h"

#include "csetuserregion.h"

namespace Ui {
class DialogBaseCameraConfig;
}

class DialogBaseCameraConfig : public QDialog, public CSetUserRegion
{
    Q_OBJECT

public:
    explicit DialogBaseCameraConfig(QWidget *parent = 0);
    ~DialogBaseCameraConfig();

	void SetId(const std::string id) ;
    std::string GetId(void) ;

protected :
	void showEvent(QShowEvent *ev) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::DialogBaseCameraConfig *ui;
	std::string m_str_id ;

	CGetImageThread* m_p_cls_getimage ;

	CSetUserRegion m_cls_set_user_region ;

	cv::Rect m_rect_user ;

public slots:
	void updatePicture(cv::Mat image);

	void OnButtonExposureGet(void) ;
	void OnButtonGainGet(void) ;
	void OnButtonFocusGet(void) ;

	void OnButtonBrightnessGet(void) ;
	void OnButtonSharpnessGet(void) ;
	void OnButtonContrastGet(void) ;
	void OnButtonISOGet(void) ;
	void OnButtonShutterSpeedGet(void) ;

	void OnButtonSetAutoExposure(void) ;

	void OnButtonSetAutoFocusSelectArea(void) ;
	void OnButtonSetAutoFocusAllArea(void) ;
	
	void OnButtonExposureSet(void) ;
	void OnButtonGainSet(void) ;
	void OnButtonFocusSet(void) ;

	void OnButtonBrightnessSet(void) ;
	void OnButtonSharpnessSet(void) ;
	void OnButtonContrastSet(void) ;
	void OnButtonISOSet(void) ;
	void OnButtonShutterSpeedSet(void) ;

	void OnButtonSetImageFlip_V(void) ;
	void OnButtonSetImageFlip_H(void) ;

	void OnButtonReset(void) ;
	void OnButtonSetCamera(void) ;

	//slider
	void OnSliderSetExposure(void) ;
	void OnSliderSetGain(void) ;
	void OnSliderSetFocus(void) ;
	void OnSliderSetBrightness(void) ;
	void OnSliderSetSharpness(void) ;
	void OnSliderSetContrast(void) ;
	void OnSliderSetISO(void) ;
	void OnSliderSetShutterSpeed(void) ;

private slots:
    void on_pushButton_save_default_clicked();
};

#endif // DIALOGBASECAMERACONFIG_H
