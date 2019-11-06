#include "dialogsetcalibration.h"
#include "ui_dialogsetcalibration.h"

DialogSetCalibration::DialogSetCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCalibration)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_chess_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetChessInfo())) ;
	connect(ui->pushButton_get_calibration_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetCalibrationImage())) ;

	connect(ui->pushButton_add, SIGNAL(clicked()), this,  SLOT(OnButtonAddCalibrationInfo())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDelCalibrationInfo())) ;
	connect(ui->pushButton_clear, SIGNAL(clicked()), this,  SLOT(OnButtonClearCalibrationInfo())) ;
	connect(ui->pushButton_update, SIGNAL(clicked()), this,	SLOT(OnButtonUpdateCalibrationInfo())) ;	

	connect(ui->pushButton_calibration_run, SIGNAL(clicked()), this, SLOT(OnButtonCalibrationRun())) ;	

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

	//list update
	connect(ui->pushButton_calibration_list_update, SIGNAL(clicked()), this, SLOT(OnButtonCalibrationCopyListUpdate())) ;		
	
    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");

	m_calibration_copy_list_model = new QStringListModel(this); ;
}

DialogSetCalibration::~DialogSetCalibration()
{
	if( m_calibration_copy_list_model != NULL )
    {
        delete m_calibration_copy_list_model ;
        m_calibration_copy_list_model = NULL ;
    }
	
    delete ui;
}

void DialogSetCalibration::SetId(const std::string id)
{
    m_str_id = id ;

    ui->label_id_parent->setText(QString::fromUtf8(m_str_id.c_str()));
}

std::string DialogSetCalibration::GetId(void)
{
    return m_str_id ;
}

void DialogSetCalibration::showEvent(QShowEvent *ev) 
{
    QDialog::showEvent(ev) ;

	//Get Calibration Image
	OnButtonGetCalibrationImage() ;
	
	//Get Chesboard information
	OnButtonGetChessInfo() ;

	//Update Information List
	OnButtonUpdateCalibrationInfo() ;

	//Update Camera Information
	//exposure min/max value
	const int exposure_min = Ensemble_Job_Camera_Get_Min_Exposure_Value(GetId()) ;
	const int exposure_max = Ensemble_Job_Camera_Get_Max_Exposure_Value(GetId()) ;

	//gain min/max value
	const int gain_min = Ensemble_Job_Camera_Get_Min_Gain_Value(GetId()) ;
	const int gain_max = Ensemble_Job_Camera_Get_Max_Gain_Value(GetId()) ;

	//focus min/max value
	const int focus_min = Ensemble_Job_Camera_Get_Min_Focus_Value(GetId()) ;
	const int focus_max = Ensemble_Job_Camera_Get_Max_Focus_Value(GetId()) ;

	//brightness min/max value
	const int brightness_min = Ensemble_Job_Camera_Get_Min_Brightness_Value(GetId()) ;
	const int brightness_max = Ensemble_Job_Camera_Get_Max_Brightness_Value(GetId()) ;

	//sharpness min/max value
	const int sharpness_min = Ensemble_Job_Camera_Get_Min_Sharpness_Value(GetId()) ;
	const int sharpness_max = Ensemble_Job_Camera_Get_Max_Sharpness_Value(GetId()) ;

	//contrast min/max value
	const int contrast_min = Ensemble_Job_Camera_Get_Min_Contrast_Value(GetId()) ;
	const int contrast_max = Ensemble_Job_Camera_Get_Max_Contrast_Value(GetId()) ;

	//iso min/max value
	const int iso_min = Ensemble_Job_Camera_Get_Min_ISO_Value(GetId()) ;
	const int iso_max = Ensemble_Job_Camera_Get_Max_ISO_Value(GetId()) ;

	//shutter speed min/max value
	const int shutter_speed_min = Ensemble_Job_Camera_Get_Min_Shutter_Speed_Value(GetId()) ;
	const int shutter_speed_max = Ensemble_Job_Camera_Get_Max_Shutter_Speed_Value(GetId()) ;

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
	int check_auto_exposure = Ensemble_Job_Camera_Get_Auto_Exposure_OnOff(GetId()) ;
	if( check_auto_exposure )	ui->checkBox_auto_exposure->setChecked(true) ;
	else						ui->checkBox_auto_exposure->setChecked(false) ;

	int check_auto_focus = Ensemble_Job_Camera_Get_Auto_Focus_OnOff(GetId()) ;
	if( check_auto_focus )		ui->checkBox_auto_focus->setChecked(true) ;
	else						ui->checkBox_auto_focus->setChecked(false) ;

	//
	OnButtonCalibrationCopyListUpdate() ;
}

void DialogSetCalibration::OnButtonGetChessInfo(void)
{
	int chess_x_num = 0 ;
	int chess_y_num = 0 ;
	int chess_square_mm_size = 0 ;

	Ensemble_Job_Calibration_Get_Chess_Info(GetId(), &chess_x_num, &chess_y_num, &chess_square_mm_size);

	ui->lineEdit_chess_x_num->setText(QString::number(chess_x_num));
	ui->lineEdit_chess_y_num->setText(QString::number(chess_y_num));
	ui->lineEdit_chess_size->setText(QString::number(chess_square_mm_size));
}

void DialogSetCalibration::OnButtonGetCalibrationImage(void)
{
	cv::Mat calib_image = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display

	unsigned char* get_data = NULL ;
    int width = 640 ;
    int height = 480 ;

	const int image_type = IMAGE_RGB888 ;
	
    Ensemble_Source_Get_Image(GET_IMAGE_CALIBRATION_FEATURE, GetId(), image_type, &get_data, &width, &height) ;

    if( get_data != NULL )
    {
        if( width>0 && height >0 )
        {
			cv::Mat get_image(height, width, CV_8UC3, get_data) ;
			cv::cvtColor(get_image, calib_image, cv::COLOR_BGR2RGB) ;
        }
    	
        delete [] get_data ;
        get_data = NULL ;
    }

    const int draw_width = ui->label_image_bg->width();
    const int draw_height = ui->label_image_bg->height();

    float rescale_w = (float)draw_width / (float)calib_image.cols ;
    float rescale_h = (float)draw_height / (float)calib_image.rows ;

    float min_rescale = std::fmin(rescale_w, rescale_h) ;
    if( min_rescale < 1.0 )
    {
        cv::resize(calib_image, calib_image, cv::Size(), min_rescale, min_rescale) ;
    }

    //fit image label by image isze
    int pos_x = (int)((float)ui->label_image_bg->x() + (float)(draw_width - calib_image.cols)/2.0) ;
    int pos_y = (int)((float)ui->label_image_bg->y() + (float)(draw_height - calib_image.rows)/2.0) ;

    ui->label_image->setGeometry(pos_x, pos_y, calib_image.cols, calib_image.rows);

	//QImage qt_display_image = QImage((const unsigned char*)calib_image.data, calib_image.cols, calib_image.rows, QImage::Format_RGB888);
	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(calib_image, ui->label_image->width(), ui->label_image->height()) ;
	
	ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetCalibration::OnButtonAddCalibrationInfo(void)
{
	float f_robot_x = ui->lineEdit_robot_x->text().toFloat();
	float f_robot_y = ui->lineEdit_robot_y->text().toFloat();
	
	Ensemble_Job_Calibration_Add(GetId(), f_robot_x, f_robot_y);

	OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonDelCalibrationInfo(void)
{
	int select_index = ui->listWidget_calibration->currentRow() ;

    Ensemble_Job_Calibration_Del(GetId(), select_index);

    OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonClearCalibrationInfo(void)
{
    Ensemble_Job_Calibration_Clear(GetId());

	OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonUpdateCalibrationInfo(void)
{
	ui->listWidget_calibration->clear();
	
	//calibration image list
	int nCalibrationInfo = Ensemble_Job_Calibration_GetCount(GetId());

    qDebug("Calibration count = %d", nCalibrationInfo) ;

	for (int i = 0; i < nCalibrationInfo; i++)
	{
        unsigned char* get_data = NULL ;
        int width = 640 ;
        int height = 480 ;
		const int image_type = IMAGE_RGB888 ;
		
		Ensemble_Job_Calibration_GetImage(GetId(), i, image_type, &get_data, &width, &height) ;

		cv::Mat calibration_image ;
        if( get_data != NULL )
        {
            if( width>0 && height >0 )
            {
                cv::Mat get_image(height, width, CV_8UC3, get_data) ;
                cv::cvtColor(get_image, calibration_image, cv::COLOR_BGR2RGB) ;
            }

            delete [] get_data ;
            get_data = NULL ;
        }

		float robot_x = 0.0, robot_y = 0.0;
		Ensemble_Job_Calibration_GetRobotInfo(GetId(), i, &robot_x, &robot_y);

		cv::Mat image;
		cv::resize(calibration_image, image, cv::Size(120, 90));

		//calibration_image.release() ;

		QImage qt_calibration_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);

        std::string str ;
        str = "Pos(" + std::to_string(robot_x) + "," + std::to_string(robot_y) + ")" ;
		//Add Image List
        ui->listWidget_calibration->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(qt_calibration_image)), str.c_str()));
	}
}

void DialogSetCalibration::OnButtonCalibrationRun(void)
{
	Ensemble_Job_Calibration_Run(GetId());

	UpdateDataCalibrationRun(0.5, 0.5);
}

void DialogSetCalibration::UpdateDataCalibrationRun(float f_pixel_x, float f_pixel_y)
{
	//check calibration result
	float robot_x = 0;
	float robot_y = 0;
	Ensemble_Job_Calibration_GetPoint(GetId(), f_pixel_x, f_pixel_y, &robot_x, &robot_y);

	std::string str_info ;
	char cstr_info[255] ;
	sprintf(cstr_info, "(%d,%d)pixel -> (%.2f,%.2f)mm", (int)(640.0*f_pixel_x), (int)(480.0*f_pixel_y), robot_x, robot_y) ;
	str_info = cstr_info ;
	ui->label_calibration_info->setText(QString::fromUtf8(str_info.c_str()));
}

void DialogSetCalibration::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		int label_w = ui->label_image->width() ;
        int label_h = ui->label_image->height() ;

       	float pixel_x = (float)point.x() / (float)label_w ;
        float pixel_y = (float)point.y() / (float)label_h ;

		UpdateDataCalibrationRun(pixel_x, pixel_y) ;
    }
}

void DialogSetCalibration::OnButtonExposureGet(void)
{
	int exposure_value = Ensemble_Job_Camera_Get_Manual_Exposure_Value(GetId()) ;

	ui->lineEdit_exposure->setText(QString::number(exposure_value));

	ui->horizontalSlider_exposure->setValue(exposure_value) ;
}

void DialogSetCalibration::OnButtonGainGet(void)
{
	int gain_value = Ensemble_Job_Camera_Get_Manual_Gain_Value(GetId()) ;

	ui->lineEdit_gain->setText(QString::number(gain_value));

	ui->horizontalSlider_gain->setValue(gain_value) ;
}

void DialogSetCalibration::OnButtonFocusGet(void)
{
	int focus_value = Ensemble_Job_Camera_Get_Manual_Focus_Value(GetId()) ;

	ui->lineEdit_focus->setText(QString::number(focus_value));

	ui->horizontalSlider_focus->setValue(focus_value) ;
}

void DialogSetCalibration::OnButtonBrightnessGet(void)
{
	int brightness_value = Ensemble_Job_Camera_Get_Manual_Brightness_Value(GetId()) ;

	ui->lineEdit_brightness->setText(QString::number(brightness_value));

	ui->horizontalSlider_brightness->setValue(brightness_value) ;
}

void DialogSetCalibration::OnButtonSharpnessGet(void)
{
	int sharpness_value = Ensemble_Job_Camera_Get_Manual_Sharpness_Value(GetId()) ;

	ui->lineEdit_sharpness->setText(QString::number(sharpness_value));

	ui->horizontalSlider_sharpness->setValue(sharpness_value) ;
}

void DialogSetCalibration::OnButtonContrastGet(void)
{
	int contrast_value = Ensemble_Job_Camera_Get_Manual_Contrast_Value(GetId()) ;

	ui->lineEdit_contrast->setText(QString::number(contrast_value));

	ui->horizontalSlider_contrast->setValue(contrast_value) ;
}

void DialogSetCalibration::OnButtonISOGet(void)
{
	int iso_value = Ensemble_Job_Camera_Get_Manual_ISO_Value(GetId()) ;

	ui->lineEdit_iso->setText(QString::number(iso_value));

	ui->horizontalSlider_iso->setValue(iso_value) ;
}

void DialogSetCalibration::OnButtonShutterSpeedGet(void)
{
	int shutter_speed_value = Ensemble_Job_Camera_Get_Manual_Shutter_Speed_Value(GetId()) ;

	ui->lineEdit_shutter_speed->setText(QString::number(shutter_speed_value));

	ui->horizontalSlider_shutter_speed->setValue(shutter_speed_value) ;
}

void DialogSetCalibration::OnButtonExposureSet(void)
{
    int value = ui->lineEdit_exposure->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Exposure_Value(GetId(), value) ;
	
	OnButtonExposureGet() ;
}

void DialogSetCalibration::OnButtonGainSet(void)
{
    int value = ui->lineEdit_gain->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Gain_Value(GetId(), value) ;

	OnButtonGainGet() ;
}

void DialogSetCalibration::OnButtonFocusSet(void)
{
    int value = ui->lineEdit_focus->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Focus_Value(GetId(), value) ;

	OnButtonFocusGet() ;
}

void DialogSetCalibration::OnButtonBrightnessSet(void)
{
	int value = ui->lineEdit_brightness->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Brightness_Value(GetId(), value) ;

	OnButtonBrightnessGet() ;
}

void DialogSetCalibration::OnButtonSharpnessSet(void)
{
	int value = ui->lineEdit_sharpness->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Sharpness_Value(GetId(), value) ;

	OnButtonSharpnessGet() ;
}

void DialogSetCalibration::OnButtonContrastSet(void)
{
	int value = ui->lineEdit_contrast->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Contrast_Value(GetId(), value) ;

	OnButtonContrastGet() ;
}

void DialogSetCalibration::OnButtonISOSet(void)
{
	int value = ui->lineEdit_iso->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_ISO_Value(GetId(), value) ;

	OnButtonISOGet() ;
}

void DialogSetCalibration::OnButtonShutterSpeedSet(void)
{
	int value = ui->lineEdit_shutter_speed->text().toInt() ;
    Ensemble_Job_Camera_Set_Manual_Shutter_Speed_Value(GetId(), value) ;

	OnButtonShutterSpeedGet() ;
}

void DialogSetCalibration::OnSliderSetExposure(void)
{
	//get value form slider
    int value = ui->horizontalSlider_exposure->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Exposure_Value(GetId(), value);

	OnButtonExposureGet() ;
}

void DialogSetCalibration::OnSliderSetGain(void) 
{
	//get value form slider
    int value = ui->horizontalSlider_gain->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Gain_Value(GetId(), value);

	OnButtonGainGet() ;
}


void DialogSetCalibration::OnSliderSetFocus(void) 
{
	//get value form slider
    int value = ui->horizontalSlider_focus->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Focus_Value(GetId(), value);

	OnButtonFocusGet() ;
}

void DialogSetCalibration::OnSliderSetBrightness(void)
{
	//get value form slider
    int value = ui->horizontalSlider_brightness->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Brightness_Value(GetId(), value);

	OnButtonBrightnessGet() ;
}

void DialogSetCalibration::OnSliderSetSharpness(void)
{
	//get value form slider
    int value = ui->horizontalSlider_sharpness->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Sharpness_Value(GetId(), value);

	OnButtonSharpnessGet() ;
}

void DialogSetCalibration::OnSliderSetContrast(void)
{
	//get value form slider
    int value = ui->horizontalSlider_contrast->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Contrast_Value(GetId(), value);

	OnButtonContrastGet() ;
}

void DialogSetCalibration::OnSliderSetISO(void)
{
	//get value form slider
    int value = ui->horizontalSlider_iso->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_ISO_Value(GetId(), value);

	OnButtonISOGet() ;
}

void DialogSetCalibration::OnSliderSetShutterSpeed(void)
{
	//get value form slider
    int value = ui->horizontalSlider_shutter_speed->value() ;

	//set value
	Ensemble_Job_Camera_Set_Manual_Shutter_Speed_Value(GetId(), value);

	OnButtonShutterSpeedGet() ;
}

void DialogSetCalibration::OnButtonCalibrationCopyListUpdate(void)
{
	//clear list data
    ui->listView_list_calibration_copy->reset();
	
	//Get Parent Tree Info.
	std::string str_xml = Ensemble_Task_Get_Parent_Tree(GetId()) ;
	
	//XML Parsing and Find Project ID string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_xml.c_str()));

	std::string str_projct_id ;
	
    if (!result)
    {
        qDebug("DialogSetCalibration::OnButtonCalibrationCopyListUpdate - xml parsing error") ;
    }
    else
    {
        for (pugi::xml_node project: doc.child("Root").children("Project"))
        {
        	std::string str_prj_id = project.attribute("ID").value();
			
			for (pugi::xml_node me: project.children("Me"))
        	{
				str_projct_id = str_prj_id ;
				break ;
    	    }	
        }
    }

	if( !str_projct_id.empty() )
	{
		std::string str_xml_has_job_info = Ensemble_Project_Get_Has_Job_Info(str_projct_id) ;
	
		//XML Parsing and Find Project ID string
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string((char *)(str_xml_has_job_info.c_str()));

		if (!result)
	    {
	        qDebug("DialogSetCalibration::OnButtonCalibrationCopyListUpdate - str_xml_has_job_info parsing error") ;
	    }
	    else
	    {
	    	QStringList stringListSource;
			
			for (pugi::xml_node job: doc.child("Jobs").children("Job"))
			{
				std::string str_job_id = job.attribute("ID").value();

				if( Ensemble_Job_Calibration_isOK(str_job_id) )		//check calibration 
				{
                    stringListSource << str_job_id.c_str() ;
				}
			}
		
	        // Create model
	        m_calibration_copy_list_model->setStringList(stringListSource);

	        // Glue model and view together
	        ui->listView_list_calibration_copy->setModel(m_calibration_copy_list_model);
	    }
	}
	
}
