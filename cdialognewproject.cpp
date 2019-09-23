#include "cdialognewproject.h"
#include "ui_cdialognewproject.h"

CDialogNewProject::CDialogNewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialogNewProject)
{
    ui->setupUi(this);

    //ui->lineEdit_name->setPlaceholderText(QString("<Default Random Name>"));
}

CDialogNewProject::~CDialogNewProject()
{
    delete ui;
}

std::string CDialogNewProject::GetName(void)
{
    QString name = ui->lineEdit_name->text() ;

    return name.toUtf8().constData();
}
