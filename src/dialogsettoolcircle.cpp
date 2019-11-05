#include "dialogsettoolcircle.h"
#include "ui_dialogsettoolcircle.h"

DialogSetToolCircle::DialogSetToolCircle(QWidget *parent) :
    QDialog(parent),
	m_pt_start(-1,-1) ,
	m_pt_end(-1,-1) ,	
    ui(new Ui::DialogSetToolCircle)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_name_change_circle, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//Object
    connect(ui->pushButton_select_object_circle, SIGNAL(clicked()), this,  SLOT(OnButtonSelectObject())) ;
    connect(ui->pushButton_reset_object_circle, SIGNAL(clicked()), this,  SLOT(OnButtonResetObject())) ;
	
	//slider
    connect(ui->horizontalSlider_feature_level_circle, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetFeatureLevel()));
    connect(ui->horizontalSlider_feature_level_circle, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMove(int)));

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetToolCircle::~DialogSetToolCircle()
{
    delete ui;
}

void DialogSetToolCircle::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id_circle->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolCircle::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name_circle->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;
	
	//Get Level 
    int feature_level = Ensemble_Tool_Get_FeatureLevel(GetId());
	//Set Slider
    ui->horizontalSlider_feature_level_circle->setValue(feature_level) ;
    ui->label_feature_level_circle->setText(QString::number(feature_level));

	//Get Base Level
	int base_feature_level = Ensemble_Job_Get_FeatureLevel(GetParentId());
	//Set Slider
	std::string str_base_feature_level ;
	str_base_feature_level = "(Base: " + std::to_string(base_feature_level) + ")" ;
	QString qstr_base_feature_level = QString::fromStdString(str_base_feature_level);
    ui->label_feature_level_base_circle->setText(qstr_base_feature_level);
	
	//Image
	OnButtonGetImage() ;
}

void DialogSetToolCircle::OnButtonGetImage(void)
{
    //Get Base Job Image
    unsigned char* get_job_image_data = NULL ;
    int job_image_width = 0 ;
    int job_image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
	
    Ensemble_Tool_Get_Image(GetId(), image_type, &get_job_image_data, &job_image_width, &job_image_height)  ;

    if( job_image_width > 0 && job_image_height > 0 )
    {
    	if( image_type == IMAGE_YUV420 )
    	{
	        //YUV420
	        cv::Mat get_image(job_image_height + job_image_height / 2, job_image_width, CV_8UC1, get_job_image_data) ;

	        CImgDec cls_image_decoder ;
	        m_image = cls_image_decoder.Decoding(get_image) ;
		}
		else if( image_type == IMAGE_RGB888 )
		{
			cv::Mat get_image(job_image_height, job_image_width, CV_8UC3, get_job_image_data) ;
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

void DialogSetToolCircle::updatePicture(cv::Mat image, cv::Rect rect_user)
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
	if( !rect_user.empty() )
	{
	    qDebug("%s : rect(%d,%d,%d,%d", __func__, rect_user.x, rect_user.y, rect_user.width, rect_user.height) ;

		if( rect_user.width > 0 && rect_user.height > 0 && m_pt_start.x >= 0 && m_pt_start.y >= 0 && m_pt_end.x >= 0 && m_pt_end.y >= 0  )
		{
			QPainter qPainter(&qt_display_image);
			qPainter.setBrush(Qt::NoBrush);
			//qPainter.setPen(Qt::black);
			//qPainter.drawRect(rect_user.x,rect_user.y,rect_user.width,rect_user.height);

			//circle
			int dx = m_pt_end.x - m_pt_start.x ;
			int dy = m_pt_end.y - m_pt_start.y ;
			int circle_radius = sqrt(dx*dx + dy*dy) ;

			qDebug("%s : circle_radius = %f", __func__, circle_radius) ;
			
			qPainter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
            qPainter.drawEllipse(QPoint(m_pt_start.x, m_pt_start.y), circle_radius, circle_radius);

			//margin line
			QString text_value = ui->lineEdit_detect_roi_margin_circle->text() ;
            const int circle_margin = text_value.toInt() / 2 ;
			
			qPainter.setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap));
			qPainter.drawEllipse(QPoint(m_pt_start.x, m_pt_start.y), circle_radius-circle_margin, circle_radius-circle_margin);
			qPainter.drawEllipse(QPoint(m_pt_start.x, m_pt_start.y), circle_radius+circle_margin, circle_radius+circle_margin);
			
			bool bEnd = qPainter.end();
		}
	}

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetToolCircle::OnButtonNameChange(void)
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
        ui->label_name_circle->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}

void DialogSetToolCircle::OnSliderSetFeatureLevel(void)
{
    //get level
    int level = ui->horizontalSlider_feature_level_circle->value() ;

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
    ui->horizontalSlider_feature_level_circle->setValue(feature_level) ;
    ui->label_feature_level_circle->setText(QString::number(feature_level));

	//Get Base Level
	int base_feature_level = Ensemble_Job_Get_FeatureLevel(GetParentId());
	//Set Slider
	std::string str_base_feature_level ;
	str_base_feature_level = "(Base: " + std::to_string(base_feature_level) + ")" ;
	QString qstr_base_feature_level = QString::fromStdString(str_base_feature_level);
    ui->label_feature_level_base_circle->setText(qstr_base_feature_level);

	//emit UpdateToolObjectImage();
}

void DialogSetToolCircle::OnSliderMove(int value)
{
    ui->label_feature_level_circle->setText(QString::number(value));
}

void DialogSetToolCircle::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetToolCircle::OnButtonResetObject(void)
{
	//Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;
}

#if 1
void DialogSetToolCircle::mousePressEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if (event->button() == Qt::LeftButton && m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		cv::Rect rect_user = m_cls_set_user_region.StartSetRegion(point.x(), point.y()) ;

		updatePicture(m_image, rect_user) ;

		m_pt_start = cv::Point(point.x(), point.y()) ;
		m_pt_end = cv::Point() ;
    }
}

void DialogSetToolCircle::mouseMoveEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if ((event->buttons() & Qt::LeftButton) && m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL)
	{
		QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		cv::Rect rect_user = m_cls_set_user_region.MoveSetRegion(point.x(), point.y()) ;
		        
		updatePicture(m_image, rect_user) ;

		m_pt_end = cv::Point(point.x(), point.y()) ;
	}
}

void DialogSetToolCircle::mouseReleaseEvent(QMouseEvent *event)
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
            if (ui->checkBox_mask_enable_inside_circle->isChecked())	b_enable_inside = false ;
            else                                                        b_enable_inside = true ;
				
            //Ensemble_Job_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			//emit UpdateToolObjectImage();
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
			//circle
			int dx = m_pt_end.x - m_pt_start.x ;
			int dy = m_pt_end.y - m_pt_start.y ;
			int circle_radius = sqrt(dx*dx + dy*dy) ;

			//margin line
			QString text_value = ui->lineEdit_detect_roi_margin_circle->text() ;
            const int circle_margin = text_value.toInt() / 2 ;
					
			cv::Point2f center_2pt = m_pt_start ;

			cv::Point2f pt_line_1(center_2pt.x - circle_radius,  center_2pt.y - circle_radius) ;
			cv::Point2f pt_line_2(center_2pt.x + circle_radius,  center_2pt.y - circle_radius) ;
			cv::Point2f pt_line_3(center_2pt.x + circle_radius,  center_2pt.y + circle_radius) ;
			cv::Point2f pt_line_4(center_2pt.x - circle_radius,  center_2pt.y + circle_radius) ;

			pt_line_1.x /= (float)label_w ;
	        pt_line_1.y /= (float)label_h ;
			pt_line_2.x /= (float)label_w ;
	        pt_line_2.y /= (float)label_h ;
			pt_line_3.x /= (float)label_w ;
	        pt_line_3.y /= (float)label_h ;
			pt_line_4.x /= (float)label_w ;
	        pt_line_4.y /= (float)label_h ;

            Ensemble_Tool_Set_SelectObject(GetId(), pt_line_1.x, pt_line_1.y, pt_line_2.x, pt_line_2.y, pt_line_3.x, pt_line_3.y, pt_line_4.x, pt_line_4.y, circle_margin) ;

			emit UpdateToolObjectImage();
		}
		
        OnButtonGetImage() ;
	}

	updatePicture(m_image) ;

	m_pt_start = cv::Point(-1,-1) ;
	m_pt_end = cv::Point(-1,-1) ;
}

#endif


