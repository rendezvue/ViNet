#include "dialogsettoolline.h"
#include "ui_dialogsettoolline.h"

DialogSetToolLine::DialogSetToolLine(QWidget *parent) :
    QDialog(parent),
	m_pt_start(-1,-1) ,
	m_pt_end(-1,-1) ,	
    ui(new Ui::DialogSetToolLine)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_name_change_line, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//Object
    connect(ui->pushButton_select_object_line, SIGNAL(clicked()), this,  SLOT(OnButtonSelectObject())) ;
    connect(ui->pushButton_reset_object_line, SIGNAL(clicked()), this,  SLOT(OnButtonResetObject())) ;
	
	//slider
    connect(ui->horizontalSlider_feature_level_line, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetFeatureLevel()));
    connect(ui->horizontalSlider_feature_level_line, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMove(int)));

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetToolLine::~DialogSetToolLine()
{
    delete ui;
}

void DialogSetToolLine::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id_line->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolLine::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name_line->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;
	
	//Get Level 
    int feature_level = Ensemble_Tool_Get_FeatureLevel(GetId());
	//Set Slider
    ui->horizontalSlider_feature_level_line->setValue(feature_level) ;
    ui->label_feature_level_line->setText(QString::number(feature_level));

	//Get Base Level
	int base_feature_level = Ensemble_Job_Get_FeatureLevel(GetParentId());
	//Set Slider
	std::string str_base_feature_level ;
	str_base_feature_level = "(Base: " + std::to_string(base_feature_level) + ")" ;
	QString qstr_base_feature_level = QString::fromStdString(str_base_feature_level);
    ui->label_feature_level_base_line->setText(qstr_base_feature_level);
	
	//Image
	OnButtonGetImage() ;
}

void DialogSetToolLine::OnButtonNameChange(void)
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
        ui->label_name_line->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}

void DialogSetToolLine::OnButtonGetImage(void)
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

void DialogSetToolLine::updatePicture(cv::Mat image, cv::Rect rect_user)
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

    QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);

	//draw set rect
	if( !rect_user.empty() )
	{
	    qDebug("%s : rect(%d,%d,%d,%d", __func__, rect_user.x, rect_user.y, rect_user.width, rect_user.height) ;

		if( rect_user.width > 0 && rect_user.height > 0 && m_pt_start.x >= 0 && m_pt_start.y >= 0 && m_pt_end.x >= 0 && m_pt_end.y >= 0 )
		{
			QPainter qPainter(&qt_display_image);
			qPainter.setBrush(Qt::NoBrush);
			//qPainter.setPen(Qt::black);
			//qPainter.drawRect(rect_user.x,rect_user.y,rect_user.width,rect_user.height);
			
            qPainter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap));
            qPainter.drawLine(m_pt_start.x, m_pt_start.y, m_pt_end.x, m_pt_end.y);

			//Draw Rotated Rect
			float dx = m_pt_end.x - m_pt_start.x ; //(rect_user.width) ;
			float dy = m_pt_end.y - m_pt_start.y ; //(rect_user.height) ;

			cv::Point2f center_2pt(m_pt_start.x+dx/2, m_pt_start.y+dy/2) ;
			
			//float dx = (rect_user.width) ;
			//float dy = (rect_user.height) ;
			//float radian = (CV_PI/2.0) - atan(dy/dx);
			//float radian = atan(dy/dx);

			cv::Point2f pt_line_1 ;
			cv::Point2f pt_line_2 ;
			cv::Point2f pt_line_3 ;
			cv::Point2f pt_line_4 ;
			
			QString text_value = ui->lineEdit_detect_roi_margin_line->text() ;
			const int min_line_margin = text_value.toInt() ;
#if 1
			float radian = atan(dy/dx) - (CV_PI/2.0) ;
			float distance = sqrt(dx*dx + dy*dy) ;
			float degree = (float)(180.0/CV_PI*radian);
			
			cv::RotatedRect rotatedRectangle(center_2pt, cv::Size(min_line_margin*2, distance), degree);

		    // We take the edges that OpenCV calculated for us
		    cv::Point2f vertices2f[4];
		    rotatedRectangle.points(vertices2f);

		    // Convert them so we can use them in a fillConvexPoly
		    pt_line_1 = vertices2f[0] ;
			pt_line_2 = vertices2f[1] ;
			pt_line_3 = vertices2f[2] ;
			pt_line_4 = vertices2f[3] ;
#else	
			
			float radian = atan(dy/dx) - (CV_PI/2.0) ;			
			
			pt_line_1.x = ( min_line_margin *  cos( radian ) ) + rect_user.x;
			pt_line_1.y = ( min_line_margin * sin( radian ) ) + rect_user.y;

			cv::Point2f pt_line_2 ;
			pt_line_2.x = ( min_line_margin *  cos( radian + CV_PI ) ) + rect_user.x;
			pt_line_2.y = ( min_line_margin * sin( radian + CV_PI ) ) + rect_user.y;

			cv::Point2f pt_line_3 ;
			pt_line_3.x = ( min_line_margin *  cos( radian ) ) + (rect_user.x + rect_user.width);
			pt_line_3.y = ( min_line_margin * sin( radian ) ) + (rect_user.y + rect_user.height);

			cv::Point2f pt_line_4 ;
			pt_line_4.x = ( min_line_margin *  cos( radian + CV_PI ) ) + (rect_user.x + rect_user.width);
			pt_line_4.y = ( min_line_margin * sin( radian + CV_PI ) ) + (rect_user.y + rect_user.height);
#endif

			qPainter.setPen(QPen(Qt::blue, 1, Qt::DashLine, Qt::RoundCap));
            qPainter.drawLine(pt_line_1.x, pt_line_1.y, pt_line_2.x, pt_line_2.y);
			qPainter.drawLine(pt_line_2.x, pt_line_2.y, pt_line_3.x, pt_line_3.y);
			qPainter.drawLine(pt_line_3.x, pt_line_3.y, pt_line_4.x, pt_line_4.y);			
			qPainter.drawLine(pt_line_4.x, pt_line_4.y, pt_line_1.x, pt_line_1.y);

#if 0
			qPainter.setBrush(Qt::red);
            qPainter.drawEllipse(pt_line_1.x, pt_line_1.y, 5, 5);
            qPainter.setBrush(Qt::green);
            qPainter.drawEllipse(pt_line_2.x, pt_line_2.y, 5, 5);
            qPainter.setBrush(Qt::blue);
            qPainter.drawEllipse(pt_line_3.x, pt_line_3.y, 5, 5);
            qPainter.setBrush(Qt::yellow);
            qPainter.drawEllipse(pt_line_4.x, pt_line_4.y, 5, 5);
#endif
	
			bool bEnd = qPainter.end();
		}
	}

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetToolLine::OnSliderSetFeatureLevel(void)
{
    //get level
    int level = ui->horizontalSlider_feature_level_line->value() ;

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
    ui->horizontalSlider_feature_level_line->setValue(feature_level) ;
    ui->label_feature_level_line->setText(QString::number(feature_level));

	//Get Base Level
	int base_feature_level = Ensemble_Job_Get_FeatureLevel(GetParentId());
	//Set Slider
	std::string str_base_feature_level ;
	str_base_feature_level = "(Base: " + std::to_string(base_feature_level) + ")" ;
	QString qstr_base_feature_level = QString::fromStdString(str_base_feature_level);
    ui->label_feature_level_base_line->setText(qstr_base_feature_level);

	//emit UpdateToolObjectImage();
}

void DialogSetToolLine::OnSliderMove(int value)
{
    ui->label_feature_level_line->setText(QString::number(value));
}

void DialogSetToolLine::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetToolLine::OnButtonResetObject(void)
{
	//Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;
}

#if 1
void DialogSetToolLine::mousePressEvent(QMouseEvent *event)
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

void DialogSetToolLine::mouseMoveEvent(QMouseEvent *event)
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

void DialogSetToolLine::mouseReleaseEvent(QMouseEvent *event)
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
            if (ui->checkBox_mask_enable_inside_line->isChecked())	b_enable_inside = false ;
            else												b_enable_inside = true ;
				
            //Ensemble_Job_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			//emit UpdateToolObjectImage();
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
			//Draw Rotated Rect
            //m_pt_end = cv::Point(point.x(), point.y()) ;
		
			float dx = m_pt_end.x - m_pt_start.x ; //(rect_user.width) ;
			float dy = m_pt_end.y - m_pt_start.y ; //(rect_user.height) ;

			cv::Point2f center_2pt(m_pt_start.x+dx/2, m_pt_start.y+dy/2) ;
			//float radian = (CV_PI/2.0) - atan(dy/dx);
			//float radian = atan(dy/dx);

			cv::Point2f pt_line_1 ;
			cv::Point2f pt_line_2 ;
			cv::Point2f pt_line_3 ;
			cv::Point2f pt_line_4 ;

			QString text_value = ui->lineEdit_detect_roi_margin_line->text() ;
			const int min_line_margin = text_value.toInt() ;
#if 1
			float radian = atan(dy/dx) - (CV_PI/2.0) ;
			float distance = sqrt(dx*dx + dy*dy) ;
			float degree = (float)(180.0/CV_PI*radian);
			
			cv::RotatedRect rotatedRectangle(center_2pt, cv::Size(min_line_margin*2, distance), degree);

		    // We take the edges that OpenCV calculated for us
		    cv::Point2f vertices2f[4];
		    rotatedRectangle.points(vertices2f);
			
		    // Convert them so we can use them in a fillConvexPoly
		    pt_line_1 = vertices2f[0] ;
			pt_line_2 = vertices2f[1] ;
			pt_line_3 = vertices2f[2] ;
			pt_line_4 = vertices2f[3] ;
			

			pt_line_1.x /= (float)label_w ;
	        pt_line_1.y /= (float)label_h ;
			pt_line_2.x /= (float)label_w ;
	        pt_line_2.y /= (float)label_h ;
			pt_line_3.x /= (float)label_w ;
	        pt_line_3.y /= (float)label_h ;
			pt_line_4.x /= (float)label_w ;
	        pt_line_4.y /= (float)label_h ;
#else	
			
			float radian = atan(dy/dx) - (CV_PI/2.0) ;			
			
			pt_line_1.x = ( min_line_margin *  cos( radian ) ) + rect_user.x;
			pt_line_1.y = ( min_line_margin * sin( radian ) ) + rect_user.y;

			cv::Point2f pt_line_2 ;
			pt_line_2.x = ( min_line_margin *  cos( radian + CV_PI ) ) + rect_user.x;
			pt_line_2.y = ( min_line_margin * sin( radian + CV_PI ) ) + rect_user.y;

			cv::Point2f pt_line_3 ;
			pt_line_3.x = ( min_line_margin *  cos( radian ) ) + (rect_user.x + rect_user.width);
			pt_line_3.y = ( min_line_margin * sin( radian ) ) + (rect_user.y + rect_user.height);

			cv::Point2f pt_line_4 ;
			pt_line_4.x = ( min_line_margin *  cos( radian + CV_PI ) ) + (rect_user.x + rect_user.width);
			pt_line_4.y = ( min_line_margin * sin( radian + CV_PI ) ) + (rect_user.y + rect_user.height);
#endif
			
            Ensemble_Tool_Set_SelectObject(GetId(), pt_line_1.x, pt_line_1.y, pt_line_2.x, pt_line_2.y, pt_line_3.x, pt_line_3.y, pt_line_4.x, pt_line_4.y, min_line_margin) ;

			emit UpdateToolObjectImage();
		}
		
        OnButtonGetImage() ;
	}

	updatePicture(m_image) ;

	m_pt_start = cv::Point(-1,-1) ;
	m_pt_end = cv::Point(-1,-1) ;
}

#endif

