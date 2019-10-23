#include "dialogresultinfo.h"
#include "ui_dialogresultinfo.h"

DialogResultInfo::DialogResultInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogResultInfo)
{
    ui->setupUi(this);
}

DialogResultInfo::~DialogResultInfo()
{
    delete ui;
}

void DialogResultInfo::SetIdInfo(const std::string id)
{
	ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

void DialogResultInfo::SetNameInfo(const std::string name)
{
	ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

void DialogResultInfo::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

    ui->textEdit->setText(QString::fromUtf8(m_str_result.c_str()));
}

void DialogResultInfo::SetResultString(const std::string result)
{
	m_str_result = result ;
}



