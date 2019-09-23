#include "dialogsettoolangleinspect.h"
#include "ui_dialogsettoolangleinspect.h"

DialogSetToolAngleInspect::DialogSetToolAngleInspect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolAngleInspect)
{
    ui->setupUi(this);

	connect(ui->pushButton_get_base_angle, SIGNAL(clicked()), this,  SLOT(OnButtonGetBaseAngle())) ;
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

DialogSetToolAngleInspect::~DialogSetToolAngleInspect()
{
    delete ui;
}

void DialogSetToolAngleInspect::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolAngleInspect::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

	OnButtonGetBaseAngle() ;
	OnButtonGetTolerance() ;

	//ID
	std::string str_id_base = Ensemble_Tool_Option_InspectAngle_Get_ID_Info_Base(GetId()) ;
	std::string str_id_target = Ensemble_Tool_Option_InspectAngle_Get_ID_Info_Target(GetId()) ;

	ui->label_id_base->setText(QString::fromStdString(str_id_base));
	ui->label_id_target->setText(QString::fromStdString(str_id_target));
}

void DialogSetToolAngleInspect::OnButtonGetBaseAngle(void)
{
	//
	float angle = Ensemble_Tool_Angle_Get_CalcAngle(GetParentId()) ;

	ui->lineEdit_base_angle->setText(QString::number(angle));
}

void DialogSetToolAngleInspect::OnComboGetPreset(int index)
{
	QString qstr_preset_rate = ui->comboBox_preset->currentText() ;

    float preset_rate = qstr_preset_rate.toFloat() ;

    float base_angle = ui->lineEdit_base_angle->text().toFloat();
    float margin = ((float)base_angle*((float)preset_rate/100.0)) ;

    ui->lineEdit_tolerance_min->setText(QString::number(base_angle - margin));
    ui->lineEdit_tolerance_max->setText(QString::number(base_angle + margin));
}

void DialogSetToolAngleInspect::OnButtonGetTolerance(void)
{
	float tol_min = -1 ;
	float tol_max = -1 ;
	
	Ensemble_Tool_Option_InspectAngle_Get_Tolerance(GetId(), &tol_min, &tol_max) ;

	ui->lineEdit_tolerance_min->setText(QString::number(tol_min));
	ui->lineEdit_tolerance_max->setText(QString::number(tol_max));
}

void DialogSetToolAngleInspect::OnButtonSetTolerance(void)
{
	float min = ui->lineEdit_tolerance_min->text().toFloat();
	float max = ui->lineEdit_tolerance_max->text().toFloat();

	Ensemble_Tool_Option_InspectAngle_Set_Tolerance(GetId(), min, max) ;
	
	OnButtonGetTolerance() ;
}

float DialogSetToolAngleInspect::GetBaseValue(void)
{
	float base = ui->lineEdit_base_angle->text().toFloat();

	return base ;
}

float DialogSetToolAngleInspect::GetToleranceMin(void)
{
	float min = ui->lineEdit_tolerance_min->text().toFloat();

	return min ;
}

float DialogSetToolAngleInspect::GetToleranceMax(void) 
{
	float max = ui->lineEdit_tolerance_max->text().toFloat();

	return max ;
}



