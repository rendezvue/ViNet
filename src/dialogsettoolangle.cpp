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

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
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
    std::string tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Name(GetId()) ;
    ui->label_name_angle->setText(QString::fromUtf8(tool_name.c_str()));

    //qDebug("Tool Name = %s", tool_name.c_str()) ;

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
    //unsigned char* get_job_image_data = NULL ;
    //int job_image_width = 0 ;
    //int job_image_height = 0 ;
    ImageBuf image_buf ;
    image_buf.image_width = 0 ;
    image_buf.image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
    //int get_image_type = 0 ;
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Image(GetId(), image_type, &image_buf)  ;

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

void DialogSetToolAngle::updatePicture(cv::Mat image)
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
    float angle = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Angle_Get_CalcAngle(GetId()) ;

	ui->lineEdit_get_calc_angle->setText(QString::number(angle));
}

void DialogSetToolAngle::OnButtonNameChange(void)
{
    std::string tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Name(GetId()) ;

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
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Set_Name(GetId(), change_name) ;
        }

        tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Name(GetId()) ;
        ui->label_name_angle->setText(QString::fromUtf8(tool_name.c_str()));

        //qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}


