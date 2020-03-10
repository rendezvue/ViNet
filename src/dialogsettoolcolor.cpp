#include "dialogsettoolcolor.h"
#include "ui_dialogsettoolcolor.h"

DialogSetToolColor::DialogSetToolColor(QWidget *parent) :
    QDialog(parent),
    m_i_set_color(-1),
    m_i_s_value(-1),
    m_i_e_value(-1),
    ui(new Ui::DialogSetToolColor)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_get_red_histogram, SIGNAL(clicked()), this,  SLOT(OnButtonGetRedHistogramImage())) ;
	connect(ui->pushButton_get_green_histogram, SIGNAL(clicked()), this,  SLOT(OnButtonGetGreenHistogramImage())) ;
	connect(ui->pushButton_get_blue_histogram, SIGNAL(clicked()), this,  SLOT(OnButtonGetBlueHistogramImage())) ;

	connect(ui->pushButton_range_get_red, SIGNAL(clicked()), this,  SLOT(OnButtonGetRedHistogramRange())) ;
	connect(ui->pushButton_range_set_red, SIGNAL(clicked()), this,  SLOT(OnButtonSetRedHistogramRange())) ;
	connect(ui->pushButton_range_get_green, SIGNAL(clicked()), this,  SLOT(OnButtonGetGreenHistogramRange())) ;
	connect(ui->pushButton_range_set_green, SIGNAL(clicked()), this,  SLOT(OnButtonSetGreenHistogramRange())) ;
	connect(ui->pushButton_range_get_blue, SIGNAL(clicked()), this,  SLOT(OnButtonGetBlueHistogramRange())) ;
	connect(ui->pushButton_range_set_blue, SIGNAL(clicked()), this,  SLOT(OnButtonSetBlueHistogramRange())) ;

	connect(ui->pushButton_get_base_pixel_count, SIGNAL(clicked()), this,  SLOT(OnButtonGetBasePixelCount())) ;

	connect(ui->pushButton_tolerance_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetTolerance())) ;
	connect(ui->pushButton_tolerance_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetTolerance())) ;
	
	
		
	//check box
	connect(ui->checkBox_use_red, SIGNAL(clicked(bool)), this,  SLOT(OnButtonSetUseRed(bool))) ;
	connect(ui->checkBox_use_green, SIGNAL(clicked(bool)), this,  SLOT(OnButtonSetUseGreen(bool))) ;
	connect(ui->checkBox_use_blue, SIGNAL(clicked(bool)), this,  SLOT(OnButtonSetUseBlue(bool))) ;

	//background color
	ui->label_image_red_histogram_2->setStyleSheet("QLabel { background-color : black; }");
	ui->label_image_green_histogram_2->setStyleSheet("QLabel { background-color : black; }");
	ui->label_image_blue_histogram_2->setStyleSheet("QLabel { background-color : black; }");

	ui->comboBox_preset->addItem("0");
	ui->comboBox_preset->addItem("10");
	ui->comboBox_preset->addItem("20");
	ui->comboBox_preset->addItem("30");
	ui->comboBox_preset->addItem("40");
	ui->comboBox_preset->addItem("50");

    connect(ui->comboBox_preset, SIGNAL(currentIndexChanged(int)), SLOT(OnComboGetPreset(int)));

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
	
}

DialogSetToolColor::~DialogSetToolColor()
{
    delete ui;
}

void DialogSetToolColor::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolColor::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

	OnButtonGetRedHistogramRange() ;
	OnButtonGetGreenHistogramRange() ;
	OnButtonGetBlueHistogramRange() ;
	
	CheckedUseRed() ;
	CheckedUseGreen() ;
	CheckedUseBlue() ;
	
	//Image
	OnButtonGetImage() ;

	//Histogram Image
	OnButtonGetRedHistogramImage() ;
	OnButtonGetGreenHistogramImage() ;
	OnButtonGetBlueHistogramImage() ;

	OnButtonGetBasePixelCount() ;
	OnButtonGetTolerance() ;
	//checkbox using color element
}

void DialogSetToolColor::OnButtonGetImage(void)
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
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_GetImage(GetId(), image_type, &image_buf)  ;

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
        else if( image_buf.image_type == ImageTypeOption::IMAGE_JPG)
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, image_buf.image_width*image_buf.image_height, CV_8UC1, image_buf.p_buf), cv::IMREAD_UNCHANGED) ;
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

void DialogSetToolColor::OnButtonGetRedHistogramImage(void)
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
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Histogram_GetImage(GetId(), 0/*red*/, image_type, &image_buf)  ;

	cv::Mat histogram_image ;
    if( image_buf.image_width > 0 && image_buf.image_height > 0 )
    {
        if( image_buf.image_type == IMAGE_YUV420)
    	{
	        //YUV420
            cv::Mat get_image(image_buf.image_height + image_buf.image_height / 2, image_buf.image_width, CV_8UC1, image_buf.p_buf) ;

	        CImgDec cls_image_decoder ;
	        histogram_image = cls_image_decoder.Decoding(get_image) ;
    	}
        else if( image_buf.image_type == IMAGE_RGB888 )
		{
            cv::Mat get_image(image_buf.image_height, image_buf.image_width, CV_8UC3, image_buf.p_buf) ;
			cv::cvtColor(get_image, histogram_image, cv::COLOR_BGR2RGB) ;
		}
        else if( image_buf.image_type == ImageTypeOption::IMAGE_JPG)
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, image_buf.image_width*image_buf.image_height, CV_8UC1, image_buf.p_buf), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, histogram_image, cv::COLOR_BGR2RGB) ;
        }

        //updatePicture(m_image) ;
        CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(histogram_image, ui->label_image_red_histogram->width(), ui->label_image_red_histogram->height()) ;
		
	    ui->label_image_red_histogram->setPixmap(QPixmap::fromImage(qt_display_image));
    }

    if( image_buf.p_buf != NULL )
    {
        delete [] image_buf.p_buf ;
        image_buf.p_buf = NULL ;
    }
}

void DialogSetToolColor::OnButtonGetGreenHistogramImage(void)
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
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Histogram_GetImage(GetId(), 1/*green*/, image_type, &image_buf)  ;

	cv::Mat histogram_image ;
    if( image_buf.image_width > 0 && image_buf.image_height > 0 )
    {
        if( image_buf.image_type == IMAGE_YUV420)
    	{
	        //YUV420
            cv::Mat get_image(image_buf.image_height + image_buf.image_height / 2, image_buf.image_width, CV_8UC1, image_buf.p_buf) ;

	        CImgDec cls_image_decoder ;
	        histogram_image = cls_image_decoder.Decoding(get_image) ;
    	}
        else if( image_buf.image_type == IMAGE_RGB888 )
		{
            cv::Mat get_image(image_buf.image_height, image_buf.image_width, CV_8UC3, image_buf.p_buf) ;
			cv::cvtColor(get_image, histogram_image, cv::COLOR_BGR2RGB) ;
		}
        else if( image_buf.image_type == ImageTypeOption::IMAGE_JPG)
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, image_buf.image_width*image_buf.image_height, CV_8UC1, image_buf.p_buf), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, histogram_image, cv::COLOR_BGR2RGB) ;
        }

        //updatePicture(m_image) ;
        CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(histogram_image, ui->label_image_green_histogram->width(), ui->label_image_green_histogram->height()) ;
		
	    ui->label_image_green_histogram->setPixmap(QPixmap::fromImage(qt_display_image));
    }

    if( image_buf.p_buf != NULL )
    {
        delete [] image_buf.p_buf ;
        image_buf.p_buf = NULL ;
    }
}

void DialogSetToolColor::OnButtonGetBlueHistogramImage(void)
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
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Histogram_GetImage(GetId(), 2/*blue*/, image_type, &image_buf)  ;

	cv::Mat histogram_image ;
    if( image_buf.image_width > 0 && image_buf.image_height > 0 )
    {
        if( image_buf.image_type == IMAGE_YUV420)
    	{
	        //YUV420
            cv::Mat get_image(image_buf.image_height + image_buf.image_height / 2, image_buf.image_width, CV_8UC1, image_buf.p_buf) ;

	        CImgDec cls_image_decoder ;
	        histogram_image = cls_image_decoder.Decoding(get_image) ;
    	}
        else if( image_buf.image_type == IMAGE_RGB888 )
		{
            cv::Mat get_image(image_buf.image_height, image_buf.image_width, CV_8UC3, image_buf.p_buf) ;
			cv::cvtColor(get_image, histogram_image, cv::COLOR_BGR2RGB) ;
		}
        else if( image_buf.image_type == ImageTypeOption::IMAGE_JPG)
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, image_buf.image_width*image_buf.image_height, CV_8UC1, image_buf.p_buf), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, histogram_image, cv::COLOR_BGR2RGB) ;
        }

        //updatePicture(m_image) ;
        CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(histogram_image, ui->label_image_blue_histogram->width(), ui->label_image_blue_histogram->height()) ;
		
	    ui->label_image_blue_histogram->setPixmap(QPixmap::fromImage(qt_display_image));
    }

    if( image_buf.p_buf != NULL )
    {
        delete [] image_buf.p_buf ;
        image_buf.p_buf = NULL ;
    }
}

void DialogSetToolColor::updatePicture(cv::Mat image)
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

	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;
	
    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetToolColor::CheckedUseRed(void)
{
	int check_color_elem = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(GetId())  ;
	if (check_color_elem & HISTOGRAM_USE_RED )
	{
		ui->checkBox_use_red->setChecked(1) ;
	}
	else
	{
		ui->checkBox_use_red->setChecked(0) ;
	}
	
	OnButtonGetRedHistogramImage() ;
	OnButtonGetImage() ;
}

void DialogSetToolColor::CheckedUseGreen(void)
{
	//check
	int check_color_elem = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(GetId())  ;
	if (check_color_elem & HISTOGRAM_USE_GREEN )
	{
		ui->checkBox_use_green->setChecked(1) ;
	}
	else
	{
		ui->checkBox_use_green->setChecked(0) ;
	}
	
	OnButtonGetGreenHistogramImage() ;
	OnButtonGetImage() ;
}

void DialogSetToolColor::CheckedUseBlue(void) 
{
	//check
	int check_color_elem = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(GetId())  ;
	if (check_color_elem & HISTOGRAM_USE_BLUE )
	{
		ui->checkBox_use_blue->setChecked(1) ;
	}
	else
	{
		ui->checkBox_use_blue->setChecked(0) ;
	}
	
	OnButtonGetBlueHistogramImage() ;
	OnButtonGetImage() ;
}

void DialogSetToolColor::OnButtonSetUseRed(bool checked)
{
	qDebug("Check RED = %d", checked) ;
	
	int color_elem = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(GetId())  ;
	
	if (checked)	color_elem |= HISTOGRAM_USE_RED ;
	else 			color_elem &= ~HISTOGRAM_USE_RED ;

	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_UseElement(GetId(), color_elem)  ;

	CheckedUseRed() ;

	OnButtonGetBasePixelCount() ;
}

void DialogSetToolColor::OnButtonSetUseGreen(bool checked)
{
	qDebug("Check GREEN = %d", checked) ;
	
	int color_elem = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(GetId())  ;
	
	if (checked)	color_elem |= HISTOGRAM_USE_GREEN ;
	else 			color_elem &= ~HISTOGRAM_USE_GREEN ;

	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_UseElement(GetId(), color_elem)  ;

	CheckedUseGreen() ;

	OnButtonGetBasePixelCount() ;
}

void DialogSetToolColor::OnButtonSetUseBlue(bool checked)
{
	qDebug("Check BLUE = %d", checked) ;
	
	int color_elem = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_UseElement(GetId())  ;
	
	if (checked)	color_elem |= HISTOGRAM_USE_BLUE ;
	else 			color_elem &= ~HISTOGRAM_USE_BLUE ;

	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_UseElement(GetId(), color_elem)  ;

	CheckedUseBlue() ;

	OnButtonGetBasePixelCount() ;
}


void DialogSetToolColor::OnButtonGetRedHistogramRange(void)
{
	int out_min=0, out_max=0 ;
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_Range(GetId(), HISTOGRAM_USE_RED, &out_min, &out_max)  ;

	ui->lineEdit_range_min_red->setText(QString::number(out_min));
	ui->lineEdit_range_max_red->setText(QString::number(out_max));
}

void DialogSetToolColor::OnButtonSetRedHistogramRange(void)
{	
	int min = ui->lineEdit_range_min_red->text().toInt();
	int max = ui->lineEdit_range_max_red->text().toInt();

	float f_min = (float)min / 255.0 ;
	float f_max = (float)max / 255.0 ;
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), HISTOGRAM_USE_RED, f_min, f_max)  ;

	//check
    OnButtonGetRedHistogramRange() ;

	OnButtonGetRedHistogramImage() ;
	OnButtonGetImage() ;

	OnButtonGetBasePixelCount() ;
}

void DialogSetToolColor::OnButtonGetGreenHistogramRange(void)
{
	int out_min=0, out_max=0 ;
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_Range(GetId(), HISTOGRAM_USE_GREEN, &out_min, &out_max)  ;

	ui->lineEdit_range_min_green->setText(QString::number(out_min));
	ui->lineEdit_range_max_green->setText(QString::number(out_max));
}

void DialogSetToolColor::OnButtonSetGreenHistogramRange(void)
{
	int min = ui->lineEdit_range_min_green->text().toInt();
	int max = ui->lineEdit_range_max_green->text().toInt();

	float f_min = (float)min / 255.0 ;
	float f_max = (float)max / 255.0 ;
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), HISTOGRAM_USE_GREEN, f_min, f_max)  ;

	//check
    OnButtonGetGreenHistogramRange() ;

	OnButtonGetGreenHistogramImage() ;
	OnButtonGetImage() ;

	OnButtonGetBasePixelCount() ;
}

void DialogSetToolColor::OnButtonGetBlueHistogramRange(void)
{
	int out_min=0, out_max=0 ;
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Histogram_Range(GetId(), HISTOGRAM_USE_BLUE, &out_min, &out_max)  ;

	ui->lineEdit_range_min_blue->setText(QString::number(out_min));
	ui->lineEdit_range_max_blue->setText(QString::number(out_max));
}

void DialogSetToolColor::OnButtonSetBlueHistogramRange(void)
{
	int min = ui->lineEdit_range_min_blue->text().toInt();
	int max = ui->lineEdit_range_min_blue->text().toInt();

	float f_min = (float)min / 255.0 ;
	float f_max = (float)max / 255.0 ;
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), HISTOGRAM_USE_BLUE, f_min, f_max)  ;

	//check
    OnButtonGetBlueHistogramRange() ;
	OnButtonGetBlueHistogramImage() ;

	OnButtonGetImage() ;

	OnButtonGetBasePixelCount() ;
}

void DialogSetToolColor::mousePressEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if (event->button() == Qt::LeftButton ) 
	{
        QPoint point = event->pos() ;
        //point.setX(point.x() - ui->label_image->x());
        //point.setY(point.y() - ui->label_image->y());

		qDebug("point : %d, %d", point.x(), point.y()) ;
        qDebug("label_image_red_histogram : %d, %d, %d, %d", ui->label_image_red_histogram->x(), ui->label_image_red_histogram->y(), ui->label_image_red_histogram->width(), ui->label_image_red_histogram->height()) ;

		int groupBox_color_x = ui->groupBox_color->x() ;
		int groupBox_color_y = ui->groupBox_color->y() ;

		qDebug("groupBox_color : %d, %d", groupBox_color_x, groupBox_color_y) ;
		
		int groupBox_color_red_x = groupBox_color_x + ui->groupBox_red->x() ;
		int groupBox_color_red_y = groupBox_color_y + ui->groupBox_red->y() ;

		qDebug("groupBox_color_red : %d, %d", groupBox_color_red_x, groupBox_color_red_y) ;
		
		int groupBox_color_green_x = groupBox_color_x + ui->groupBox_green->x() ;
		int groupBox_color_green_y = groupBox_color_y + ui->groupBox_green->y() ;

		int groupBox_color_blue_x = groupBox_color_x + ui->groupBox_blue->x() ;
		int groupBox_color_blue_y = groupBox_color_y + ui->groupBox_blue->y() ;

		int groupBox_color_red_histogram_x = groupBox_color_red_x + ui->label_image_red_histogram_2->x() ;
		int groupBox_color_red_histogram_y = groupBox_color_red_y + ui->label_image_red_histogram_2->y() ;

		int groupBox_color_green_histogram_x = groupBox_color_green_x + ui->label_image_green_histogram_2->x() ;
		int groupBox_color_green_histogram_y = groupBox_color_green_y + ui->label_image_green_histogram_2->y() ;

		int groupBox_color_blue_histogram_x = groupBox_color_blue_x + ui->label_image_blue_histogram_2->x() ;
		int groupBox_color_blue_histogram_y = groupBox_color_blue_y + ui->label_image_blue_histogram_2->y() ;

		qDebug("groupBox_color_red_histogram : %d, %d", groupBox_color_red_histogram_x, groupBox_color_red_histogram_y) ;

		//check position
        if( point.x() >= groupBox_color_red_histogram_x &&
            point.x() <= groupBox_color_red_histogram_x + ui->label_image_red_histogram_2->width() &&
            point.y() >= groupBox_color_red_histogram_y &&
            point.y() <= groupBox_color_red_histogram_y + ui->label_image_red_histogram_2->height()  )
		{
			qDebug("Start Set Red Range") ;
			
            m_i_set_color = HISTOGRAM_USE_RED ;
            m_i_s_value = point.x() - (groupBox_color_red_x + ui->label_image_red_histogram->x()) ;
			m_i_e_value = m_i_s_value ;

			//qDebug("Red Value : m_i_s_value = %d, point.x()=%d, groupBox_color_red_x=%d, ui->label_image_red_histogram->x()=%d", m_i_s_value, point.x(), groupBox_color_red_x, ui->label_image_red_histogram->x()) ;

			float f_min = 0.0 ;
	        int label_w = ui->label_image_red_histogram->width() ;
	        f_min = (float)m_i_s_value / (float)label_w ;

			qDebug("Red Value : f_min = %f", f_min) ;
			
            CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_min, f_min) ;

			OnButtonGetRedHistogramImage() ;
			OnButtonGetImage() ;

			OnButtonGetRedHistogramRange() ;

			OnButtonGetBasePixelCount() ;
		}
        else if( point.x() >= groupBox_color_green_histogram_x &&
                 point.x() <= groupBox_color_green_histogram_x + ui->label_image_green_histogram_2->width() &&
                 point.y() >= groupBox_color_green_histogram_y &&
                 point.y() <= groupBox_color_green_histogram_y + ui->label_image_green_histogram_2->height()  )
        {
        	qDebug("Start Set Green Range") ;
			
            m_i_set_color = HISTOGRAM_USE_GREEN ;
            m_i_s_value = point.x() - (groupBox_color_green_x + ui->label_image_green_histogram->x()) ;
			m_i_e_value = m_i_s_value ;
			
			qDebug("Green Value : m_i_s_value = %d", m_i_s_value) ;
			
			float f_min = 0.0 ;
	        int label_w = ui->label_image_green_histogram->width() ;
	        f_min = (float)m_i_s_value / (float)label_w ;

			qDebug("Green Value : f_min = %f", f_min) ;
			
            CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_min, f_min) ;

			OnButtonGetGreenHistogramImage() ;
			OnButtonGetImage() ;

			OnButtonGetGreenHistogramRange() ;

			OnButtonGetBasePixelCount() ;
        }
        else if( point.x() >= groupBox_color_blue_histogram_x &&
                 point.x() <= groupBox_color_blue_histogram_x + ui->label_image_blue_histogram_2->width() &&
                 point.y() >= groupBox_color_blue_histogram_y &&
                 point.y() <= groupBox_color_blue_histogram_y + ui->label_image_blue_histogram_2->height()  )
        {
        	qDebug("Start Set Blue Range") ;
			
            m_i_set_color = HISTOGRAM_USE_BLUE ;
            m_i_s_value = point.x() - (groupBox_color_blue_x + ui->label_image_blue_histogram->x()) ;
			m_i_e_value = m_i_s_value ;
			
			qDebug("Blue Value : m_i_s_value = %d", m_i_s_value) ;
			
			float f_min = 0.0 ;
	        int label_w = ui->label_image_blue_histogram->width() ;
	        f_min = (float)m_i_s_value / (float)label_w ;

			qDebug("Blue Value : f_min = %f", f_min) ;
			
            CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_min, f_min) ;

			OnButtonGetBlueHistogramImage() ;
			OnButtonGetImage() ;

			OnButtonGetBlueHistogramRange() ;

			OnButtonGetBasePixelCount() ;
        }
        else
        {
            m_i_set_color = -1 ;
            m_i_s_value = -1 ;
            m_i_e_value = -1 ;
        }
    }
}

void DialogSetToolColor::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug("%s - %d", __func__, __LINE__) ;

    if ((event->buttons() & Qt::LeftButton) )
	{
		QPoint point = event->pos() ;
        //point.setX(point.x() - ui->label_image->x());
        //point.setY(point.y() - ui->label_image->y());

        if( m_i_set_color>=0)
        {
        	int groupBox_color_x = ui->groupBox_color->x() ;
			int groupBox_color_y = ui->groupBox_color->y() ;

			//qDebug("groupBox_color : %d, %d", groupBox_color_x, groupBox_color_y) ;
			
			int groupBox_color_red_x = groupBox_color_x + ui->groupBox_red->x() ;
			int groupBox_color_red_y = groupBox_color_y + ui->groupBox_red->y() ;

			//qDebug("groupBox_color_red : %d, %d", groupBox_color_red_x, groupBox_color_red_y) ;
			
			int groupBox_color_green_x = groupBox_color_x + ui->groupBox_green->x() ;
			int groupBox_color_green_y = groupBox_color_y + ui->groupBox_green->y() ;

			int groupBox_color_blue_x = groupBox_color_x + ui->groupBox_blue->x() ;
			int groupBox_color_blue_y = groupBox_color_y + ui->groupBox_blue->y() ;

			int groupBox_color_red_histogram_x = groupBox_color_red_x + ui->label_image_red_histogram_2->x() ;
			int groupBox_color_red_histogram_y = groupBox_color_red_y + ui->label_image_red_histogram_2->y() ;

			int groupBox_color_green_histogram_x = groupBox_color_green_x + ui->label_image_green_histogram_2->x() ;
			int groupBox_color_green_histogram_y = groupBox_color_green_y + ui->label_image_green_histogram_2->y() ;

			int groupBox_color_blue_histogram_x = groupBox_color_blue_x + ui->label_image_blue_histogram_2->x() ;
			int groupBox_color_blue_histogram_y = groupBox_color_blue_y + ui->label_image_blue_histogram_2->y() ;
			
            if( m_i_set_color == HISTOGRAM_USE_RED )
            {
            	//m_i_s_value = point.x() - (groupBox_color_red_x + ui->label_image_red_histogram->x()) ;
                m_i_e_value = point.x() - (groupBox_color_red_x + ui->label_image_red_histogram->x()) ;
				//qDebug("m_i_e_value = %d", m_i_e_value) ;
				//qDebug("m_i_e_value = %d, point.x()=%d, groupBox_color_red_x=%d, ui->label_image_red_histogram->x()=%d", m_i_e_value, point.x(), groupBox_color_red_x, ui->label_image_red_histogram->x()) ;
				
				if( m_i_e_value > m_i_s_value)
				{
					float f_min = 0.0 ;
					float f_max = 0.0 ;
		        	int label_w = ui->label_image_red_histogram->width() ;

					f_min = (float)m_i_s_value / (float)label_w ;
					f_max = (float)m_i_e_value / (float)label_w ;

					if( f_min < f_max )
					{
						CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_min, f_max) ;
					}
					else
					{
						CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_max, f_min) ;
					}			
					
					OnButtonGetRedHistogramImage() ;
					OnButtonGetImage() ;

					OnButtonGetRedHistogramRange() ;

					OnButtonGetBasePixelCount() ;
				}
            }
            else  if( m_i_set_color == HISTOGRAM_USE_GREEN )
            {
                m_i_e_value = point.x() - (groupBox_color_green_x + ui->label_image_green_histogram->x()) ;

				if( m_i_e_value > m_i_s_value)
				{
					float f_min = 0.0 ;
					float f_max = 0.0 ;
		        	int label_w = ui->label_image_green_histogram->width() ;

					f_min = (float)m_i_s_value / (float)label_w ;
					f_max = (float)m_i_e_value / (float)label_w ;

					if( f_min < f_max )
					{
						CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_min, f_max) ;
					}
					else
					{
						CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_max, f_min) ;
					}			

					OnButtonGetGreenHistogramImage() ;
					OnButtonGetImage() ;

					OnButtonGetGreenHistogramRange() ;

					OnButtonGetBasePixelCount() ;
				}
            }
            else  if( m_i_set_color == HISTOGRAM_USE_BLUE )
            {
                m_i_e_value = point.x() - (groupBox_color_blue_x + ui->label_image_blue_histogram->x()) ;

				if( m_i_e_value > m_i_s_value)
				{
					float f_min = 0.0 ;
					float f_max = 0.0 ;
		        	int label_w = ui->label_image_blue_histogram->width() ;

					f_min = (float)m_i_s_value / (float)label_w ;
					f_max = (float)m_i_e_value / (float)label_w ;

					if( f_min < f_max )
					{
						CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_min, f_max) ;
					}
					else
					{
						CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Histogram_Range(GetId(), m_i_set_color, f_max, f_min) ;
					}			

					OnButtonGetBlueHistogramImage() ;
					OnButtonGetImage() ;

					OnButtonGetBlueHistogramRange() ;

					OnButtonGetBasePixelCount() ;
				}
            }
        }
	}
}

void DialogSetToolColor::mouseReleaseEvent(QMouseEvent *event)
{
    m_i_set_color = -1 ;
    m_i_s_value = -1 ;
    m_i_e_value = -1 ;

	OnButtonGetRedHistogramRange() ;
	OnButtonGetGreenHistogramRange() ;
	OnButtonGetBlueHistogramRange() ;
}

void DialogSetToolColor::OnButtonGetBasePixelCount(void)
{
	int base_pixel_count = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Base_Pixel_Count(GetId()) ;

	ui->lineEdit_base_pixel_count->setText(QString::number(base_pixel_count));

	OnButtonGetTolerance() ;
}

void DialogSetToolColor::OnButtonGetTolerance(void)
{
	int tol_min = -1 ;
	int tol_max = -1 ;
	
	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Get_Tolerance(GetId(), &tol_min, &tol_max) ;

	ui->lineEdit_tolerance_min->setText(QString::number(tol_min));
	ui->lineEdit_tolerance_max->setText(QString::number(tol_max));
}

void DialogSetToolColor::OnButtonSetTolerance(void)
{
	int min = ui->lineEdit_tolerance_min->text().toInt();
	int max = ui->lineEdit_tolerance_max->text().toInt();

	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectColor_Set_Tolerance(GetId(), min, max) ;
	
	OnButtonGetTolerance() ;
}

int DialogSetToolColor::GetBaseValue(void)
{
	int base = ui->lineEdit_base_pixel_count->text().toInt();

	return base ;
}

int DialogSetToolColor::GetToleranceMin(void)
{
	int min = ui->lineEdit_tolerance_min->text().toInt();

	return min ;
}

int DialogSetToolColor::GetToleranceMax(void) 
{
	int max = ui->lineEdit_tolerance_max->text().toInt();

	return max ;
}

void DialogSetToolColor::OnComboGetPreset(int index)
{
	QString qstr_preset_rate = ui->comboBox_preset->currentText() ;

    int preset_rate = qstr_preset_rate.toInt() ;

	int base_count = ui->lineEdit_base_pixel_count->text().toInt();
	int margin = (int)((float)base_count*((float)preset_rate/100.0)) ;
	
	ui->lineEdit_tolerance_min->setText(QString::number(base_count - margin));
	ui->lineEdit_tolerance_max->setText(QString::number(base_count + margin));
}

