#include "dialogbasecameraconfig.h"
#include "ui_dialogbasecameraconfig.h"

DialogBaseCameraConfig::DialogBaseCameraConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBaseCameraConfig)
{
    ui->setupUi(this);

	//Camera Config 
	connect(ui->pushButton_exposure_get, SIGNAL(clicked()), this, SLOT(OnButtonExposureGet())) ;	
	connect(ui->pushButton_gain_get, SIGNAL(clicked()), this, SLOT(OnButtonGainGet())) ;	
	connect(ui->pushButton_focus_get, SIGNAL(clicked()), this, SLOT(OnButtonFocusGet())) ;	
	connect(ui->pushButton_brightness_get, SIGNAL(clicked()), this, SLOT(OnButtonBrightnessGet())) ;	
	connect(ui->pushButton_sharpness_get, SIGNAL(clicked()), this, SLOT(OnButtonSharpnessGet())) ;	
	connect(ui->pushButton_contrast_get, SIGNAL(clicked()), this, SLOT(OnButtonContrastGet())) ;	
	connect(ui->pushButton_iso_get, SIGNAL(clicked()), this, SLOT(OnButtonISOGet())) ;	
	connect(ui->pushButton_shutter_speed_get, SIGNAL(clicked()), this, SLOT(OnButtonShutterSpeedGet())) ;	

	connect(ui->pushButton_exposure_set, SIGNAL(clicked()), this, SLOT(OnButtonExposureSet())) ;	
	connect(ui->pushButton_gain_set, SIGNAL(clicked()), this, SLOT(OnButtonGainSet())) ;	
	connect(ui->pushButton_focus_set, SIGNAL(clicked()), this, SLOT(OnButtonFocusSet())) ;	
	connect(ui->pushButton_brightness_set, SIGNAL(clicked()), this, SLOT(OnButtonBrightnessSet())) ;	
	connect(ui->pushButton_sharpness_set, SIGNAL(clicked()), this, SLOT(OnButtonSharpnessSet())) ;	
	connect(ui->pushButton_contrast_set, SIGNAL(clicked()), this, SLOT(OnButtonContrastSet())) ;	
	connect(ui->pushButton_iso_set, SIGNAL(clicked()), this, SLOT(OnButtonISOSet())) ;	
	connect(ui->pushButton_shutter_speed_set, SIGNAL(clicked()), this, SLOT(OnButtonShutterSpeedSet())) ;	

	//slider
    connect(ui->horizontalSlider_exposure, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetExposure()));
	connect(ui->horizontalSlider_gain, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetGain()));
	connect(ui->horizontalSlider_focus, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetFocus()));

	connect(ui->horizontalSlider_brightness, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetBrightness()));
	connect(ui->horizontalSlider_sharpness, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetSharpness()));
	connect(ui->horizontalSlider_contrast, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetContrast()));
	connect(ui->horizontalSlider_iso, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetISO()));
	connect(ui->horizontalSlider_shutter_speed, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetShutterSpeed()));

	//reset
	connect(ui->pushButton_reset, SIGNAL(clicked()), this, SLOT(OnButtonReset()));
}

DialogBaseCameraConfig::~DialogBaseCameraConfig()
{
    delete ui;
}

void DialogBaseCameraConfig::showEvent(QShowEvent *ev) 
{
    QDialog::showEvent(ev) ;

	//Update Camera Information
	//exposure min/max value
	const int exposure_min = Ensemble_Camera_Get_Min_Exposure_Value() ;
	const int exposure_max = Ensemble_Camera_Get_Max_Exposure_Value() ;

	//gain min/max value
	const int gain_min = Ensemble_Camera_Get_Min_Gain_Value() ;
	const int gain_max = Ensemble_Camera_Get_Max_Gain_Value() ;

	//focus min/max value
	const int focus_min = Ensemble_Camera_Get_Min_Focus_Value() ;
	const int focus_max = Ensemble_Camera_Get_Max_Focus_Value() ;

	//brightness min/max value
	const int brightness_min = Ensemble_Camera_Get_Min_Brightness_Value() ;
	const int brightness_max = Ensemble_Camera_Get_Max_Brightness_Value() ;

	//sharpness min/max value
	const int sharpness_min = Ensemble_Camera_Get_Min_Sharpness_Value() ;
	const int sharpness_max = Ensemble_Camera_Get_Max_Sharpness_Value() ;

	//contrast min/max value
	const int contrast_min = Ensemble_Camera_Get_Min_Contrast_Value() ;
	const int contrast_max = Ensemble_Camera_Get_Max_Contrast_Value() ;

	//iso min/max value
	const int iso_min = Ensemble_Camera_Get_Min_ISO_Value() ;
	const int iso_max = Ensemble_Camera_Get_Max_ISO_Value() ;

	//shutter speed min/max value
	const int shutter_speed_min = Ensemble_Camera_Get_Min_Shutter_Speed_Value() ;
	const int shutter_speed_max = Ensemble_Camera_Get_Max_Shutter_Speed_Value() ;

	ui->label_exposure_min->setText(QString::number(exposure_min)) ;
	ui->label_exposure_max->setText(QString::number(exposure_max)) ;

	ui->label_gain_min->setText(QString::number(gain_min)) ;
	ui->label_gain_max->setText(QString::number(gain_max)) ;

	ui->label_focus_min->setText(QString::number(focus_min)) ;
	ui->label_focus_max->setText(QString::number(focus_max)) ;

	ui->label_brightness_min->setText(QString::number(brightness_min)) ;
	ui->label_brightness_max->setText(QString::number(brightness_max)) ;

	ui->label_sharpness_min->setText(QString::number(sharpness_min)) ;
	ui->label_sharpness_max->setText(QString::number(sharpness_max)) ;

	ui->label_contrast_min->setText(QString::number(contrast_min)) ;
	ui->label_contrast_max->setText(QString::number(contrast_max)) ;

	ui->label_iso_min->setText(QString::number(iso_min)) ;
	ui->label_iso_max->setText(QString::number(iso_max)) ;

	ui->label_shutter_speed_min->setText(QString::number(shutter_speed_min)) ;
	ui->label_shutter_speed_max->setText(QString::number(shutter_speed_max)) ;

	//Set Range
	ui->horizontalSlider_exposure->setRange(exposure_min, exposure_max) ;
	ui->horizontalSlider_gain->setRange(exposure_min, exposure_max) ;
	ui->horizontalSlider_focus->setRange(focus_min, focus_max) ;
	ui->horizontalSlider_brightness->setRange(brightness_min, brightness_max) ;
	ui->horizontalSlider_sharpness->setRange(sharpness_min, sharpness_max) ;
	ui->horizontalSlider_contrast->setRange(contrast_min, contrast_max) ;
	ui->horizontalSlider_iso->setRange(iso_min, iso_max) ;
	ui->horizontalSlider_shutter_speed->setRange(shutter_speed_min, shutter_speed_max) ;
	
	OnButtonExposureGet() ;
	OnButtonGainGet() ;
	OnButtonFocusGet() ;
	OnButtonBrightnessGet() ;
	OnButtonSharpnessGet() ;
	OnButtonContrastGet() ;
	OnButtonISOGet() ;
	OnButtonShutterSpeedGet() ;

	//Checkbox
	int check_auto_exposure = Ensemble_Camera_Get_Auto_Exposure_OnOff() ;
	if( check_auto_exposure )	ui->checkBox_auto_exposure->setChecked(true) ;
	else						ui->checkBox_auto_exposure->setChecked(false) ;

	int check_auto_focus = Ensemble_Camera_Get_Auto_Focus_OnOff() ;
	if( check_auto_focus )		ui->checkBox_auto_focus->setChecked(true) ;
	else						ui->checkBox_auto_focus->setChecked(false) ;

}

void DialogBaseCameraConfig::OnButtonExposureGet(void)
{
	int exposure_value = Ensemble_Camera_Get_Manual_Exposure_Value() ;

	ui->lineEdit_exposure->setText(QString::number(exposure_value));

	ui->horizontalSlider_exposure->setValue(exposure_value) ;
}

void DialogBaseCameraConfig::OnButtonGainGet(void)
{
	int gain_value = Ensemble_Camera_Get_Manual_Gain_Value() ;

	ui->lineEdit_gain->setText(QString::number(gain_value));

	ui->horizontalSlider_gain->setValue(gain_value) ;
}

void DialogBaseCameraConfig::OnButtonFocusGet(void)
{
	int focus_value = Ensemble_Camera_Get_Manual_Focus_Value() ;

	ui->lineEdit_focus->setText(QString::number(focus_value));

	ui->horizontalSlider_focus->setValue(focus_value) ;
}

void DialogBaseCameraConfig::OnButtonBrightnessGet(void)
{
	int brightness_value = Ensemble_Camera_Get_Manual_Brightness_Value() ;

	ui->lineEdit_brightness->setText(QString::number(brightness_value));

	ui->horizontalSlider_brightness->setValue(brightness_value) ;
}

void DialogBaseCameraConfig::OnButtonSharpnessGet(void)
{
	int sharpness_value = Ensemble_Camera_Get_Manual_Sharpness_Value() ;

	ui->lineEdit_sharpness->setText(QString::number(sharpness_value));

	ui->horizontalSlider_sharpness->setValue(sharpness_value) ;
}

void DialogBaseCameraConfig::OnButtonContrastGet(void)
{
	int contrast_value = Ensemble_Camera_Get_Manual_Contrast_Value() ;

	ui->lineEdit_contrast->setText(QString::number(contrast_value));

	ui->horizontalSlider_contrast->setValue(contrast_value) ;
}

void DialogBaseCameraConfig::OnButtonISOGet(void)
{
	int iso_value = Ensemble_Camera_Get_Manual_ISO_Value() ;

	ui->lineEdit_iso->setText(QString::number(iso_value));

	ui->horizontalSlider_iso->setValue(iso_value) ;
}

void DialogBaseCameraConfig::OnButtonShutterSpeedGet(void)
{
	int shutter_speed_value = Ensemble_Camera_Get_Manual_Shutter_Speed_Value() ;

	ui->lineEdit_shutter_speed->setText(QString::number(shutter_speed_value));

	ui->horizontalSlider_shutter_speed->setValue(shutter_speed_value) ;
}

void DialogBaseCameraConfig::OnButtonExposureSet(void)
{
    int value = ui->lineEdit_exposure->text().toInt() ;
    Ensemble_Camera_Set_Manual_Exposure_Value(value) ;
	
	OnButtonExposureGet() ;
}

void DialogBaseCameraConfig::OnButtonGainSet(void)
{
    int value = ui->lineEdit_gain->text().toInt() ;
    Ensemble_Camera_Set_Manual_Gain_Value(value) ;

	OnButtonGainGet() ;
}

void DialogBaseCameraConfig::OnButtonFocusSet(void)
{
    int value = ui->lineEdit_focus->text().toInt() ;
    Ensemble_Camera_Set_Manual_Focus_Value(value) ;

	OnButtonFocusGet() ;
}

void DialogBaseCameraConfig::OnButtonBrightnessSet(void)
{
	int value = ui->lineEdit_brightness->text().toInt() ;
    Ensemble_Camera_Set_Manual_Brightness_Value(value) ;

	OnButtonBrightnessGet() ;
}

void DialogBaseCameraConfig::OnButtonSharpnessSet(void)
{
	int value = ui->lineEdit_sharpness->text().toInt() ;
    Ensemble_Camera_Set_Manual_Sharpness_Value(value) ;

	OnButtonSharpnessGet() ;
}

void DialogBaseCameraConfig::OnButtonContrastSet(void)
{
	int value = ui->lineEdit_contrast->text().toInt() ;
    Ensemble_Camera_Set_Manual_Contrast_Value(value) ;

	OnButtonContrastGet() ;
}

void DialogBaseCameraConfig::OnButtonISOSet(void)
{
	int value = ui->lineEdit_iso->text().toInt() ;
    Ensemble_Camera_Set_Manual_ISO_Value(value) ;

	OnButtonISOGet() ;
}

void DialogBaseCameraConfig::OnButtonShutterSpeedSet(void)
{
	int value = ui->lineEdit_shutter_speed->text().toInt() ;
    Ensemble_Camera_Set_Manual_Shutter_Speed_Value(value) ;

	OnButtonShutterSpeedGet() ;
}

void DialogBaseCameraConfig::OnSliderSetExposure(void)
{
	//get value form slider
    int value = ui->horizontalSlider_exposure->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Exposure_Value(value);

	OnButtonExposureGet() ;
}

void DialogBaseCameraConfig::OnSliderSetGain(void) 
{
	//get value form slider
    int value = ui->horizontalSlider_gain->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Gain_Value(value);

	OnButtonGainGet() ;
}


void DialogBaseCameraConfig::OnSliderSetFocus(void) 
{
	//get value form slider
    int value = ui->horizontalSlider_focus->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Focus_Value(value);

	OnButtonFocusGet() ;
}

void DialogBaseCameraConfig::OnSliderSetBrightness(void)
{
	//get value form slider
    int value = ui->horizontalSlider_brightness->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Brightness_Value(value);

	OnButtonBrightnessGet() ;
}

void DialogBaseCameraConfig::OnSliderSetSharpness(void)
{
	//get value form slider
    int value = ui->horizontalSlider_sharpness->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Sharpness_Value(value);

	OnButtonSharpnessGet() ;
}

void DialogBaseCameraConfig::OnSliderSetContrast(void)
{
	//get value form slider
    int value = ui->horizontalSlider_contrast->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Contrast_Value(value);

	OnButtonContrastGet() ;
}

void DialogBaseCameraConfig::OnSliderSetISO(void)
{
	//get value form slider
    int value = ui->horizontalSlider_iso->value() ;

	//set value
	Ensemble_Camera_Set_Manual_ISO_Value(value);

	OnButtonISOGet() ;
}

void DialogBaseCameraConfig::OnSliderSetShutterSpeed(void)
{
	//get value form slider
    int value = ui->horizontalSlider_shutter_speed->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Shutter_Speed_Value(value);

	OnButtonShutterSpeedGet() ;
}

void DialogBaseCameraConfig::OnButtonReset(void)
{
    Ensemble_Camera_Set_Factory_Reset() ;

	OnButtonExposureGet() ;
	OnButtonGainGet() ;
	OnButtonFocusGet() ;
	OnButtonBrightnessGet() ;
	OnButtonSharpnessGet() ;
	OnButtonContrastGet() ;
	OnButtonISOGet() ;
	OnButtonShutterSpeedGet() ;

	//Checkbox
	int check_auto_exposure = Ensemble_Camera_Get_Auto_Exposure_OnOff() ;
	if( check_auto_exposure )	ui->checkBox_auto_exposure->setChecked(true) ;
	else						ui->checkBox_auto_exposure->setChecked(false) ;

	int check_auto_focus = Ensemble_Camera_Get_Auto_Focus_OnOff() ;
	if( check_auto_focus )		ui->checkBox_auto_focus->setChecked(true) ;
	else						ui->checkBox_auto_focus->setChecked(false) ;
}

