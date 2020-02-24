#include "dialogsettoolobject.h"
#include "ui_dialogsettoolobject.h"

DialogSetToolObject::DialogSetToolObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolObject)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
	connect(ui->pushButton_name_change, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//Object
	connect(ui->pushButton_select_object, SIGNAL(clicked()), this,  SLOT(OnButtonSelectObject())) ;
	connect(ui->pushButton_reset_object, SIGNAL(clicked()), this,  SLOT(OnButtonResetObject())) ;

	//reference point
	connect(ui->pushButton_ref_point_select, SIGNAL(clicked()), this,  SLOT(OnButtonSelectRefPoint())) ;
	connect(ui->pushButton_ref_point_reset, SIGNAL(clicked()), this,  SLOT(OnButtonResetRefPoint())) ;

	//mask
    connect(ui->pushButton_mask_push, SIGNAL(clicked()), this,  SLOT(OnButtonMaskPush())) ;
    connect(ui->pushButton_mask_pop, SIGNAL(clicked()), this,  SLOT(OnButtonMaskPop())) ;
    connect(ui->pushButton_mask_clear, SIGNAL(clicked()), this,  SLOT(OnButtonMaskClear())) ;

	//detect option button
    connect(ui->pushButton_detect_margin_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectOptionMargin())) ;
    connect(ui->pushButton_detect_threshold_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectOptionThreshold())) ;
    connect(ui->pushButton_detect_margin_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetDetectOptionMargin())) ;
    connect(ui->pushButton_detect_threshold_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetDetectOptionThreshold())) ;

	//inspect
	connect(ui->pushButton_detect_inspection_tolerance_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetDetectInspectionToleranceInfo())) ;
	connect(ui->pushButton_detect_inspection_tolerance_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetDetectInspectionToleranceInfo())) ;
	
	//constraint angle
	connect(ui->pushButton_constraint_angle_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetConstraintAngle())) ;
	connect(ui->pushButton_constraint_angle_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetConstraintAngle())) ;
	
	//slider
	connect(ui->horizontalSlider_feature_level, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetFeatureLevel()));
	connect(ui->horizontalSlider_feature_level, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMove(int)));

	//feature (custom) check box
    connect(ui->checkBox_use_custom_feature, SIGNAL(clicked(bool)), this,  SLOT(OnCheckFeatureUseCustomOption(bool))) ;
	
    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetToolObject::~DialogSetToolObject()
{
    delete ui;
}

void DialogSetToolObject::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolObject::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;
	
	//Get Level 
    int feature_level = Ensemble_Tool_Get_FeatureLevel(GetId());
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(feature_level) ;
	ui->label_feature_level->setText(QString::number(feature_level));

	//Get Base Level
	int base_feature_level = Ensemble_Job_Get_FeatureLevel(GetParentId());
	//Set Slider
	std::string str_base_feature_level ;
	str_base_feature_level = "(Base: " + std::to_string(base_feature_level) + ")" ;
	QString qstr_base_feature_level = QString::fromStdString(str_base_feature_level);
	ui->label_feature_level_base->setText(qstr_base_feature_level);

	int use_custom_feature = Ensemble_Tool_Get_UseCustomFeatureOption(GetId());
	if( use_custom_feature )
	{
		//checked and disable level ctrl.	
		ui->checkBox_use_custom_feature->setChecked(1);

		//disable feature slider
        ui->horizontalSlider_feature_level->setEnabled(false) ;
	}
	else
	{
		//checked and disable level ctrl.	
		ui->checkBox_use_custom_feature->setChecked(0);

		//disable feature slider
        ui->horizontalSlider_feature_level->setEnabled(true) ;
	}
	
	//Image
	OnButtonGetImage() ;

	//Get Detect Option Value
    OnButtonGetDetectOptionMargin() ;
    OnButtonGetDetectOptionThreshold(); ;
	OnButtonGetConstraintAngle() ;
	OnButtonGetDetectInspectionToleranceInfo() ;
}

void DialogSetToolObject::OnButtonNameChange(void)
{
    std::string tool_name = Ensemble_Tool_Get_Name(GetId()) ;

    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetId());
    dlg_change_name.SetName(tool_name);

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        std::string change_name = dlg_change_name.GetName() ;

        qDebug("Tool Change Name = %s", change_name.c_str()) ;
		
        if( !change_name.empty() )
        {
            Ensemble_Tool_Set_Name(GetId(), change_name) ;
        }

        tool_name = Ensemble_Tool_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}

void DialogSetToolObject::OnButtonGetImage(void)
{
    //Get Base Job Image
    unsigned char* get_job_image_data = NULL ;
    int job_image_width = 0 ;
    int job_image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
    int get_image_type = 0 ;
    Ensemble_Tool_Get_Image(GetId(), image_type, &get_job_image_data, &job_image_width, &job_image_height, &get_image_type)  ;

    if( job_image_width > 0 && job_image_height > 0 )
    {
        if( get_image_type == IMAGE_YUV420 )
    	{
	        //YUV420
	        cv::Mat get_image(job_image_height + job_image_height / 2, job_image_width, CV_8UC1, get_job_image_data) ;

	        CImgDec cls_image_decoder ;
	        m_image = cls_image_decoder.Decoding(get_image) ;
		}
        else if( get_image_type == IMAGE_RGB888 )
		{
			cv::Mat get_image(job_image_height, job_image_width, CV_8UC3, get_job_image_data) ;
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

void DialogSetToolObject::updatePicture(cv::Mat image, cv::Rect rect_user)
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
    if( !(rect_user.width <= 0 || rect_user.height <= 0) )
	{
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

void DialogSetToolObject::updatePictureCenterLine(cv::Mat image, cv::Point pt_user)
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

void DialogSetToolObject::OnSliderSetFeatureLevel(void)
{
    //get level
    int level = ui->horizontalSlider_feature_level->value() ;

	qDebug("%s : SetFeatureLevel = %d", __func__, level) ;
	//set level
    Ensemble_Tool_Set_FeatureLevel(GetId(), level);

	//Update Image
	qDebug("%s : GetImage", __func__) ;
	OnButtonGetImage() ;

	qDebug("%s : GetFeatureLevel", __func__) ;
	
	//Get Level 
    int feature_level = Ensemble_Tool_Get_FeatureLevel(GetId());

	qDebug("%s : GetFeatureLevel = %d", __func__, feature_level) ;
	
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(feature_level) ;
	ui->label_feature_level->setText(QString::number(feature_level));

	//Get Base Level
	int base_feature_level = Ensemble_Job_Get_FeatureLevel(GetParentId());
	//Set Slider
	std::string str_base_feature_level ;
	str_base_feature_level = "(Base: " + std::to_string(base_feature_level) + ")" ;
	QString qstr_base_feature_level = QString::fromStdString(str_base_feature_level);
	ui->label_feature_level_base->setText(qstr_base_feature_level);

	//emit UpdateToolObjectImage();
}

void DialogSetToolObject::OnSliderMove(int value)
{
    ui->label_feature_level->setText(QString::number(value));
}

void DialogSetToolObject::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetToolObject::OnButtonResetObject(void)
{
	//Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;
}

#if 1
void DialogSetToolObject::mousePressEvent(QMouseEvent *event)
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

void DialogSetToolObject::mouseMoveEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

   if ( m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL)
	{
		QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		if( m_cls_set_user_region.GetStatus() == SetBaseStatus::SET_REF_POINT )
		{
			updatePictureCenterLine(m_image, cv::Point(point.x(), point.y())) ;
		}
		else
		{
			if( (event->buttons() & Qt::LeftButton) )
			{
				cv::Rect rect_user = m_cls_set_user_region.MoveSetRegion(point.x(), point.y()) ;

				updatePicture(m_image, rect_user) ;
			}
		}
	}
}

void DialogSetToolObject::mouseReleaseEvent(QMouseEvent *event)
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
            //Ensemble_Job_Set_DetectArea(GetId(), f_x, f_y, f_w, f_h) ;

            //emit UpdateToolObjectImage();
        }
        else if( set_status == SetBaseStatus::SET_ZOOM)
        {
            //Ensemble_Job_Set_Zoom(GetId(), f_x, f_y, f_w, f_h) ;
        }
		else if( set_status == SetBaseStatus::SET_MASK)
        {
            bool b_enable_inside = false ;
            if (ui->checkBox_mask_enable_inside->isChecked())	b_enable_inside = false ;
            else												b_enable_inside = true ;
				
            Ensemble_Tool_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			emit UpdateToolObjectImage();
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
            Ensemble_Tool_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;

			emit UpdateToolObjectImage();
		}
		else if( set_status == SetBaseStatus::SET_REF_POINT)
		{
			QPoint point = event->pos() ;
	        point.setX(point.x() - ui->label_image->x());
	        point.setY(point.y() - ui->label_image->y());

			f_x = (float)point.x() / (float)label_w ;
        	f_y = (float)point.y() / (float)label_h ;
		
			//SelectObject
			//Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;
            Ensemble_Tool_Set_Ref_Point(GetId(), f_x, f_y) ;

            emit UpdateToolObjectImage();
		}
		
        OnButtonGetImage() ;
	}

	updatePicture(m_image) ;
}

#endif

void DialogSetToolObject::OnCheckFeatureUseCustomOption(bool checked)
{
    qDebug("Tool Check = %d : Use Custom Feature", checked) ;

	Ensemble_Tool_Set_UseCustomFeatureOption(GetId(), checked) ;
	int use_custom_feature = Ensemble_Tool_Get_UseCustomFeatureOption(GetId()) ;
	
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

void DialogSetToolObject::OnButtonSelectRefPoint(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_REF_POINT) ;
}

void DialogSetToolObject::OnButtonResetRefPoint(void)
{
	Ensemble_Tool_Del_Ref_Point(GetId()) ;
	
    OnButtonGetImage() ;
}


void DialogSetToolObject::OnButtonGetConstraintAngle(void)
{
	//Get Detect Option Value
    int detect_option_constraint_angle_min = Ensemble_Tool_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MIN) ;
	int detect_option_constraint_angle_max = Ensemble_Tool_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MAX) ;
	
    ui->lineEdit_constraint_angle_min->setText(QString::number(detect_option_constraint_angle_min)) ;
	ui->lineEdit_constraint_angle_max->setText(QString::number(detect_option_constraint_angle_max)) ;
}

void DialogSetToolObject::OnButtonSetConstraintAngle(void)
{
	 //Set Detect Option Value
    QString qstr_detect_option_constraint_angle_min = ui->lineEdit_constraint_angle_min->text() ;
    Ensemble_Tool_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MIN, qstr_detect_option_constraint_angle_min.toFloat()) ;

	QString qstr_detect_option_constraint_angle_max = ui->lineEdit_constraint_angle_max->text() ;
    Ensemble_Tool_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_CONSTRAINT_ANGLE_MAX, qstr_detect_option_constraint_angle_max.toFloat()) ;

    OnButtonGetConstraintAngle() ;
}

void DialogSetToolObject::OnButtonMaskPush(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_MASK) ;
}

void DialogSetToolObject::OnButtonMaskPop(void)
{
	Ensemble_Tool_Undo_MaskArea(GetId()) ;

	OnButtonGetImage() ;

	emit UpdateToolObjectImage();
}

void DialogSetToolObject::OnButtonMaskClear(void)
{
	Ensemble_Tool_Del_MaskArea(GetId()) ;

	OnButtonGetImage() ;

	emit UpdateToolObjectImage();
}

void DialogSetToolObject::OnButtonSetDetectOptionMargin(void)
{
    //Set Detect Option Value
    QString text_value = ui->lineEdit_detect_margin->text() ;
    Ensemble_Tool_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_MARGIN, text_value.toFloat()) ;

    OnButtonGetDetectOptionMargin() ;
}

void DialogSetToolObject::OnButtonSetDetectOptionThreshold(void)
{
    //Set Detect Option Value
    QString text_value = ui->lineEdit_detect_threshold->text() ;
    Ensemble_Tool_Set_DetectOption(GetId(), DetectOption::DETECT_OPTION_THRESHOLD, text_value.toFloat()) ;

    OnButtonGetDetectOptionThreshold() ;
}

void DialogSetToolObject::OnButtonGetDetectOptionMargin(void)
{
    //Get Detect Option Value
    int detect_option_margin = Ensemble_Tool_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_MARGIN) ;
    ui->lineEdit_detect_margin->setText(QString::number(detect_option_margin)) ;
}

void DialogSetToolObject::OnButtonGetDetectOptionThreshold(void)
{
    //Get Detect Option Value
    float detect_option_threshold = Ensemble_Tool_Get_DetectOption(GetId(), DetectOption::DETECT_OPTION_THRESHOLD) ;
    ui->lineEdit_detect_threshold->setText(QString("%2").arg(detect_option_threshold)) ;
}

void DialogSetToolObject::OnButtonSetDetectInspectionToleranceInfo(void) 
{
	float tol_distance_min = ui->lineEdit_detect_inspection_threshold->text().toFloat() ;
    Ensemble_Tool_Detect_Object_Set_Inspection_Tolerance_Info(GetId(), tol_distance_min) ;

    OnButtonGetDetectInspectionToleranceInfo() ;
}

void DialogSetToolObject::OnButtonGetDetectInspectionToleranceInfo(void)
{
	//Get Detect Option Value
	float detect_inspect_threshold = 0 ;
    Ensemble_Tool_Detect_Object_Get_Inspection_Tolerance_Info(GetId(), &detect_inspect_threshold) ;
    ui->lineEdit_detect_inspection_threshold->setText(QString("%2").arg(detect_inspect_threshold)) ;
}

