#include "dialogcoding.h"
#include "ui_dialogcoding.h"

DialogCoding::DialogCoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCoding)
{
    ui->setupUi(this);

	connect(ui->pushButton_name_change, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	connect(ui->pushButton_upload, SIGNAL(clicked()), this,  SLOT(OnButtonUpload())) ;
	connect(ui->pushButton_download, SIGNAL(clicked()), this,  SLOT(OnButtonDownload())) ;
	connect(ui->pushButton_upload_and_run, SIGNAL(clicked()), this,  SLOT(OnButtonUploadAndRun())) ;
}

DialogCoding::~DialogCoding()
{
    delete ui;
}

void DialogCoding::OnButtonUpload(void)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;
}

void DialogCoding::OnButtonDownload(void)
{
}

void DialogCoding::OnButtonUploadAndRun(void)
{
}

void DialogCoding::SetId(const std::string id)
{
    m_str_id = id ;

    ui->label_id->setText(QString::fromUtf8(m_str_id.c_str()));
}

std::string DialogCoding::GetId(void)
{
    return m_str_id ;
}

void DialogCoding::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string base_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(base_name.c_str()));

}

void DialogCoding::OnButtonNameChange(void)
{
    std::string base_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;

    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetId());
    dlg_change_name.SetName(base_name);

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        std::string change_name = dlg_change_name.GetName() ;

        if( !change_name.empty() )
        {
            CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Name(GetId(), change_name) ;
        }

        base_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(base_name.c_str()));

		emit UpdateBaseName(QString::fromUtf8(base_name.c_str())) ;
    }
}

