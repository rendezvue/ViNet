#include "formjobbasecode.h"
#include "ui_formjobbasecode.h"

FormJobBaseCode::FormJobBaseCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormJobBaseCode)
{
    ui->setupUi(this);
}

FormJobBaseCode::~FormJobBaseCode()
{
    delete ui;
}
