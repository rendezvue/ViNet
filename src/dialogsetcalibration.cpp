#include "dialogsetcalibration.h"
#include "ui_dialogsetcalibration.h"

DialogSetCalibration::DialogSetCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCalibration)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_chess_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetChessInfo())) ;
	connect(ui->pushButton_get_calibration_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetCalibrationImage())) ;

	connect(ui->pushButton_add, SIGNAL(clicked()), this,  SLOT(OnButtonAddCalibrationInfo())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDelCalibrationInfo())) ;
	connect(ui->pushButton_clear, SIGNAL(clicked()), this,  SLOT(OnButtonClearCalibrationInfo())) ;
	connect(ui->pushButton_update, SIGNAL(clicked()), this,	SLOT(OnButtonUpdateCalibrationInfo())) ;	

	connect(ui->pushButton_calibration_run, SIGNAL(clicked()), this, SLOT(OnButtonCalibrationRun())) ;	
}

DialogSetCalibration::~DialogSetCalibration()
{
    delete ui;
}

void DialogSetCalibration::SetId(const std::string id)
{
    m_str_id = id ;

    ui->label_id_parent->setText(QString::fromUtf8(m_str_id.c_str()));
}

std::string DialogSetCalibration::GetId(void)
{
    return m_str_id ;
}

void DialogSetCalibration::showEvent(QShowEvent *ev) 
{
    QDialog::showEvent(ev) ;

	//Get Calibration Image
	OnButtonGetCalibrationImage() ;
	
	//Get Chesboard information
	OnButtonGetChessInfo() ;

	//Update Information List
	OnButtonUpdateCalibrationInfo() ;
}

void DialogSetCalibration::OnButtonGetChessInfo(void)
{
	int chess_x_num = 0 ;
	int chess_y_num = 0 ;
	int chess_square_mm_size = 0 ;

	Ensemble_Job_Calibration_Get_Chess_Info(GetId(), &chess_x_num, &chess_y_num, &chess_square_mm_size);

	ui->lineEdit_chess_x_num->setText(QString::number(chess_x_num));
	ui->lineEdit_chess_y_num->setText(QString::number(chess_y_num));
	ui->lineEdit_chess_size->setText(QString::number(chess_square_mm_size));
}

void DialogSetCalibration::OnButtonGetCalibrationImage(void)
{
	cv::Mat calib_image = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display

	unsigned char* get_data = NULL ;
    int width = 640 ;
    int height = 480 ;

	const int image_type = IMAGE_RGB888 ;
	
    Ensemble_Source_Get_Image(GET_IMAGE_CALIBRATION_FEATURE, GetId(), image_type, &get_data, &width, &height) ;

    if( get_data != NULL )
    {
        if( width>0 && height >0 )
        {
			cv::Mat get_image(height, width, CV_8UC3, get_data) ;
			cv::cvtColor(get_image, calib_image, cv::COLOR_BGR2RGB) ;
        }
    	
        delete [] get_data ;
        get_data = NULL ;
    }

	QImage qt_display_image = QImage((const unsigned char*)calib_image.data, calib_image.cols, calib_image.rows, QImage::Format_RGB888);
	ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetCalibration::OnButtonAddCalibrationInfo(void)
{
	float f_robot_x = ui->lineEdit_robot_x->text().toFloat();
	float f_robot_y = ui->lineEdit_robot_y->text().toFloat();
	
	Ensemble_Job_Calibration_Add(GetId(), f_robot_x, f_robot_y);

	OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonDelCalibrationInfo(void)
{
	int select_index = ui->listWidget_calibration->currentRow() ;

    Ensemble_Job_Calibration_Del(GetId(), select_index);

    OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonClearCalibrationInfo(void)
{
    Ensemble_Job_Calibration_Clear(GetId());

	OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonUpdateCalibrationInfo(void)
{
	ui->listWidget_calibration->clear();
	
	//calibration image list
	int nCalibrationInfo = Ensemble_Job_Calibration_GetCount(GetId());

    qDebug("Calibration count = %d", nCalibrationInfo) ;

	for (int i = 0; i < nCalibrationInfo; i++)
	{
        unsigned char* get_data = NULL ;
        int width = 640 ;
        int height = 480 ;
		const int image_type = IMAGE_RGB888 ;
		
		Ensemble_Job_Calibration_GetImage(GetId(), i, image_type, &get_data, &width, &height) ;

		cv::Mat calibration_image ;
        if( get_data != NULL )
        {
            if( width>0 && height >0 )
            {
                cv::Mat get_image(height, width, CV_8UC3, get_data) ;
                cv::cvtColor(get_image, calibration_image, cv::COLOR_BGR2RGB) ;
            }

            delete [] get_data ;
            get_data = NULL ;
        }

		float robot_x = 0.0, robot_y = 0.0;
		Ensemble_Job_Calibration_GetRobotInfo(GetId(), i, &robot_x, &robot_y);

		cv::Mat image;
		cv::resize(calibration_image, image, cv::Size(120, 90));

		//calibration_image.release() ;

		QImage qt_calibration_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);

        std::string str ;
        str = "Pos(" + std::to_string(robot_x) + "," + std::to_string(robot_y) + ")" ;
		//Add Image List
        ui->listWidget_calibration->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(qt_calibration_image)), str.c_str()));
	}
}

void DialogSetCalibration::OnButtonCalibrationRun(void)
{
	Ensemble_Job_Calibration_Run(GetId());

	UpdateDataCalibrationRun(0.5, 0.5);
}

void DialogSetCalibration::UpdateDataCalibrationRun(float f_pixel_x, float f_pixel_y)
{
	//check calibration result
	float robot_x = 0;
	float robot_y = 0;
	Ensemble_Job_Calibration_GetPoint(GetId(), f_pixel_x, f_pixel_y, &robot_x, &robot_y);

	std::string str_info ;
	char cstr_info[255] ;
	sprintf(cstr_info, "(%d,%d)pixel -> (%.2f,%.2f)mm", (int)(640.0*f_pixel_x), (int)(480.0*f_pixel_y), robot_x, robot_y) ;
	str_info = cstr_info ;
	ui->label_calibration_info->setText(QString::fromUtf8(str_info.c_str()));
}

void DialogSetCalibration::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		int label_w = ui->label_image->width() ;
        int label_h = ui->label_image->height() ;

       	float pixel_x = (float)point.x() / (float)label_w ;
        float pixel_y = (float)point.y() / (float)label_h ;

		UpdateDataCalibrationRun(pixel_x, pixel_y) ;
    }
}

