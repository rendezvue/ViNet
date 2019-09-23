#include "dialogsettoolangle.h"
#include "ui_dialogsettoolangle.h"

DialogSetToolAngle::DialogSetToolAngle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolAngle)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_name_change_angle, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//get calc angle
	connect(ui->pushButton_get_calc_angle, SIGNAL(clicked()), this,  SLOT(OnButtonGetCalcAngle())) ;
}

DialogSetToolAngle::~DialogSetToolAngle()
{
    delete ui;
}

void DialogSetToolAngle::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id_angle->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolAngle::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = EnsembleToolGetName(GetId()) ;
    ui->label_name_angle->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;

	//Calc Distance
    OnButtonGetCalcAngle() ;
	
	//Tolerance
    //OnButtonGetTolRate() ;

	//Image
	OnButtonGetImage() ;
}

void DialogSetToolAngle::OnButtonGetImage(void)
{
    //Get Base Job Image
    unsigned char* get_job_image_data = NULL ;
    int job_image_width = 0 ;
    int job_image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
	
    EnsembleToolGetImage(GetId(), image_type, &get_job_image_data, &job_image_width, &job_image_height)  ;

    if( job_image_width > 0 && job_image_height > 0 )
    {
    	if( image_type == IMAGE_YUV420)
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

void DialogSetToolAngle::updatePicture(cv::Mat image)
{
    QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

#if 0
void DialogSetToolAngle::OnButtonGetTolRate(void)
{
	//Tolerance
	float tol_rate = EnsembleToolGetDistanceToleranceRate(GetParentId(), GetId()) ; 

	ui->lineEdit_dist_tolerance->setText(QString::number(tol_rate));
}
#endif

void DialogSetToolAngle::OnButtonGetCalcAngle(void)
{
	//Tolerance
    float angle = Ensemble_Tool_Angle_Get_CalcAngle(GetId()) ;

	ui->lineEdit_get_calc_angle->setText(QString::number(angle));
}

void DialogSetToolAngle::OnButtonNameChange(void)
{
    std::string tool_name = EnsembleToolGetName(GetId()) ;

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
            EnsembleToolSetName(GetId(), change_name) ;
        }

        tool_name = EnsembleToolGetName(GetId()) ;
        ui->label_name_angle->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}


