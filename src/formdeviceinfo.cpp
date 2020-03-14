#include "formdeviceinfo.h"
#include "ui_formdeviceinfo.h"

FormDeviceInfo::FormDeviceInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceInfo)
{
    ui->setupUi(this);

	//buttton
	connect(ui->pushButton_camera_config, SIGNAL(clicked()), this, SLOT(OnButtonBaseCameraConfig())) ;	
	connect(ui->pushButton_db_load, SIGNAL(clicked()), this, SLOT(OnButton_DB_Load())) ;	
	connect(ui->pushButton_job_clear, SIGNAL(clicked()), this, SLOT(OnButton_Job_Clear())) ;	
	
}

FormDeviceInfo::~FormDeviceInfo()
{
    delete ui;
}

void FormDeviceInfo::SetDeviceNameInfo(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormDeviceInfo::GetDeviceNameInfo(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormDeviceInfo::SetMacAddressInfo(const std::string mac, const std::string ip, const std::string port)
{
	ui->label_mac->setText(QString::fromUtf8(mac.c_str()));
	ui->label_ip->setText(QString::fromUtf8(ip.c_str()));
	ui->label_port->setText(QString::fromUtf8(port.c_str()));
}

std::string FormDeviceInfo::GetMacAddressInfo(void)
{
    QString mac = ui->label_mac->text() ;

    std::string str_mac = mac.toUtf8().constData();

    return str_mac ;
}

void FormDeviceInfo::SetIconInfo(QImage image)
{
    QSize label_icon_size = ui->label_icon->size() ;
    //QSize image_icon_size = image.size();

    QImage icon = image.scaled(label_icon_size.width(), label_icon_size.height(), Qt::KeepAspectRatio ) ;

	//CMat2QImage cls_mat_2_qimage ;
	//QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_icon->width(), ui->label_icon->height()) ;
	
    ui->label_icon->setPixmap(QPixmap::fromImage(icon));
}

void FormDeviceInfo::OnButtonBaseCameraConfig(void)
{
	 //New Job Dialog
    DialogBaseCameraConfig dlg_camera_config_dialog ;

	dlg_camera_config_dialog.SetId("ini") ;
	
    int dialogCode = dlg_camera_config_dialog.exec();

    if(dialogCode == QDialog::Accepted)
    { 
    } 
}

void FormDeviceInfo::OnButton_DB_Load(void) 
{
	QString qstr_ip = ui->label_ip->text() ;
    std::string str_ip = qstr_ip.toUtf8().constData();

	QString qstr_port = ui->label_port->text() ;
    std::string str_port = qstr_port.toUtf8().constData();

	qDebug("OnButton_DB_Load = %s, %s", str_ip.c_str(), str_port.c_str()) ;
	
	if( !str_ip.empty() && !str_port.empty() )
	{
		int port = std::stoi(str_port) ;

		CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

		if( p_device )
		{
			DialogDbList dlg_db_list ;

			dlg_db_list.SetNetworkInfo(str_ip, port) ;

			int dialogCode = dlg_db_list.exec();

			if(dialogCode == QDialog::Accepted)
			{ // YesButton clicked
				std::vector<std::string> vec_db = dlg_db_list.Get_Sel_DB() ;

				const int db_size = vec_db.size() ;

				if( db_size > 0 )
				{
					//load
                    p_device->Ensemble_Task_File_Load(vec_db) ;

	                emit signal_Change_Task() ;
				}
			}
		}
	}
}

void FormDeviceInfo::OnButton_Job_Clear(void)
{
	QString qstr_ip = ui->label_ip->text() ;
    std::string str_ip = qstr_ip.toUtf8().constData();

	QString qstr_port = ui->label_port->text() ;
    std::string str_port = qstr_port.toUtf8().constData();

	qDebug("OnButton_Job_Clear = %s, %s", str_ip.c_str(), str_port.c_str()) ;
	
	if( !str_ip.empty() && !str_port.empty() )
	{
		int port = std::stoi(str_port) ;

		CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

		if( p_device )
		{
			p_device->Ensemble_Task_Clear() ;

			emit signal_Change_Task() ;
		}
	}
}


