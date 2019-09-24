#include "dialogsetcalibration.h"
#include "ui_dialogsetcalibration.h"

DialogSetCalibration::DialogSetCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCalibration)
{
    ui->setupUi(this);

	//button
	connect(ui->pushButton_chess_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetChessInfo())) ;
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

	//Get Chesboard information
	OnButtonGetChessInfo() ;
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

	//calibration image list
	int nCalibrationInfo = Ensemble_Job_Calibration_GetCount(GetId());

	ui->listWidget_calibration->clear();
	for (int i = 0; i < nCalibrationInfo; i++)
	{
		cv::Mat calibration_image = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);

		int len = 640*480*3;
		Ensemble_Job_Calibration_GetImage(GetId(), i, (char**)&calibration_image.data, &len);
			
		float robot_x = 0.0, robot_y = 0.0;
		Ensemble_Job_Calibration_GetRobotInfo(GetId(), i, &robot_x, &robot_y);

		cv::Mat image;
		cv::resize(calibration_image, image, cv::Size(160, 120));

		QImage qt_calibration_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);

        std::string str ;
        str = "Pos(" + std::to_string(robot_x) + "," + std::to_string(robot_y) + ")" ;
		//Add Image List
        ui->listWidget_calibration->addItem(new QListWidgetItem(QIcon(QPixmap::fromImage(qt_calibration_image)), str.c_str()));
	}
}

