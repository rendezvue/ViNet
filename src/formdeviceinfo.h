#ifndef FORMDEVICEINFO_H
#define FORMDEVICEINFO_H

#include <QWidget>
#include <QImage>

#include "dialogbasecameraconfig.h"
#include "dialogdblist.h"
#include "dialogcheckforupdates.h"
#include "cdialognewproject.h"


namespace Ui {
class FormDeviceInfo;
}

class FormDeviceInfo : public QWidget
{
    Q_OBJECT

public:
    explicit FormDeviceInfo(QWidget *parent = 0);
    ~FormDeviceInfo();

    void SetDeviceNameInfo(const std::string name) ;
    std::string GetDeviceNameInfo(void) ;

    void SetMacAddressInfo(const std::string mac, const std::string ip, const std::string port) ;
    std::string GetMacAddressInfo(void) ;

    void SetIconInfo(QImage image) ;

private:
    Ui::FormDeviceInfo *ui;

public slots:
	void OnButtonBaseCameraConfig(void) ;
	void OnButton_DB_Load(void) ;
	void OnButton_DB_Save(void) ;
	void OnButton_Job_Clear(void) ;
	void OnButton_Add_New_Project(void) ;
	void OnButton_Device_Update(void) ;
	
signals:
	void signal_Change_Task(void);

};

#endif // FORMDEVICEINFO_H
