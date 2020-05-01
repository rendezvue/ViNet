#include "formtooloptionwithprogress.h"
#include "ui_formtooloptionwithprogress.h"

FormToolOptionWithProgress::FormToolOptionWithProgress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormToolOptionWithProgress)
{
    ui->setupUi(this);
}

FormToolOptionWithProgress::~FormToolOptionWithProgress()
{
    delete ui;
}
