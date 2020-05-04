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

	//Camera Config
	connect(ui->pushButton_camera_config, SIGNAL(clicked()), this, SLOT(OnButtonCameraConfig())) ;	
	
	//list update
	connect(ui->pushButton_calibration_list_update, SIGNAL(clicked()), this, SLOT(OnButtonCalibrationCopyListUpdate())) ;		
	connect(ui->pushButton_calibration_copy, SIGNAL(clicked()), this, SLOT(OnButtonCalibrationCopy())) ;		
	
	
    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");

	m_calibration_copy_list_model = new QStringListModel(this); ;
}

DialogSetCalibration::~DialogSetCalibration()
{
	if( m_calibration_copy_list_model != NULL )
    {
        delete m_calibration_copy_list_model ;
        m_calibration_copy_list_model = NULL ;
    }
	
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

	//
	OnButtonCalibrationCopyListUpdate() ;
}

void DialogSetCalibration::OnButtonGetChessInfo(void)
{
	int chess_x_num = 0 ;
	int chess_y_num = 0 ;
    float chess_square_mm_size = 0 ;

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Get_Chess_Info(GetId(), &chess_x_num, &chess_y_num, &chess_square_mm_size);

	ui->lineEdit_chess_x_num->setText(QString::number(chess_x_num));
	ui->lineEdit_chess_y_num->setText(QString::number(chess_y_num));
	ui->lineEdit_chess_size->setText(QString::number(chess_square_mm_size));
}

void DialogSetCalibration::OnButtonGetCalibrationImage(void)
{
	cv::Mat calib_image = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display

    //unsigned char* get_data = NULL ;
    //int width = 640 ;
    //int height = 480 ;
    ImageBuf image ;
    image.image_width = 640 ;
    image.image_height = 480 ;

	const int image_type = IMAGE_RGB888 ;
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Source_Get_CalibrationImage(GetId(), image_type, &image) ;

    if( image.p_buf != NULL )
    {
        if( image.image_width>0 && image.image_height >0 )
        {
            if( image.image_type == ImageTypeOption::IMAGE_RGB888 )
            {
                cv::Mat get_image(image.image_height, image.image_width, CV_8UC3, image.p_buf) ;
                cv::cvtColor(get_image, calib_image, cv::COLOR_BGR2RGB) ;
            }
            else if( image.image_type == ImageTypeOption::IMAGE_JPG)
            {
                cv::Mat get_image = cv::imdecode(cv::Mat(1, image.image_width*image.image_height, CV_8UC1, image.p_buf), cv::IMREAD_UNCHANGED) ;
                cv::cvtColor(get_image, calib_image, cv::COLOR_BGR2RGB) ;
            }
        }
    	
        delete [] image.p_buf ;
        image.p_buf = NULL ;
    }

    const int draw_width = ui->label_image_bg->width();
    const int draw_height = ui->label_image_bg->height();

    float rescale_w = (float)draw_width / (float)calib_image.cols ;
    float rescale_h = (float)draw_height / (float)calib_image.rows ;

    float min_rescale = std::fmin(rescale_w, rescale_h) ;
    if( min_rescale < 1.0 )
    {
        cv::resize(calib_image, calib_image, cv::Size(), min_rescale, min_rescale) ;
    }

    //fit image label by image isze
    int pos_x = (int)((float)ui->label_image_bg->x() + (float)(draw_width - calib_image.cols)/2.0) ;
    int pos_y = (int)((float)ui->label_image_bg->y() + (float)(draw_height - calib_image.rows)/2.0) ;

    ui->label_image->setGeometry(pos_x, pos_y, calib_image.cols, calib_image.rows);

	//QImage qt_display_image = QImage((const unsigned char*)calib_image.data, calib_image.cols, calib_image.rows, QImage::Format_RGB888);
	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(calib_image, ui->label_image->width(), ui->label_image->height()) ;
	
	ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void DialogSetCalibration::OnButtonAddCalibrationInfo(void)
{
	float f_robot_x = ui->lineEdit_robot_x->text().toFloat();
	float f_robot_y = ui->lineEdit_robot_y->text().toFloat();
	
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Add(GetId(), f_robot_x, f_robot_y);

	OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonDelCalibrationInfo(void)
{
	int select_index = ui->listWidget_calibration->currentRow() ;

    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Del(GetId(), select_index);

    OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonClearCalibrationInfo(void)
{
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Clear(GetId());

	OnButtonUpdateCalibrationInfo();
}

void DialogSetCalibration::OnButtonUpdateCalibrationInfo(void)
{
	ui->listWidget_calibration->clear();
	
	//calibration image list
	int nCalibrationInfo = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_GetCount(GetId());

    //qDebug("Calibration count = %d", nCalibrationInfo) ;

	for (int i = 0; i < nCalibrationInfo; i++)
	{
        //unsigned char* get_data = NULL ;
        //int width = 640 ;
        //int height = 480 ;
        ImageBuf image_buf ;
        image_buf.image_width = 640 ;
        image_buf.image_height = 640 ;

		const int image_type = IMAGE_RGB888 ;
        //int get_image_type = 0 ;
        CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_GetImage(GetId(), i, image_type, &image_buf) ;

		CImageBuf2Mat cls_imagebuf2mat ;
        cv::Mat calibration_image = cls_imagebuf2mat.Cvt(image_buf) ;

        if( image_buf.p_buf != NULL )
        {
            delete [] image_buf.p_buf ;
            image_buf.p_buf = NULL ;
        }

		float robot_x = 0.0, robot_y = 0.0;
		CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_GetRobotInfo(GetId(), i, &robot_x, &robot_y);

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
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Run(GetId());

	UpdateDataCalibrationRun(0.5, 0.5);
}

void DialogSetCalibration::UpdateDataCalibrationRun(float f_pixel_x, float f_pixel_y)
{
	//check calibration result
	float robot_x = 0;
	float robot_y = 0;
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_GetPoint(GetId(), f_pixel_x, f_pixel_y, &robot_x, &robot_y);

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

void DialogSetCalibration::OnButtonCalibrationCopyListUpdate(void)
{
	//clear list data
    ui->listView_list_calibration_copy->reset();
	
	//Get Parent Tree Info.
	std::string str_xml = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Parent_Tree(GetId()) ;
	
	//XML Parsing and Find Project ID string
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_xml.c_str()));

	std::string str_projct_id ;
	
    if (!result)
    {
        //qDebug("DialogSetCalibration::OnButtonCalibrationCopyListUpdate - xml parsing error") ;
    }
    else
    {
        for (pugi::xml_node project: doc.child("Root").children("Project"))
        {
        	std::string str_prj_id = project.attribute("ID").value();
			
			for (pugi::xml_node me: project.children("Me"))
        	{
				str_projct_id = str_prj_id ;
				break ;
    	    }	
        }
    }

	if( !str_projct_id.empty() )
	{
		std::string str_xml_has_job_info = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Project_Get_Has_Job_Info(str_projct_id) ;
	
		//XML Parsing and Find Project ID string
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string((char *)(str_xml_has_job_info.c_str()));

		if (!result)
	    {
	        //qDebug("DialogSetCalibration::OnButtonCalibrationCopyListUpdate - str_xml_has_job_info parsing error") ;
	    }
	    else
	    {
	    	QStringList stringListSource;
			
			for (pugi::xml_node job: doc.child("Jobs").children("Job"))
			{
				std::string str_job_id = job.attribute("ID").value();

				if( CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_isOK(str_job_id) )		//check calibration 
				{
                    stringListSource << str_job_id.c_str() ;
				}
			}
		
	        // Create model
	        m_calibration_copy_list_model->setStringList(stringListSource);

	        // Glue model and view together
	        ui->listView_list_calibration_copy->setModel(m_calibration_copy_list_model);
	    }
	}
	
}

void DialogSetCalibration::OnButtonCalibrationCopy(void)
{
	QModelIndex index = ui->listView_list_calibration_copy->currentIndex();
	QString itemText = index.data(Qt::DisplayRole).toString();

	std::string str_item_text = itemText.toUtf8().constData();

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Copy(GetId(), str_item_text) ;

	//Get Chesboard information
	OnButtonGetChessInfo() ;

	//Update Information List
	OnButtonUpdateCalibrationInfo() ;	

	//
	OnButtonCalibrationCopyListUpdate() ;
}

void DialogSetCalibration::OnButtonCameraConfig(void)
{
	DialogBaseCameraConfig dlg_camera_config ;

	dlg_camera_config.SetId(GetId()) ;

	int dialogCode = dlg_camera_config.exec();

    if(dialogCode == QDialog::Accepted)
    { 
    }
}



void DialogSetCalibration::on_pushButton_chess_set_clicked()
{

    int chess_x_num = ui->lineEdit_chess_x_num->text().toInt();
    int chess_y_num = ui->lineEdit_chess_y_num->text().toInt();
    float chess_square_mm_size = ui->lineEdit_chess_size->text().toFloat();

    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Calibration_Set_Chess_Info(GetId(), chess_x_num, chess_y_num, chess_square_mm_size);
}
