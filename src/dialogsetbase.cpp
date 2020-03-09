#include "dialogsetbase.h"
#include "ui_dialogsetbase.h"

DialogSetBase::DialogSetBase(QWidget *parent) :
   QDialog(parent),
    ui(new Ui::DialogSetBase)
{
    ui->setupUi(this);

    //button
    connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_set_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonSetImage())) ;
    connect(ui->pushButton_set_detect_area, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectArea())) ;
    connect(ui->pushButton_clear_detect_area, SIGNAL(clicked()), this,  SLOT(OnButtonClearDetectArea())) ;
    connect(ui->pushButton_zoom_set, SIGNAL(clicked()), this,  SLOT(OnButtonZoomSet())) ;
    connect(ui->pushButton_zoom_reset, SIGNAL(clicked()), this,  SLOT(OnButtonZoomReset())) ;
    connect(ui->pushButton_name_change, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//object button
	connect(ui->pushButton_select_object, SIGNAL(clicked()), this,  SLOT(OnButtonSelectObject())) ;
	connect(ui->pushButton_reset_object, SIGNAL(clicked()), this,  SLOT(OnButtonResetObject())) ;

	//erase
	connect(ui->pushButton_select_erase, SIGNAL(clicked()), this,  SLOT(OnButtonSelectErase())) ;
	connect(ui->pushButton_reset_erase, SIGNAL(clicked()), this,  SLOT(OnButtonResetErase())) ;
			
	//reference point
	connect(ui->pushButton_ref_point_select, SIGNAL(clicked()), this,  SLOT(OnButtonSelectRefPoint())) ;
	connect(ui->pushButton_ref_point_reset, SIGNAL(clicked()), this,  SLOT(OnButtonResetRefPoint())) ;
	
    //mask
    connect(ui->pushButton_mask_push, SIGNAL(clicked()), this,  SLOT(OnButtonMaskPush())) ;
    connect(ui->pushButton_mask_pop, SIGNAL(clicked()), this,  SLOT(OnButtonMaskPop())) ;
    connect(ui->pushButton_mask_clear, SIGNAL(clicked()), this,  SLOT(OnButtonMaskClear())) ;

	//slider
    connect(ui->horizontalSlider_feature_level, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetFeatureLevel()));
    connect(ui->horizontalSlider_feature_level, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMove(int)));

    //detect option button
    connect(ui->pushButton_detect_margin_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectOptionMargin())) ;
    connect(ui->pushButton_detect_threshold_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectOptionThreshold())) ;
    connect(ui->pushButton_detect_count_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectOptionCount())) ;
    connect(ui->pushButton_detect_margin_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetDetectOptionMargin())) ;
    connect(ui->pushButton_detect_threshold_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetDetectOptionThreshold())) ;
    connect(ui->pushButton_detect_count_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetDetectOptionCount())) ;
	//constraint angle
	connect(ui->pushButton_constraint_angle_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetConstraintAngle())) ;
	connect(ui->pushButton_constraint_angle_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetConstraintAngle())) ;

    //feature (custom) check box
    connect(ui->checkBox_use_custom_feature, SIGNAL(clicked(bool)), this,  SLOT(OnCheckFeatureUseCustomOption(bool))) ;

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetBase::~DialogSetBase()
{ 	
    delete ui;
}

void DialogSetBase::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string base_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(base_name.c_str()));

	//use custom
	int use_custom_feature = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_UseCustomFeatureOption(GetId()) ;
	if( use_custom_feature )
	{
		//checked and disable level ctrl.	
		ui->checkBox_use_custom_feature->setChecked(1);

        ui->horizontalSlider_feature_level->setEnabled(false) ;
	}
	else
	{
		//un-checked
		ui->checkBox_use_custom_feature->setChecked(0);

        ui->horizontalSlider_feature_level->setEnabled(true) ;
	}
	
	
	//Get Level 
	int feature_level = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_FeatureLevel(GetId());
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(feature_level) ;
	ui->label_feature_level->setText(QString::number(feature_level));

	//Get Detect Option Value
    OnButtonGetDetectOptionMargin() ;
    OnButtonGetDetectOptionThreshold(); ;
    OnButtonGetDetectOptionCount() ;
	OnButtonGetConstraintAngle() ;
	
#if 0
	int detect_option_margin = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_MARGIN) ;
	float detect_option_threshold = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_THRESHOLD) ;
	int detect_option_count = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_COUNT) ;
    ui->lineEdit_detect_margin->setText(QString::number(detect_option_margin)) ;
    ui->lineEdit_detect_threshold->setText(QString("%2").arg(detect_option_threshold)) ;
    ui->lineEdit_detect_count->setText(QString::number(detect_option_count)) ;

    qDebug("Detect Option : Margin = %d", detect_option_margin) ;
    qDebug("Detect Option : Threshold = %f", detect_option_threshold) ;
    qDebug("Detect Option : Count = %d", detect_option_count) ;
#endif

    OnButtonGetImage() ;	
}

void DialogSetBase::updatePicture(cv::Mat image, cv::Rect rect_user) 
{
    const int draw_width = ui->label_image_bg->width();
    const int draw_height = ui->label_image_bg->height();

    float rescale_w = (float)draw_width / (float)image.cols ;
    float rescale_h = (float)draw_height / (float)image.rows ;

    float min_rescale = std::fmin(rescale_w, rescale_h) ;
    if( min_rescale < 1.0 )
    {
        cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
    }

    //fit image label by image isze
    int pos_x = (int)((float)ui->label_image_bg->x() + (float)(draw_width - image.cols)/2.0) ;
    int pos_y = (int)((float)ui->label_image_bg->y() + (float)(draw_height - image.rows)/2.0) ;

    ui->label_image->setGeometry(pos_x, pos_y, image.cols, image.rows);

    //QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);
    CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;

    if( !(rect_user.width <= 0 || rect_user.height <= 0) )
	{
		//draw set rect
	    qDebug("%s : rect(%d,%d,%d,%d", __func__, rect_user.x, rect_user.y, rect_user.width, rect_user.height) ;

		if( rect_user.width > 0 && rect_user.height > 0 )
		{
			QPainter qPainter(&qt_display_image);
			qPainter.setBrush(Qt::NoBrush);
			qPainter.setPen(Qt::red);
			
			qPainter.drawRect(rect_user.x,rect_user.y,rect_user.width,rect_user.height);

			bool bEnd = qPainter.end();
		}
	}

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetBase::updatePictureCenterLine(cv::Mat image, cv::Point pt_user)
{
	const int draw_width = ui->label_image_bg->width();
    const int draw_height = ui->label_image_bg->height();

    float rescale_w = (float)draw_width / (float)image.cols ;
    float rescale_h = (float)draw_height / (float)image.rows ;

    float min_rescale = std::fmin(rescale_w, rescale_h) ;
    if( min_rescale < 1.0 )
    {
        cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
    }

    //fit image label by image isze
    int pos_x = (int)((float)ui->label_image_bg->x() + (float)(draw_width - image.cols)/2.0) ;
    int pos_y = (int)((float)ui->label_image_bg->y() + (float)(draw_height - image.rows)/2.0) ;

    ui->label_image->setGeometry(pos_x, pos_y, image.cols, image.rows);

    //QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);
    CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;

    //draw set rect
    qDebug("%s : point(%d,%d)", __func__, pt_user.x, pt_user.y) ;

    if( pt_user.x > 0 && pt_user.y > 0 )
    {
        QPainter qPainter(&qt_display_image);
        qPainter.setBrush(Qt::NoBrush);
        qPainter.setPen(Qt::red);

        qPainter.drawLine(pt_user.x,0,pt_user.x,image.rows);
        qPainter.drawLine(0,pt_user.y,image.cols,pt_user.y);

        bool bEnd = qPainter.end();
    }

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetBase::SetId(const std::string id)
{
    m_str_id = id ;

    ui->label_id->setText(QString::fromUtf8(m_str_id.c_str()));
}

std::string DialogSetBase::GetId(void)
{
    return m_str_id ;
}

void DialogSetBase::OnButtonGetImage(void)
{
    //Get Base Job Image
    unsigned char* get_job_image_data = NULL ;
    int job_image_width = 0 ;
    int job_image_height = 0 ;

	const int image_type = ImageTypeOption::IMAGE_RGB888 ;
    int get_image_type = 0 ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_Image(GetId(), image_type, &get_job_image_data, &job_image_width, &job_image_height, &get_image_type)  ;

    if( job_image_width > 0 && job_image_height > 0 )
    {
        if( get_image_type == ImageTypeOption::IMAGE_YUV420 )
    	{
	        //YUV420
	        cv::Mat get_image(job_image_height + job_image_height / 2, job_image_width, CV_8UC1, get_job_image_data) ;

	        CImgDec cls_image_decoder ;
	        m_image = cls_image_decoder.Decoding(get_image) ;
    	}
        else if( get_image_type == ImageTypeOption::IMAGE_RGB888 )
		{
			cv::Mat get_image(job_image_height, job_image_width, CV_8UC3, get_job_image_data) ;
			//get_image.copyTo(m_image) ;
			cv::cvtColor(get_image, m_image, cv::COLOR_BGR2RGB) ;
		}
        else if( get_image_type == ImageTypeOption::IMAGE_JPG)
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, job_image_width*job_image_height, CV_8UC1, get_job_image_data), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, m_image, cv::COLOR_BGR2RGB) ;
        }

        updatePicture(m_image) ;
    }

    if( get_job_image_data != NULL )
    {
        delete [] get_job_image_data ;
        get_job_image_data = NULL ;
    }
}

void DialogSetBase::OnButtonSetImage(void)
{
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_Image(GetId())  ;
	
    OnButtonGetImage() ;

	emit UpdateBaseImage();
}

void DialogSetBase::OnButtonSetDetectArea(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_AREA) ;
}

void DialogSetBase::OnButtonClearDetectArea(void)
{
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectArea(GetId(), 0, 0, 0, 0) ;

    OnButtonGetImage() ;

    emit UpdateBaseImage();
}

void DialogSetBase::OnButtonZoomSet(void)
{
 	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_ZOOM) ;
}

void DialogSetBase::OnButtonZoomReset(void)
{
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_Zoom(GetId(), 0, 0, 0, 0) ;       //reset

    OnButtonGetImage() ;
}

void DialogSetBase::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetBase::OnButtonResetObject(void)
{
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;
}

void DialogSetBase::OnButtonSelectRefPoint(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_REF_POINT) ;
}

void DialogSetBase::OnButtonResetRefPoint(void)
{
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Del_Ref_Point(GetId()) ;
	
    OnButtonGetImage() ;
}


void DialogSetBase::OnButtonNameChange(void)
{
    std::string base_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_Name(GetId()) ;

    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetId());
    dlg_change_name.SetName(base_name);

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        std::string change_name = dlg_change_name.GetName() ;

        if( !change_name.empty() )
        {
            CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_Name(GetId(), change_name) ;
        }

        base_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(base_name.c_str()));

		emit UpdateBaseName(QString::fromUtf8(base_name.c_str())) ;
    }
}

void DialogSetBase::OnButtonMaskPush(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_MASK) ;
}

void DialogSetBase::OnButtonMaskPop(void)
{
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Undo_MaskArea(GetId()) ;

	OnButtonGetImage() ;

	emit UpdateBaseImage();
}

void DialogSetBase::OnButtonMaskClear(void)
{
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Del_MaskArea(GetId()) ;

	OnButtonGetImage() ;

	emit UpdateBaseImage();
}

//void DialogSetBase::OnSliderSetFeatureLevel(int level)
void DialogSetBase::OnSliderSetFeatureLevel(void)
{
    //get level
    int level = ui->horizontalSlider_feature_level->value() ;

	//set level
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_FeatureLevel(GetId(), level);

	//Update Image
	OnButtonGetImage() ;
	
	//Get Level 
	int feature_level = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_FeatureLevel(GetId());
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(feature_level) ;
	ui->label_feature_level->setText(QString::number(feature_level));

	emit UpdateBaseImage();
}

void DialogSetBase::OnSliderMove(int value)
{
    ui->label_feature_level->setText(QString::number(value));
}

#if 1
void DialogSetBase::mousePressEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if (event->button() == Qt::LeftButton && m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		cv::Rect rect_user = m_cls_set_user_region.StartSetRegion(point.x(), point.y()) ;

		updatePicture(m_image, rect_user) ;
    }
}

void DialogSetBase::mouseMoveEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if ( m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL)
	{
		QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		updatePictureCenterLine(m_image, cv::Point(point.x(), point.y())) ;
			
		if( (event->buttons() & Qt::LeftButton) )
		{
			cv::Rect rect_user = m_cls_set_user_region.MoveSetRegion(point.x(), point.y()) ;

			updatePicture(m_image, rect_user) ;
		}
	}
}

void DialogSetBase::mouseReleaseEvent(QMouseEvent *event)
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

		cv::Rect rect_user = m_cls_set_user_region.EndSetRegion() ;
	
        f_x = (float)rect_user.x / (float)label_w ;
        f_y = (float)rect_user.y / (float)label_h ;
        f_w = (float)rect_user.width / (float)label_w ;
        f_h = (float)rect_user.height / (float)label_h ;

		qDebug("%s - %d : m_set_status(%d)", __func__, __LINE__, set_status) ;
		
        if( set_status == SetBaseStatus::SET_AREA )
        {
            CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectArea(GetId(), f_x, f_y, f_w, f_h) ;

            emit UpdateBaseImage();
        }
        else if( set_status == SetBaseStatus::SET_ZOOM)
        {
            CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_Zoom(GetId(), f_x, f_y, f_w, f_h) ;
        }
		else if( set_status == SetBaseStatus::SET_MASK)
        {
            bool b_enable_inside = false ;
            if (ui->checkBox_mask_enable_inside->isChecked())	b_enable_inside = false ;
            else												b_enable_inside = true ;
				
            CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			emit UpdateBaseImage();
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
			CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;

			emit UpdateBaseImage();
		}
		else if( set_status == SetBaseStatus::SET_REF_POINT)
		{
			QPoint point = event->pos() ;
	        point.setX(point.x() - ui->label_image->x());
	        point.setY(point.y() - ui->label_image->y());

			f_x = (float)point.x() / (float)label_w ;
        	f_y = (float)point.y() / (float)label_h ;
		
			//SelectObject
			//CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;
            CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_Ref_Point(GetId(), f_x, f_y) ;

			emit UpdateBaseImage();
		}
		else if( set_status == SetBaseStatus::SET_ERASE)
		{
			//SelectObject
			CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_Erase(GetId(), f_x, f_y, f_w, f_h) ;

			emit UpdateBaseImage();
		}
	
        OnButtonGetImage() ;
	}

	updatePicture(m_image) ;
}
#endif

void DialogSetBase::OnButtonSetDetectOptionMargin(void)
{
    //Set Detect Option Value
    QString text_value = ui->lineEdit_detect_margin->text() ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_MARGIN, text_value.toFloat()) ;

    OnButtonGetDetectOptionMargin() ;
}

void DialogSetBase::OnButtonSetDetectOptionThreshold(void)
{
    //Set Detect Option Value
    QString text_value = ui->lineEdit_detect_threshold->text() ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_THRESHOLD, text_value.toFloat()) ;

    OnButtonGetDetectOptionThreshold() ;
}

void DialogSetBase::OnButtonSetDetectOptionCount(void)
{
    //Set Detect Option Value
    QString text_value = ui->lineEdit_detect_count->text() ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_COUNT, text_value.toFloat()) ;

    OnButtonGetDetectOptionCount() ;
}

void DialogSetBase::OnButtonGetDetectOptionMargin(void)
{
    //Get Detect Option Value
    int detect_option_margin = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_MARGIN) ;
    ui->lineEdit_detect_margin->setText(QString::number(detect_option_margin)) ;
}

void DialogSetBase::OnButtonGetDetectOptionThreshold(void)
{
    //Get Detect Option Value
    float detect_option_threshold = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_THRESHOLD) ;
    ui->lineEdit_detect_threshold->setText(QString("%2").arg(detect_option_threshold)) ;
}

void DialogSetBase::OnButtonGetDetectOptionCount(void)
{
    //Get Detect Option Value
    int detect_option_count = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_COUNT) ;
    ui->lineEdit_detect_count->setText(QString::number(detect_option_count)) ;
}

void DialogSetBase::OnCheckFeatureUseCustomOption(bool checked)
{
    qDebug("Check = %d : Use Custom Feature", checked) ;

	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_UseCustomFeatureOption(GetId(), checked) ;
	int use_custom_feature = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_UseCustomFeatureOption(GetId()) ;
	
	if( use_custom_feature )
	{	
		//checked and disable level ctrl.	
		ui->checkBox_use_custom_feature->setChecked(1);

		//disable feature slider
        ui->horizontalSlider_feature_level->setEnabled(false) ;

		//open set dialog for custom value
		DialogSetCustomFeatureOption dlg_set_custom_feature_option ;
		dlg_set_custom_feature_option.SetId(GetId()) ;
	
	   int dialogCode = dlg_set_custom_feature_option.exec();
	   
	}
	else
	{
        ui->horizontalSlider_feature_level->setEnabled(true) ;
	}
}

void DialogSetBase::OnButtonGetConstraintAngle(void)
{
	//Get Detect Option Value
    int detect_option_constraint_angle_min = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MIN) ;
	int detect_option_constraint_angle_max = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MAX) ;
	
    ui->lineEdit_constraint_angle_min->setText(QString::number(detect_option_constraint_angle_min)) ;
	ui->lineEdit_constraint_angle_max->setText(QString::number(detect_option_constraint_angle_max)) ;
}

void DialogSetBase::OnButtonSetConstraintAngle(void)
{
	 //Set Detect Option Value
    QString qstr_detect_option_constraint_angle_min = ui->lineEdit_constraint_angle_min->text() ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MIN, qstr_detect_option_constraint_angle_min.toFloat()) ;

	QString qstr_detect_option_constraint_angle_max = ui->lineEdit_constraint_angle_max->text() ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MAX, qstr_detect_option_constraint_angle_max.toFloat()) ;

    OnButtonGetConstraintAngle() ;
}

void DialogSetBase::OnButtonSelectErase(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_ERASE) ;
}

void DialogSetBase::OnButtonResetErase(void)
{
	CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Del_Erase(GetId()) ;
}

