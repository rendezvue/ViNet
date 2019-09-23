#include "dialogchangename.h"
#include "ui_dialogchangename.h"

DialogChangeName::DialogChangeName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChangeName)
{
    ui->setupUi(this);
}

DialogChangeName::~DialogChangeName()
{
    delete ui;
}

void DialogChangeName::SetId(const std::string id)
{
    ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

void DialogChangeName::SetName(const std::string name)
{
    ui->lineEdit_name->setText(QString::fromUtf8(name.c_str()));
}

std::string DialogChangeName::GetName(void)
{
    QString name = ui->lineEdit_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}
