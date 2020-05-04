#include "dialogsettooloffsetdistance.h"
#include "ui_dialogsettooloffsetdistance.h"
#include <QButtonGroup>
DialogSetToolOffsetDistance::DialogSetToolOffsetDistance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolOffsetDistance)
{
    ui->setupUi(this);

	//Object
	connect(ui->pushButton_select_object, SIGNAL(clicked()), this,	SLOT(OnButtonSelectObject())) ;
	connect(ui->pushButton_reset_object, SIGNAL(clicked()), this,  SLOT(OnButtonResetObject())) ;
	
	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_name_change, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//region get/set
	connect(ui->pushButton_region_set, SIGNAL(clicked()), this,  SLOT(OnButtonRegionSet())) ;
	connect(ui->pushButton_region_get, SIGNAL(clicked()), this,  SLOT(OnButtonRegionGet())) ;
	
	//radio button
	connect(ui->radioButton_1, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
	connect(ui->radioButton_2, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
	connect(ui->radioButton_3, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
	connect(ui->radioButton_4, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;

	//inspection button
	connect(ui->pushButton_offset_object_get_info, SIGNAL(clicked()), this,  SLOT(OnButtonGetInspectionBaseInfo())) ;	
	connect(ui->pushButton_offset_object_get_tolerance, SIGNAL(clicked()), this,  SLOT(OnButtonGetInspectionToleranceInfo())) ;	
	connect(ui->pushButton_offset_object_set_tolerance, SIGNAL(clicked()), this,  SLOT(OnButtonSetInspectionToleranceInfo())) ;	

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetToolOffsetDistance::~DialogSetToolOffsetDistance()
{
    delete ui;
}

void DialogSetToolOffsetDistance::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolOffsetDistance::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

    //qDebug("Tool Name = %s", tool_name.c_str()) ;

	int direction = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Get_Direction(GetId()) ;

	if( direction == 0 ) 		ui->radioButton_1->setChecked(true) ;
	else if( direction == 1 ) 	ui->radioButton_2->setChecked(true) ;
	else if( direction == 2 ) 	ui->radioButton_3->setChecked(true) ;
	else if( direction == 3 ) 	ui->radioButton_4->setChecked(true) ;

	OnButtonRegionGet() ;

	OnButtonGetInspectionBaseInfo() ;
	OnButtonGetInspectionToleranceInfo() ;
	
	//Image
	OnButtonGetImage() ;
}

void DialogSetToolOffsetDistance::OnButtonGetImage(void)
{
    //Get Base Job Image
    //unsigned char* get_job_image_data = NULL ;
    //int job_image_width = 0 ;
    //int job_image_height = 0 ;
    ImageBuf image_buf ;
    image_buf.image_width = 0 ;
    image_buf.image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
    //int get_image_type = 0 ;
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Get_Image(GetId(), image_type, &image_buf)  ;

    if( image_buf.image_width > 0 && image_buf.image_height > 0 )
    {
        if( image_buf.image_type == IMAGE_YUV420)
    	{
	        //YUV420
            cv::Mat get_image(image_buf.image_height + image_buf.image_height / 2, image_buf.image_width, CV_8UC1, image_buf.p_buf) ;

	        CImgDec cls_image_decoder ;
	        m_image = cls_image_decoder.Decoding(get_image) ;
    	}
        else if( image_buf.image_type == IMAGE_RGB888 )
		{
            cv::Mat get_image(image_buf.image_height, image_buf.image_width, CV_8UC3, image_buf.p_buf) ;
			cv::cvtColor(get_image, m_image, cv::COLOR_BGR2RGB) ;
		}
        else if( image_buf.image_type == IMAGE_JPG )
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, image_buf.image_height*image_buf.image_width, CV_8UC1, image_buf.p_buf), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, m_image, cv::COLOR_BGR2RGB) ;
        }

        updatePicture(m_image) ;
    }

    if( image_buf.p_buf != NULL )
    {
        delete [] image_buf.p_buf ;
        image_buf.p_buf = NULL ;
    }
}

void DialogSetToolOffsetDistance::updatePicture(cv::Mat image, cv::Rect rect_user)
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
	    //qDebug("%s : rect(%d,%d,%d,%d", __func__, rect_user.x, rect_user.y, rect_user.width, rect_user.height) ;

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


void DialogSetToolOffsetDistance::OnButtonNameChange(void)
{
    std::string tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Get_Name(GetId()) ;

    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetId());
    dlg_change_name.SetName(tool_name);

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        std::string change_name = dlg_change_name.GetName() ;

        //qDebug("Tool Change Name = %s", change_name.c_str()) ;
		
        if( !change_name.empty() )
        {
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Set_Name(GetId(), change_name) ;
        }

        tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

        //qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}

void DialogSetToolOffsetDistance::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetToolOffsetDistance::OnButtonResetObject(void)
{
	//CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;

	OnButtonGetInspectionBaseInfo() ;
	OnButtonGetInspectionToleranceInfo() ;
}

void DialogSetToolOffsetDistance::mousePressEvent(QMouseEvent *event)
{
    //qDebug("%s - %d", __func__, __LINE__) ;

    if (event->button() == Qt::LeftButton && m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		cv::Rect rect_user = m_cls_set_user_region.StartSetRegion(point.x(), point.y()) ;

		updatePicture(m_image, rect_user) ;
    }
}

void DialogSetToolOffsetDistance::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug("%s - %d", __func__, __LINE__) ;

   if ( m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL)
	{
		QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

        if( (event->buttons() & Qt::LeftButton) )
        {
            cv::Rect rect_user = m_cls_set_user_region.MoveSetRegion(point.x(), point.y()) ;

            updatePicture(m_image, rect_user) ;
        }
	}
}

void DialogSetToolOffsetDistance::mouseReleaseEvent(QMouseEvent *event)
{
    //if (event->button() == Qt::LeftButton && scribbling) {
    //    drawLineTo(event->pos());
    //    scribbling = false;
    //}

	//Set
	int set_status = m_cls_set_user_region.GetStatus() ;

	//qDebug("%s - %d : m_set_status(%d), event->buttons()=%d", __func__, __LINE__, set_status, event->buttons()) ;
	
    if (set_status > SetBaseStatus::NORMAL)
	{
		//qDebug("%s - %d", __func__, __LINE__) ;
		
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

		//qDebug("%s - %d : m_set_status(%d)", __func__, __LINE__, set_status) ;
		
        if( set_status == SetBaseStatus::SET_AREA )
        {
            //CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_DetectArea(GetId(), f_x, f_y, f_w, f_h) ;

            //emit UpdateToolObjectImage();
        }
        else if( set_status == SetBaseStatus::SET_ZOOM)
        {
            //CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Zoom(GetId(), f_x, f_y, f_w, f_h) ;
        }
		else if( set_status == SetBaseStatus::SET_MASK)
        {
        	#if 0
            bool b_enable_inside = false ;
            if (ui->checkBox_mask_enable_inside->isChecked())	b_enable_inside = false ;
            else												b_enable_inside = true ;
				
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			emit UpdateToolObjectImage();
			#endif
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;

			OnButtonRegionGet() ;
			
			emit UpdateToolObjectImage();
		}
		else if( set_status == SetBaseStatus::SET_REF_POINT)
		{
			#if 0
			QPoint point = event->pos() ;
	        point.setX(point.x() - ui->label_image->x());
	        point.setY(point.y() - ui->label_image->y());

			f_x = (float)point.x() / (float)label_w ;
        	f_y = (float)point.y() / (float)label_h ;
		
			//SelectObject
			//CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Set_Ref_Point(GetId(), f_x, f_y) ;

            emit UpdateToolObjectImage();
			#endif
		}
		
        OnButtonGetImage() ;

		OnButtonGetInspectionBaseInfo() ;
		OnButtonGetInspectionToleranceInfo() ;
	}

	updatePicture(m_image) ;
}

void DialogSetToolOffsetDistance::OnButtonChangeDirection(void)
{
	QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();

    for(int i = 0; i < allButtons.size(); ++i)
    {
        group.addButton(allButtons[i],i);
    }

    int index = group.checkedId() ;

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Set_Direction(GetId(), index) ;

	int direction = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Get_Direction(GetId()) ;

	if( direction == 0 ) 		ui->radioButton_1->setChecked(true) ;
	else if( direction == 1 ) 	ui->radioButton_2->setChecked(true) ;
	else if( direction == 2 ) 	ui->radioButton_3->setChecked(true) ;
	else if( direction == 3 ) 	ui->radioButton_4->setChecked(true) ;
	
#if 0
    group.checkedButton()->setChecked(true);

    foreach(QAbstractButton* button, allButtons)
    {
        if (id(button) == _id)
        {
            button->setChecked(true);
            break;
        }
    }
#endif	

	OnButtonGetImage() ;

	OnButtonGetInspectionBaseInfo() ;
	OnButtonGetInspectionToleranceInfo() ;

}

void DialogSetToolOffsetDistance::OnButtonRegionGet(void)
{
	float x=0,y=0,width=0,height=0 ; 
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Get_Region(GetId(), &x, &y, &width, &height) ;

	ui->lineEdit_region_x->setText(QString::number(x));
	ui->lineEdit_region_y->setText(QString::number(y));
	ui->lineEdit_region_width->setText(QString::number(width));
	ui->lineEdit_region_height->setText(QString::number(height));
}

void DialogSetToolOffsetDistance::OnButtonRegionSet(void)
{
	int region_x = ui->lineEdit_region_x->text().toInt() ;
	int region_y = ui->lineEdit_region_y->text().toInt() ;
	int region_width = ui->lineEdit_region_width->text().toInt() ;
	int region_height = ui->lineEdit_region_height->text().toInt() ;

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Set_Region(GetId(), region_x, region_y, region_width, region_height) ;

	OnButtonGetImage() ;

	OnButtonGetInspectionBaseInfo() ;
	OnButtonGetInspectionToleranceInfo() ;
}

void DialogSetToolOffsetDistance::OnButtonGetInspectionBaseInfo(void)
{
	float distance=0,angle=0 ; 
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Get_Inspection_Base_Info(GetId(), &distance, &angle) ;

	ui->lineEdit_base_info_distance->setText(QString::number(distance));
	ui->lineEdit_base_info_angle->setText(QString::number(angle));
}

void DialogSetToolOffsetDistance::OnButtonGetInspectionToleranceInfo(void)
{
	float distance_min=0,distance_max=0, angle_min=0, angle_max=0 ; 
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Get_Inspection_Tolerance_Info(GetId(), &distance_min, &distance_max, &angle_min, &angle_max) ;

	ui->lineEdit_tol_info_distance_min->setText(QString::number(distance_min));
	ui->lineEdit_tol_info_distance_max->setText(QString::number(distance_max));

	ui->lineEdit_tol_info_angle_min->setText(QString::number(angle_min));
	ui->lineEdit_tol_info_angle_max->setText(QString::number(angle_max));
}

void DialogSetToolOffsetDistance::OnButtonSetInspectionToleranceInfo(void)
{
	float tol_distance_min = ui->lineEdit_tol_info_distance_min->text().toFloat() ;
	float tol_distance_max = ui->lineEdit_tol_info_distance_max->text().toFloat() ;
	float tol_angle_min = ui->lineEdit_tol_info_angle_min->text().toFloat() ;
	float tol_angle_max = ui->lineEdit_tol_info_angle_max->text().toFloat() ;

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Offset_Distance_Set_Inspection_Tolerance_Info(GetId(), tol_distance_min, tol_distance_max, tol_angle_min, tol_angle_max) ;

	OnButtonGetInspectionToleranceInfo() ;
}

