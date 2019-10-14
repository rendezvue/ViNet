#include "dialogsetcustomfeatureoption.h"
#include "ui_dialogsetcustomfeatureoption.h"

DialogSetCustomFeatureOption::DialogSetCustomFeatureOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCustomFeatureOption)
{
    ui->setupUi(this);

    //button
    //pushButton_get
}

DialogSetCustomFeatureOption::~DialogSetCustomFeatureOption()
{
    delete ui;
}
