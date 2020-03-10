#include "dialogsettooldistance.h"
#include "ui_dialogsettooldistance.h"

DialogSetToolDistance::DialogSetToolDistance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolDistance)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_name_change_distance, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//get calc distance
	connect(ui->pushButton_get_calc_distance, SIGNAL(clicked()), this,  SLOT(OnButtonGetCalcDistance())) ;

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetToolDistance::~DialogSetToolDistance()
{
    delete ui;
}

void DialogSetToolDistance::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id_distance->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolDistance::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name_distance->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;

	//Calc Distance
	OnButtonGetCalcDistance() ;
	
	//Tolerance
    //OnButtonGetTolRate() ;

	//Image
	OnButtonGetImage() ;
}

void DialogSetToolDistance::OnButtonGetImage(void)
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
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Get_Image(GetId(), image_type, &image_buf)  ;

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

void DialogSetToolDistance::updatePicture(cv::Mat image)
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

#if 0
void DialogSetToolDistance::OnButtonGetTolRate(void)
{
	//Tolerance
	float tol_rate = EnsembleToolGetDistanceToleranceRate(GetParentId(), GetId()) ; 

	ui->lineEdit_dist_tolerance->setText(QString::number(tol_rate));
}
#endif

void DialogSetToolDistance::OnButtonGetCalcDistance(void)
{
	//Tolerance
    float distance = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Distance_Get_CalcDistance(GetId()) ;

	ui->lineEdit_get_calc_distance->setText(QString::number(distance));
}

void DialogSetToolDistance::OnButtonNameChange(void)
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
        ui->label_name_distance->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}
