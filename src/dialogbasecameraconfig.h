#ifndef DIALOGBASECAMERACONFIG_H
#define DIALOGBASECAMERACONFIG_H

#include <QDialog>

//API
#include "EnsembleAPI.h"


namespace Ui {
class DialogBaseCameraConfig;
}

class DialogBaseCameraConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBaseCameraConfig(QWidget *parent = 0);
    ~DialogBaseCameraConfig();

	void SetId(const std::string id) ;
    std::string GetId(void) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogBaseCameraConfig *ui;
	std::string m_str_id ;

public slots:
	void OnButtonExposureGet(void) ;
	void OnButtonGainGet(void) ;
	void OnButtonFocusGet(void) ;

	void OnButtonBrightnessGet(void) ;
	void OnButtonSharpnessGet(void) ;
	void OnButtonContrastGet(void) ;
	void OnButtonISOGet(void) ;
	void OnButtonShutterSpeedGet(void) ;
	
	void OnButtonExposureSet(void) ;
	void OnButtonGainSet(void) ;
	void OnButtonFocusSet(void) ;

	void OnButtonBrightnessSet(void) ;
	void OnButtonSharpnessSet(void) ;
	void OnButtonContrastSet(void) ;
	void OnButtonISOSet(void) ;
	void OnButtonShutterSpeedSet(void) ;

	void OnButtonReset(void) ;

	//slider
	void OnSliderSetExposure(void) ;
	void OnSliderSetGain(void) ;
	void OnSliderSetFocus(void) ;
	void OnSliderSetBrightness(void) ;
	void OnSliderSetSharpness(void) ;
	void OnSliderSetContrast(void) ;
	void OnSliderSetISO(void) ;
	void OnSliderSetShutterSpeed(void) ;

};

#endif // DIALOGBASECAMERACONFIG_H
