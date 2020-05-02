#include "dialogcoding.h"
#include "ui_dialogcoding.h"

DialogCoding::DialogCoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCoding)
{
    ui->setupUi(this);
}

DialogCoding::~DialogCoding()
{
    delete ui;
}
