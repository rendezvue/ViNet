#include "dialogsetcode.h"
#include "ui_dialogsetcode.h"

DialogSetCode::DialogSetCode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCode)
{
    ui->setupUi(this);

	
	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
	connect(ui->pushButton_name_change, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//button
	connect(ui->pushButton_padding_rate_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetPaddingRate())) ;
	connect(ui->pushButton_padding_rate_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetPaddingRate())) ;

	//background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
	ui->label_image_bg_code->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetCode::~DialogSetCode()
{
    delete ui;
}

void DialogSetCode::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetCode::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;

	//Image
	OnButtonGetImage() ;

	//padding rate
	OnButtonGetPaddingRate() ;
}

void DialogSetCode::OnButtonGetImage(void)
{
    //Get Base Job Image
    unsigned char* get_job_image_data = NULL ;
    int job_image_width = 0 ;
    int job_image_height = 0 ;

    int image_type = IMAGE_RGB888 ;
    int get_image_type = 0 ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_Image(GetId(), image_type, &get_job_image_data, &job_image_width, &job_image_height, &get_image_type)  ;

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
        else if( get_image_type == IMAGE_JPG )
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, job_image_height*job_image_width, CV_8UC1, get_job_image_data), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, m_image, cv::COLOR_BGR2RGB) ;
        }
		
        updatePicture(m_image) ;
    }

    if( get_job_image_data != NULL )
    {
        delete [] get_job_image_data ;
        get_job_image_data = NULL ;
    }

	//object image : code imae
    unsigned char* get_object_image_data = NULL ;
    int object_image_width = 0 ;
    int object_image_height = 0 ;

    image_type = IMAGE_RGB888 ;
    image_type += IMAGE_ORI_SIZE ;
    get_image_type = 0 ;
    int ret_image_size = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_ObjectImage(GetId(), image_type, &get_object_image_data, &object_image_width, &object_image_height, &get_image_type)  ;

	cv::Mat object_image ;
	if( object_image_width > 0 && object_image_height > 0 )
	{
        qDebug("object image test : object_image_width(%d), object_image_height(%d), ret_image_size(%d)", object_image_width, object_image_height, ret_image_size) ;

        if( get_image_type == IMAGE_YUV420 )
		{
			//YUV420 
	        cv::Mat get_image(object_image_height + object_image_height / 2, object_image_width, CV_8UC1, get_object_image_data) ;

	        CImgDec cls_image_decoder ;
	        object_image = cls_image_decoder.Decoding(get_image) ;

		}
        else if( get_image_type == IMAGE_RGB888 )
		{
            //cv::Mat get_image(object_image_height, object_image_width, CV_8UC3, get_object_image_data) ;
            cv::Mat get_image(object_image_height, object_image_width, CV_8UC3) ;
            get_image.data = get_object_image_data ;

			cv::cvtColor(get_image, object_image, cv::COLOR_BGR2RGB) ;
		}
        else if( get_image_type == IMAGE_JPG )
        {
            cv::Mat get_image = cv::imdecode(cv::Mat(1, object_image_height*object_image_width, CV_8UC1, get_object_image_data), cv::IMREAD_UNCHANGED) ;
            cv::cvtColor(get_image, object_image, cv::COLOR_BGR2RGB) ;
        }

		SetObjectImage(object_image) ;

		emit UpdateToolObjectImage();
	}

    if( get_object_image_data != NULL )
    {
        delete [] get_object_image_data ;
        get_object_image_data = NULL ;
    }

	//Get Code Info
	std::string str_code_type = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Detect_Code_Get_Ref_CodeType(GetId()) ;
	std::string str_code_data = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Detect_Code_Get_Ref_CodeData(GetId()) ;

	std::string str_code_info ;
	str_code_info += "(" + str_code_type + ")" ;
	str_code_info += str_code_data ;
	
    ui->label_code->setText(QString::fromUtf8(str_code_info.c_str()));
	
}

void DialogSetCode::updatePicture(cv::Mat image, cv::Rect rect_user)
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

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetCode::SetObjectImage(cv::Mat image)
{
   	const int draw_width = ui->label_image_bg_code->width();
    const int draw_height = ui->label_image_bg_code->height();

    float rescale_w = (float)draw_width / (float)image.cols ;
    float rescale_h = (float)draw_height / (float)image.rows ;

    float min_rescale = std::fmin(rescale_w, rescale_h) ;
    if( min_rescale < 1.0 )
    {
        cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
    }

    //fit image label by image isze
    int pos_x = (int)((float)ui->label_image_bg_code->x() + (float)(draw_width - image.cols)/2.0) ;
    int pos_y = (int)((float)ui->label_image_bg_code->y() + (float)(draw_height - image.rows)/2.0) ;

    ui->label_image_code->setGeometry(pos_x, pos_y, image.cols, image.rows);

    //QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);
    CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image_code->width(), ui->label_image_code->height()) ;

    ui->label_image_code->setPixmap(QPixmap::fromImage(qt_display_image));
}


void DialogSetCode::OnButtonNameChange(void)
{
    std::string tool_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_Name(GetId()) ;

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
            CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Set_Name(GetId(), change_name) ;
        }

        tool_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}

void DialogSetCode::OnButtonGetPaddingRate(void)
{
	float padding_rate = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Detect_Code_Get_Padding(GetId()) ;

	ui->lineEdit_padding_rate->setText(QString::number(padding_rate)) ;
}

void DialogSetCode::OnButtonSetPaddingRate(void)
{
	QString qstr_padding_rate = ui->lineEdit_padding_rate->text() ;
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Detect_Code_Set_Padding(GetId(), qstr_padding_rate.toFloat()) ;

	OnButtonGetImage() ;
	OnButtonGetPaddingRate() ;
}

