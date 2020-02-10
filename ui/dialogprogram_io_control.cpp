#include "dialogprogram_io_control.h"
#include "ui_dialogprogram_io_control.h"

dialogprogram_io_control::dialogprogram_io_control(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogprogram_io_control)
{
    ui->setupUi(this);
}

dialogprogram_io_control::~dialogprogram_io_control()
{
    delete ui;
}
