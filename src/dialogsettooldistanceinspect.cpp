#include "dialogsettooldistanceinspect.h"
#include "ui_dialogsettooldistanceinspect.h"

DialogSetToolDistanceInspect::DialogSetToolDistanceInspect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolDistanceInspect)
{
    ui->setupUi(this);

	connect(ui->pushButton_get_base_distance, SIGNAL(clicked()), this,  SLOT(OnButtonGetBaseDistance())) ;
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

DialogSetToolDistanceInspect::~DialogSetToolDistanceInspect()
{
    delete ui;
}

void DialogSetToolDistanceInspect::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolDistanceInspect::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

	OnButtonGetBaseDistance() ;
	OnButtonGetTolerance() ;

	//ID
	std::string str_id_base = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_InspectDistance_Get_ID_Info_Base(GetId()) ;
	std::string str_id_target = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_InspectDistance_Get_ID_Info_Target(GetId()) ;

	ui->label_id_base->setText(QString::fromStdString(str_id_base));
	ui->label_id_target->setText(QString::fromStdString(str_id_target));
}

void DialogSetToolDistanceInspect::OnButtonGetBaseDistance(void)
{
	//
	float distance = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Distance_Get_CalcDistance(GetParentId()) ;

	ui->lineEdit_base_distance->setText(QString::number(distance));
}

void DialogSetToolDistanceInspect::OnComboGetPreset(int index)
{
	QString qstr_preset_rate = ui->comboBox_preset->currentText() ;

    float preset_rate = qstr_preset_rate.toFloat() ;

    float base_distance = ui->lineEdit_base_distance->text().toFloat();
    float margin = ((float)base_distance*((float)preset_rate/100.0)) ;

	ui->lineEdit_tolerance_min->setText(QString::number(base_distance - margin));
	ui->lineEdit_tolerance_max->setText(QString::number(base_distance + margin));
}

void DialogSetToolDistanceInspect::OnButtonGetTolerance(void)
{
	float tol_min = -1 ;
	float tol_max = -1 ;
	
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_InspectDistance_Get_Tolerance(GetId(), &tol_min, &tol_max) ;

	ui->lineEdit_tolerance_min->setText(QString::number(tol_min));
	ui->lineEdit_tolerance_max->setText(QString::number(tol_max));
}

void DialogSetToolDistanceInspect::OnButtonSetTolerance(void)
{
	float min = ui->lineEdit_tolerance_min->text().toFloat();
	float max = ui->lineEdit_tolerance_max->text().toFloat();

	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_InspectDistance_Set_Tolerance(GetId(), min, max) ;
	
	OnButtonGetTolerance() ;
}

float DialogSetToolDistanceInspect::GetBaseValue(void)
{
	float base = ui->lineEdit_base_distance->text().toFloat();

	return base ;
}

float DialogSetToolDistanceInspect::GetToleranceMin(void)
{
	float min = ui->lineEdit_tolerance_min->text().toFloat();

	return min ;
}

float DialogSetToolDistanceInspect::GetToleranceMax(void) 
{
	float max = ui->lineEdit_tolerance_max->text().toFloat();

	return max ;
}

