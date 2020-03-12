#include "dialogsettoolinspectdistance.h"
#include "ui_dialogsettoolinspectdistance.h"

DialogSetToolInspectDistance::DialogSetToolInspectDistance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolInspectDistance)
{
    ui->setupUi(this);
}

DialogSetToolInspectDistance::~DialogSetToolInspectDistance()
{
    delete ui;
}

void DialogSetToolInspectDistance::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id_distance->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolInspectDistance::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name_distance->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;
}


