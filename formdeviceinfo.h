#ifndef FORMDEVICEINFO_H
#define FORMDEVICEINFO_H

#include <QWidget>
#include <QImage>

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

    void SetMacAddressInfo(const std::string mac) ;
    std::string GetMacAddressInfo(void) ;

    void SetIconInfo(QImage image) ;

private:
    Ui::FormDeviceInfo *ui;
};

#endif // FORMDEVICEINFO_H
