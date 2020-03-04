#include "dialogsettooldiameterinspect.h"
#include "ui_dialogsettooldiameterinspect.h"

DialogSetToolDiameterInspect::DialogSetToolDiameterInspect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolDiameterInspect)
{
    ui->setupUi(this);

	connect(ui->pushButton_get_base_diameter, SIGNAL(clicked()), this,  SLOT(OnButtonGetBaseDiameter())) ;
	connect(ui->pushButton_tolerance_get, SIGNAL(clicked()), this,  SLOT(OnButtonGetTolerance())) ;
	connect(ui->pushButton_tolerance_set, SIGNAL(clicked()), this,  SLOT(OnButtonSetTolerance())) ;
	
		
	ui->comboBox_preset->addItem("0");
	ui->comboBox_preset->addItem("10");
	ui->comboBox_preset->addItem("20");
	ui->comboBox_preset->addItem("30");
	ui->comboBox_preset->addItem("40");
	ui->comboBox_preset->addItem("50");

    connect(ui->comboBox_preset, SIGNAL(currentIndexChanged(int)), SLOT(OnComboGetPreset(int)));
}

DialogSetToolDiameterInspect::~DialogSetToolDiameterInspect()
{
    delete ui;
}

void DialogSetToolDiameterInspect::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolDiameterInspect::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

	OnButtonGetBaseDiameter() ;
	OnButtonGetTolerance() ;
}

void DialogSetToolDiameterInspect::OnButtonGetBaseDiameter(void)
{
	//
	float diameter = CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Circle_Get_CalcDiameter(GetParentId()) ;

	ui->lineEdit_base_diameter->setText(QString::number(diameter));
}

void DialogSetToolDiameterInspect::OnComboGetPreset(int index)
{
	QString qstr_preset_rate = ui->comboBox_preset->currentText() ;

    float preset_rate = qstr_preset_rate.toFloat() ;

    float base_diameter = ui->lineEdit_base_diameter->text().toFloat();
    float margin = ((float)base_diameter*((float)preset_rate/100.0)) ;

    ui->lineEdit_tolerance_min->setText(QString::number(base_diameter - margin));
    ui->lineEdit_tolerance_max->setText(QString::number(base_diameter + margin));
}

void DialogSetToolDiameterInspect::OnButtonGetTolerance(void)
{
	float tol_min = -1 ;
	float tol_max = -1 ;
	
    CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectDiameter_Get_Tolerance(GetId(), &tol_min, &tol_max) ;

	ui->lineEdit_tolerance_min->setText(QString::number(tol_min));
	ui->lineEdit_tolerance_max->setText(QString::number(tol_max));
}

void DialogSetToolDiameterInspect::OnButtonSetTolerance(void)
{
	float min = ui->lineEdit_tolerance_min->text().toFloat();
	float max = ui->lineEdit_tolerance_max->text().toFloat();

	CEnsemble::getInstance()->m_cls_api.Ensemble_Tool_Option_InspectDiameter_Set_Tolerance(GetId(), min, max) ;
	
	OnButtonGetTolerance() ;
}

float DialogSetToolDiameterInspect::GetBaseValue(void)
{
	float base = ui->lineEdit_base_diameter->text().toFloat();

	return base ;
}

float DialogSetToolDiameterInspect::GetToleranceMin(void)
{
	float min = ui->lineEdit_tolerance_min->text().toFloat();

	return min ;
}

float DialogSetToolDiameterInspect::GetToleranceMax(void) 
{
	float max = ui->lineEdit_tolerance_max->text().toFloat();

	return max ;
}




