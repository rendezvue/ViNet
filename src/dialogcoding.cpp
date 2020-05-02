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
	connect(ui->pushButton_update, SIGNAL(clicked()), this,  SLOT(OnButtonUpdate())) ;
}

DialogCoding::~DialogCoding()
{
    delete ui;
}

void DialogCoding::OnButtonUpload(void)
{
	std::string code ;
	code = ui->plainTextEdit_code->toPlainText().toStdString();;
	
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Python_Code(GetId(), code) ;

	//Get Code from Ensemble
	OnButtonUpdate() ;
}

void DialogCoding::OnButtonDownload(void)
{
	const std::string code = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Python_Code(GetId()) ;
	QString qstr_code = QString::fromUtf8(code.c_str());
	
	ui->plainTextEdit_code->setPlainText(qstr_code);
}

void DialogCoding::OnButtonUploadAndRun(void)
{
	//Code Upload
	OnButtonUpload() ;

	//Update
	OnButtonUpdate() ;
	//Download
	OnButtonDownload() ;
	
	//Run
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Run_Python_Code(GetId()) ;
}

void DialogCoding::OnButtonUpdate(void)
{
	//Get Code from Ensemble
	const std::string code = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Python_Code(GetId()) ;
	QString qstr_code = QString::fromUtf8(code.c_str());
	ui->plainTextEdit_code_ensemble->setPlainText(qstr_code);
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

	OnButtonUpdate() ;
	OnButtonDownload() ;
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

