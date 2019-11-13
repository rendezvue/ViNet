#include "dialogbasecameraconfig.h"
#include "ui_dialogbasecameraconfig.h"

DialogBaseCameraConfig::DialogBaseCameraConfig(QWidget *parent) :
	m_p_cls_getimage(NULL),
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

	//Auto Focus Area
	connect(ui->pushButton_auto_focus_select_area, SIGNAL(clicked()), this, SLOT(OnButtonSetAutoFocusSelectArea())) ;	
	connect(ui->pushButton_auto_focus_all_area, SIGNAL(clicked()), this, SLOT(OnButtonSetAutoFocusAllArea())) ;	
	
	connect(ui->pushButton_exposure_set, SIGNAL(clicked()), this, SLOT(OnButtonExposureSet())) ;	
	connect(ui->pushButton_gain_set, SIGNAL(clicked()), this, SLOT(OnButtonGainSet())) ;	
	connect(ui->pushButton_focus_set, SIGNAL(clicked()), this, SLOT(OnButtonFocusSet())) ;	
	connect(ui->pushButton_brightness_set, SIGNAL(clicked()), this, SLOT(OnButtonBrightnessSet())) ;	
	connect(ui->pushButton_sharpness_set, SIGNAL(clicked()), this, SLOT(OnButtonSharpnessSet())) ;	
	connect(ui->pushButton_contrast_set, SIGNAL(clicked()), this, SLOT(OnButtonContrastSet())) ;	
	connect(ui->pushButton_iso_set, SIGNAL(clicked()), this, SLOT(OnButtonISOSet())) ;	
	connect(ui->pushButton_shutter_speed_set, SIGNAL(clicked()), this, SLOT(OnButtonShutterSpeedSet())) ;	

	connect(ui->checkBox_image_flip_v, SIGNAL(clicked()), this, SLOT(OnButtonSetImageFlip_V())) ;	
	connect(ui->checkBox_image_flip_h, SIGNAL(clicked()), this, SLOT(OnButtonSetImageFlip_H())) ;	


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

	//Set
	connect(ui->pushButton_set_camera, SIGNAL(clicked()), this, SLOT(OnButtonSetCamera())) ;	

	//Image Thread
	m_p_cls_getimage = new CGetImageThread(this) ;
	m_p_cls_getimage->SetSourceType(0);
    connect(m_p_cls_getimage, SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture(cv::Mat))) ;
	m_p_cls_getimage->m_thread_run = true ;
    m_p_cls_getimage->start();
}

DialogBaseCameraConfig::~DialogBaseCameraConfig()
{
	if( m_p_cls_getimage != NULL )
    {
    	m_p_cls_getimage->m_thread_run = false ;

		while( m_p_cls_getimage->isFinished() == false )
		{
		} ;
		
    	m_p_cls_getimage->quit();
		
        delete m_p_cls_getimage ;
        m_p_cls_getimage = NULL ;
    }
	
    delete ui;
}

void DialogBaseCameraConfig::showEvent(QShowEvent *ev) 
{
    QDialog::showEvent(ev) ;

	//Update Camera Information
	//exposure min/max value
	const int exposure_min = Ensemble_Camera_Get_Min_Exposure_Value(GetId()) ;
	const int exposure_max = Ensemble_Camera_Get_Max_Exposure_Value(GetId()) ;

	//gain min/max value
	const int gain_min = Ensemble_Camera_Get_Min_Gain_Value(GetId()) ;
	const int gain_max = Ensemble_Camera_Get_Max_Gain_Value(GetId()) ;

	//focus min/max value
	const int focus_min = Ensemble_Camera_Get_Min_Focus_Value(GetId()) ;
	const int focus_max = Ensemble_Camera_Get_Max_Focus_Value(GetId()) ;

	//brightness min/max value
	const int brightness_min = Ensemble_Camera_Get_Min_Brightness_Value(GetId()) ;
	const int brightness_max = Ensemble_Camera_Get_Max_Brightness_Value(GetId()) ;

	//sharpness min/max value
	const int sharpness_min = Ensemble_Camera_Get_Min_Sharpness_Value(GetId()) ;
	const int sharpness_max = Ensemble_Camera_Get_Max_Sharpness_Value(GetId()) ;

	//contrast min/max value
	const int contrast_min = Ensemble_Camera_Get_Min_Contrast_Value(GetId()) ;
	const int contrast_max = Ensemble_Camera_Get_Max_Contrast_Value(GetId()) ;

	//iso min/max value
	const int iso_min = Ensemble_Camera_Get_Min_ISO_Value(GetId()) ;
	const int iso_max = Ensemble_Camera_Get_Max_ISO_Value(GetId()) ;

	//shutter speed min/max value
	const int shutter_speed_min = Ensemble_Camera_Get_Min_Shutter_Speed_Value(GetId()) ;
	const int shutter_speed_max = Ensemble_Camera_Get_Max_Shutter_Speed_Value(GetId()) ;

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

	//image flip
	int check_image_flip_v = Ensemble_Camera_Get_Flip_V(GetId()) ;
	if( check_image_flip_v )	ui->checkBox_image_flip_v->setChecked(true) ;
	else						ui->checkBox_image_flip_v->setChecked(false) ;
	
	int check_image_flip_h = Ensemble_Camera_Get_Flip_H(GetId()) ;
	if( check_image_flip_h )	ui->checkBox_image_flip_h->setChecked(true) ;
	else						ui->checkBox_image_flip_h->setChecked(false) ;
	
}

void DialogBaseCameraConfig::OnButtonExposureGet(void)
{
	int exposure_value = Ensemble_Camera_Get_Manual_Exposure_Value(GetId()) ;

	ui->lineEdit_exposure->setText(QString::number(exposure_value));

	ui->horizontalSlider_exposure->setValue(exposure_value) ;
}

void DialogBaseCameraConfig::OnButtonGainGet(void)
{
	int gain_value = Ensemble_Camera_Get_Manual_Gain_Value(GetId()) ;

	ui->lineEdit_gain->setText(QString::number(gain_value));

	ui->horizontalSlider_gain->setValue(gain_value) ;
}

void DialogBaseCameraConfig::OnButtonFocusGet(void)
{
	int focus_value = Ensemble_Camera_Get_Manual_Focus_Value(GetId()) ;

	ui->lineEdit_focus->setText(QString::number(focus_value));

	ui->horizontalSlider_focus->setValue(focus_value) ;
}

void DialogBaseCameraConfig::OnButtonBrightnessGet(void)
{
	int brightness_value = Ensemble_Camera_Get_Manual_Brightness_Value(GetId()) ;

	ui->lineEdit_brightness->setText(QString::number(brightness_value));

	ui->horizontalSlider_brightness->setValue(brightness_value) ;
}

void DialogBaseCameraConfig::OnButtonSharpnessGet(void)
{
	int sharpness_value = Ensemble_Camera_Get_Manual_Sharpness_Value(GetId()) ;

	ui->lineEdit_sharpness->setText(QString::number(sharpness_value));

	ui->horizontalSlider_sharpness->setValue(sharpness_value) ;
}

void DialogBaseCameraConfig::OnButtonContrastGet(void)
{
	int contrast_value = Ensemble_Camera_Get_Manual_Contrast_Value(GetId()) ;

	ui->lineEdit_contrast->setText(QString::number(contrast_value));

	ui->horizontalSlider_contrast->setValue(contrast_value) ;
}

void DialogBaseCameraConfig::OnButtonISOGet(void)
{
	int iso_value = Ensemble_Camera_Get_Manual_ISO_Value(GetId()) ;

	ui->lineEdit_iso->setText(QString::number(iso_value));

	ui->horizontalSlider_iso->setValue(iso_value) ;
}

void DialogBaseCameraConfig::OnButtonShutterSpeedGet(void)
{
	int shutter_speed_value = Ensemble_Camera_Get_Manual_Shutter_Speed_Value(GetId()) ;

	ui->lineEdit_shutter_speed->setText(QString::number(shutter_speed_value));

	ui->horizontalSlider_shutter_speed->setValue(shutter_speed_value) ;
}

void DialogBaseCameraConfig::OnButtonExposureSet(void)
{
    int value = ui->lineEdit_exposure->text().toInt() ;
    Ensemble_Camera_Set_Manual_Exposure_Value(GetId(), value) ;
	
	OnButtonExposureGet() ;
}

void DialogBaseCameraConfig::OnButtonGainSet(void)
{
    int value = ui->lineEdit_gain->text().toInt() ;
    Ensemble_Camera_Set_Manual_Gain_Value(GetId(), value) ;

	OnButtonGainGet() ;
}

void DialogBaseCameraConfig::OnButtonFocusSet(void)
{
    int value = ui->lineEdit_focus->text().toInt() ;
    Ensemble_Camera_Set_Manual_Focus_Value(GetId(), value) ;

	OnButtonFocusGet() ;
}

void DialogBaseCameraConfig::OnButtonBrightnessSet(void)
{
	int value = ui->lineEdit_brightness->text().toInt() ;
    Ensemble_Camera_Set_Manual_Brightness_Value(GetId(), value) ;

	OnButtonBrightnessGet() ;
}

void DialogBaseCameraConfig::OnButtonSharpnessSet(void)
{
	int value = ui->lineEdit_sharpness->text().toInt() ;
    Ensemble_Camera_Set_Manual_Sharpness_Value(GetId(), value) ;

	OnButtonSharpnessGet() ;
}

void DialogBaseCameraConfig::OnButtonContrastSet(void)
{
	int value = ui->lineEdit_contrast->text().toInt() ;
    Ensemble_Camera_Set_Manual_Contrast_Value(GetId(), value) ;

	OnButtonContrastGet() ;
}

void DialogBaseCameraConfig::OnButtonISOSet(void)
{
	int value = ui->lineEdit_iso->text().toInt() ;
    Ensemble_Camera_Set_Manual_ISO_Value(GetId(), value) ;

	OnButtonISOGet() ;
}

void DialogBaseCameraConfig::OnButtonShutterSpeedSet(void)
{
	int value = ui->lineEdit_shutter_speed->text().toInt() ;
    Ensemble_Camera_Set_Manual_Shutter_Speed_Value(GetId(), value) ;

	OnButtonShutterSpeedGet() ;
}

void DialogBaseCameraConfig::OnButtonSetImageFlip_V(void)
{
	if (ui->checkBox_image_flip_v->isChecked())	Ensemble_Camera_Set_Flip_V(GetId(), true) ;
	else										Ensemble_Camera_Set_Flip_V(GetId(), false) ;
	
	int check_image_flip_v = Ensemble_Camera_Get_Flip_V(GetId()) ;
	if( check_image_flip_v )	ui->checkBox_image_flip_v->setChecked(true) ;
	else						ui->checkBox_image_flip_v->setChecked(false) ;
}

void DialogBaseCameraConfig::OnButtonSetImageFlip_H(void) 
{
	if (ui->checkBox_image_flip_h->isChecked())	Ensemble_Camera_Set_Flip_H(GetId(), true) ;
	else										Ensemble_Camera_Set_Flip_H(GetId(), false) ;
	
	int check_image_flip_h = Ensemble_Camera_Get_Flip_H(GetId()) ;
	if( check_image_flip_h )	ui->checkBox_image_flip_h->setChecked(true) ;
	else						ui->checkBox_image_flip_h->setChecked(false) ;
}

void DialogBaseCameraConfig::OnSliderSetExposure(void)
{
	//get value form slider
    int value = ui->horizontalSlider_exposure->value() ;
 
	//set value
	Ensemble_Camera_Set_Manual_Exposure_Value(GetId(), value);

	OnButtonExposureGet() ;
}

void DialogBaseCameraConfig::OnSliderSetGain(void) 
{
	//get value form slider
    int value = ui->horizontalSlider_gain->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Gain_Value(GetId() ,value);

	OnButtonGainGet() ;
}


void DialogBaseCameraConfig::OnSliderSetFocus(void) 
{
	//get value form slider
    int value = ui->horizontalSlider_focus->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Focus_Value(GetId(), value);

	OnButtonFocusGet() ;
}

void DialogBaseCameraConfig::OnSliderSetBrightness(void)
{
	//get value form slider
    int value = ui->horizontalSlider_brightness->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Brightness_Value(GetId(), value);

	OnButtonBrightnessGet() ;
}

void DialogBaseCameraConfig::OnSliderSetSharpness(void)
{
	//get value form slider
    int value = ui->horizontalSlider_sharpness->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Sharpness_Value(GetId(), value);

	OnButtonSharpnessGet() ;
}

void DialogBaseCameraConfig::OnSliderSetContrast(void)
{
	//get value form slider
    int value = ui->horizontalSlider_contrast->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Contrast_Value(GetId(), value);

	OnButtonContrastGet() ;
}

void DialogBaseCameraConfig::OnSliderSetISO(void)
{
	//get value form slider
    int value = ui->horizontalSlider_iso->value() ;

	//set value
	Ensemble_Camera_Set_Manual_ISO_Value(GetId(), value);

	OnButtonISOGet() ;
}

void DialogBaseCameraConfig::OnSliderSetShutterSpeed(void)
{
	//get value form slider
    int value = ui->horizontalSlider_shutter_speed->value() ;

	//set value
	Ensemble_Camera_Set_Manual_Shutter_Speed_Value(GetId(), value);

	OnButtonShutterSpeedGet() ;
}

void DialogBaseCameraConfig::OnButtonReset(void)
{
    Ensemble_Camera_Set_Factory_Reset(GetId()) ;

	OnButtonExposureGet() ;
	OnButtonGainGet() ;
	OnButtonFocusGet() ;
	OnButtonBrightnessGet() ;
	OnButtonSharpnessGet() ;
	OnButtonContrastGet() ;
	OnButtonISOGet() ;
	OnButtonShutterSpeedGet() ;
}

void DialogBaseCameraConfig::OnButtonSetCamera(void)
{
	Ensemble_Camera_Set_Config_Run(GetId()) ;
}

void DialogBaseCameraConfig::SetId(const std::string id)
{
    m_str_id = id ;

	if( m_str_id == "ini" )
	{
		ui->label_id->setText("Ensemble Camera Device") ;
	}
	else
	{
	    ui->label_id->setText(QString::fromUtf8(m_str_id.c_str()));
	}
}

std::string DialogBaseCameraConfig::GetId(void)
{
    return m_str_id ;
}

void DialogBaseCameraConfig::updatePicture(cv::Mat image)
{
    //ui->ui_label->setPixmap(QPixmap::fromImage(qt_display_image));
    QLabel *p_image_label_bg = NULL ;
    QLabel *p_image_label = NULL ;

    p_image_label = ui->label_image ;
    p_image_label_bg = ui->label_image_bg ;

    if( p_image_label && p_image_label_bg )
    {
        const int draw_width = p_image_label_bg->width();
        const int draw_height = p_image_label_bg->height();

        float rescale_w = (float)draw_width / (float)image.cols ;
        float rescale_h = (float)draw_height / (float)image.rows ;

        float min_rescale = std::fmin(rescale_w, rescale_h) ;
        if( min_rescale < 1.0 )
        {
            cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
        }

        //fit image label by image isze
        int pos_x = (int)((float)p_image_label_bg->x() + (float)(draw_width - image.cols)/2.0) ;
        int pos_y = (int)((float)p_image_label_bg->y() + (float)(draw_height - image.rows)/2.0) ;

        p_image_label->setGeometry(pos_x, pos_y, image.cols, image.rows);

		CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, p_image_label->width(), p_image_label->height()) ;

		//Set Rect
		float auto_focus_area_x = 0 ;
		float auto_focus_area_y = 0 ;
		float auto_focus_area_width = 0 ;
		float auto_focus_area_height = 0 ;
		Ensemble_Camera_Get_Auto_Focus_Area(GetId(), &auto_focus_area_x, &auto_focus_area_y, &auto_focus_area_width, &auto_focus_area_height ) ;

        int i_auto_focus_area_x = auto_focus_area_x * image.cols ;
		int i_auto_focus_area_y = auto_focus_area_y * image.rows ;
		int i_auto_focus_area_width = auto_focus_area_width * image.cols ;
		int i_auto_focus_area_height = auto_focus_area_height * image.rows ;
		if( i_auto_focus_area_width > 0 && i_auto_focus_area_height > 0 )
		{
			QPainter qPainter(&qt_display_image);
			qPainter.setBrush(Qt::NoBrush);
			qPainter.setPen(Qt::green);
			
			qPainter.drawRect(i_auto_focus_area_x,i_auto_focus_area_y,i_auto_focus_area_width,i_auto_focus_area_height);

			bool bEnd = qPainter.end();
		}
		
		if( !m_rect_user.empty() )
		{
			if( m_rect_user.width > 0 && m_rect_user.height > 0 )
			{
				QPainter qPainter(&qt_display_image);
				qPainter.setBrush(Qt::NoBrush);
				qPainter.setPen(Qt::red);
				
				qPainter.drawRect(m_rect_user.x,m_rect_user.y,m_rect_user.width,m_rect_user.height);

				bool bEnd = qPainter.end();
			}
		}
			
        p_image_label->setPixmap(QPixmap::fromImage(qt_display_image));
    }
}

void DialogBaseCameraConfig::OnButtonSetAutoFocusSelectArea(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_AREA) ;
}

void DialogBaseCameraConfig::OnButtonSetAutoFocusAllArea(void)
{
	//All 
	//Set Focus Area
    int ret = Ensemble_Camera_Set_Auto_Focus(GetId(), 0, 0, -1, -1) ;
}

void DialogBaseCameraConfig::mousePressEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if (event->button() == Qt::LeftButton && m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		m_rect_user = m_cls_set_user_region.StartSetRegion(point.x(), point.y()) ;

		//updatePicture(m_image, rect_user) ;
    }
}

void DialogBaseCameraConfig::mouseMoveEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if ( m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL)
	{
		QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		if( (event->buttons() & Qt::LeftButton) )
		{
			m_rect_user = m_cls_set_user_region.MoveSetRegion(point.x(), point.y()) ;

			//updatePicture(m_image, rect_user) ;
		}
	}
}

void DialogBaseCameraConfig::mouseReleaseEvent(QMouseEvent *event)
{
    //if (event->button() == Qt::LeftButton && scribbling) {
    //    drawLineTo(event->pos());
    //    scribbling = false;
    //}

	//Set
	int set_status = m_cls_set_user_region.GetStatus() ;

	qDebug("%s - %d : m_set_status(%d), event->buttons()=%d", __func__, __LINE__, set_status, event->buttons()) ;
	
    if (set_status > SetBaseStatus::NORMAL)
	{
		qDebug("%s - %d", __func__, __LINE__) ;
		
		float f_x = 0.0 ;
		float f_y = 0.0 ;
		float f_w = 0.0 ;
		float f_h = 0.0 ;

        int label_w = ui->label_image->width() ;
        int label_h = ui->label_image->height() ;

		m_rect_user = m_cls_set_user_region.EndSetRegion() ;
	
        f_x = (float)m_rect_user.x / (float)label_w ;
        f_y = (float)m_rect_user.y / (float)label_h ;
        f_w = (float)m_rect_user.width / (float)label_w ;
        f_h = (float)m_rect_user.height / (float)label_h ;

		qDebug("%s - %d : m_set_status(%d)", __func__, __LINE__, set_status) ;
		
        if( set_status == SetBaseStatus::SET_AREA )
        {
			//Set Focus Area
            int ret = Ensemble_Camera_Set_Auto_Focus(GetId(), f_x, f_y, f_w, f_h) ;
        }

		m_rect_user = cv::Rect() ;
	}
}

