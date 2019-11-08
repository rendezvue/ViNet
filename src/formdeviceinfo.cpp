#include "formdeviceinfo.h"
#include "ui_formdeviceinfo.h"

FormDeviceInfo::FormDeviceInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceInfo)
{
    ui->setupUi(this);

	connect(ui->pushButton_camera_config, SIGNAL(clicked()), this, SLOT(OnButtonBaseCameraConfig())) ;	
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

void FormDeviceInfo::SetMacAddressInfo(const std::string mac)
{
     ui->label_mac->setText(QString::fromUtf8(mac.c_str()));
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

