#include "dialogsetcustomfeatureoption.h"
#include "ui_dialogsetcustomfeatureoption.h"

DialogSetCustomFeatureOption::DialogSetCustomFeatureOption(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetCustomFeatureOption)
{
    ui->setupUi(this);

    //button
    //pushButton_get
    connect(ui->pushButton_get, SIGNAL(clicked()), this,  SLOT(OnButtonGet())) ;
	connect(ui->pushButton_set, SIGNAL(clicked()), this,  SLOT(OnButtonSet())) ;
}

DialogSetCustomFeatureOption::~DialogSetCustomFeatureOption()
{
    delete ui;
}

void DialogSetCustomFeatureOption::SetId(const std::string id)
{
	m_str_id = id ;
}


void DialogSetCustomFeatureOption::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    OnButtonGet() ;
}

void DialogSetCustomFeatureOption::OnButtonGet(void)
{
	//Get Option Value
    int option_blur = 0 ;
	int option_thre_upper = 0 ;
	int option_thre_lower = 0 ;
	int option_kernel_size = 0 ;

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Get_Feature_Option(m_str_id, &option_blur, &option_thre_upper, &option_thre_lower, &option_kernel_size) ;

	ui->lineEdit_blur->setText(QString::number(option_blur)) ;
	ui->lineEdit_upper_thre->setText(QString::number(option_thre_upper)) ;
	ui->lineEdit_lower_thre->setText(QString::number(option_thre_lower)) ;
	ui->lineEdit_kernel_size->setText(QString::number(option_kernel_size)) ;
}

void DialogSetCustomFeatureOption::OnButtonSet(void)
{
	int option_blur = ui->lineEdit_blur->text().toInt() ;
	int option_thre_upper = ui->lineEdit_upper_thre->text().toInt() ; ;
	int option_thre_lower = ui->lineEdit_lower_thre->text().toInt() ; ;
	int option_kernel_size = ui->lineEdit_kernel_size->text().toInt() ; ;
	
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Set_Feature_Option(m_str_id, option_blur, option_thre_upper, option_thre_lower, option_kernel_size) ;

	OnButtonGet() ;
}

