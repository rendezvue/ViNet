#include "dialogsetdetectplane.h"
#include "ui_dialogsetdetectplane.h"

DialogSetDetectPlane::DialogSetDetectPlane(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetDetectPlane)
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
	
	//mask
	connect(ui->pushButton_mask_push, SIGNAL(clicked()), this,	SLOT(OnButtonMaskPush())) ;
	connect(ui->pushButton_mask_pop, SIGNAL(clicked()), this,  SLOT(OnButtonMaskPop())) ;
	connect(ui->pushButton_mask_clear, SIGNAL(clicked()), this,  SLOT(OnButtonMaskClear())) ;

	//background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetDetectPlane::~DialogSetDetectPlane()
{
    delete ui;
}

void DialogSetDetectPlane::SetId(const std::string id)
{
    m_str_id = id ;

    ui->label_id->setText(QString::fromUtf8(m_str_id.c_str()));
}

std::string DialogSetDetectPlane::GetId(void)
{
    return m_str_id ;
}

void DialogSetDetectPlane::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string base_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(base_name.c_str()));

    OnButtonGetImage() ;	
}

void DialogSetDetectPlane::OnButtonGetImage(void)
{
    //Get Base Job Image
    //unsigned char* get_job_image_data = NULL ;
    //int job_image_width = 0 ;
    //int job_image_height = 0 ;
    ImageBuf image_buf ;
    image_buf.image_width = 0 ;
    image_buf.image_height = 0 ;

	const int image_type = ImageTypeOption::IMAGE_RGB888 ;
    //int get_image_type = 0 ;
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Image(GetId(), image_type, &image_buf)  ;
	
    if( image_buf.image_width > 0 && image_buf.image_height > 0 )
    {
    	CImageBuf2Mat cls_imagebuf2mat ;
    	m_image = cls_imagebuf2mat.Cvt(image_buf) ;
		
        updatePicture(m_image) ;
    }

    if( image_buf.p_buf != NULL )
    {
        delete [] image_buf.p_buf ;
        image_buf.p_buf = NULL ;
    }
}

void DialogSetDetectPlane::updatePicture(cv::Mat image, cv::Rect rect_user) 
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

    if( !(rect_user.width <= 0 || rect_user.height <= 0))
	{
		//draw set rect
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

void DialogSetDetectPlane::OnButtonSetImage(void)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Image(GetId())  ;
	
    OnButtonGetImage() ;

	emit UpdateBaseImage();
}

void DialogSetDetectPlane::OnButtonSetDetectArea(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_AREA) ;
}

void DialogSetDetectPlane::OnButtonClearDetectArea(void)
{
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_DetectArea(GetId(), 0, 0, 0, 0) ;

    OnButtonGetImage() ;

    emit UpdateBaseImage();
}

void DialogSetDetectPlane::OnButtonZoomSet(void)
{
 	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_ZOOM) ;
}

void DialogSetDetectPlane::OnButtonZoomReset(void)
{
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Zoom(GetId(), 0, 0, 0, 0) ;       //reset

    OnButtonGetImage() ;
}

void DialogSetDetectPlane::OnButtonNameChange(void)
{
    std::string base_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;

    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetId());
    dlg_change_name.SetName(base_name);

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        std::string change_name = dlg_change_name.GetName() ;

        if( !change_name.empty() )
        {
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Name(GetId(), change_name) ;
        }

        base_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(base_name.c_str()));

		emit UpdateBaseName(QString::fromUtf8(base_name.c_str())) ;
    }
}

void DialogSetDetectPlane::OnButtonMaskPush(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_MASK) ;
}

void DialogSetDetectPlane::OnButtonMaskPop(void)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Undo_MaskArea(GetId()) ;

	OnButtonGetImage() ;

	emit UpdateBaseImage();
}

void DialogSetDetectPlane::OnButtonMaskClear(void)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Del_MaskArea(GetId()) ;

	OnButtonGetImage() ;

	emit UpdateBaseImage();
}

void DialogSetDetectPlane::mousePressEvent(QMouseEvent *event)
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

void DialogSetDetectPlane::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug("%s - %d", __func__, __LINE__) ;

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

void DialogSetDetectPlane::mouseReleaseEvent(QMouseEvent *event)
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
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_DetectArea(GetId(), f_x, f_y, f_w, f_h) ;

            emit UpdateBaseImage();
        }
        else if( set_status == SetBaseStatus::SET_ZOOM)
        {
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Zoom(GetId(), f_x, f_y, f_w, f_h) ;
        }
		else if( set_status == SetBaseStatus::SET_MASK)
        {
            bool b_enable_inside = false ;
            if (ui->checkBox_mask_enable_inside->isChecked())	b_enable_inside = false ;
            else												b_enable_inside = true ;
				
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			emit UpdateBaseImage();
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
			CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;

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
			//CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Ref_Point(GetId(), f_x, f_y) ;

			emit UpdateBaseImage();
		}
		else if( set_status == SetBaseStatus::SET_ERASE)
		{
			//SelectObject
			CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Erase(GetId(), f_x, f_y, f_w, f_h) ;

			emit UpdateBaseImage();
		}
	
        OnButtonGetImage() ;
	}

	updatePicture(m_image) ;
}

void DialogSetDetectPlane::updatePictureCenterLine(cv::Mat image, cv::Point pt_user)
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
    //qDebug("%s : point(%d,%d)", __func__, pt_user.x, pt_user.y) ;

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

void DialogSetDetectPlane::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetDetectPlane::OnButtonResetObject(void)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;
}

