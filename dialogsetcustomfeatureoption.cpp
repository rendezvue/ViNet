#include "dialogsetcustomfeatureoption.h"
#include "ui_dialogsetcustomfeatureoption.h"

DialogSetCustomFeatureOption::DialogSetCustomFeatureOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCustomFeatureOption)
{
    ui->setupUi(this);
}

DialogSetCustomFeatureOption::~DialogSetCustomFeatureOption()
{
    delete ui;
}
