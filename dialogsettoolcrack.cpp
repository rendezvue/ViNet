#include "dialogsettoolcrack.h"
#include "ui_dialogsettoolcrack.h"

DialogSetToolCrack::DialogSetToolCrack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolCrack)
{
    ui->setupUi(this);

	//slider
	connect(ui->horizontalSlider_feature_level, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetFeatureLevel()));
	connect(ui->horizontalSlider_feature_level, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMove(int)));
}

DialogSetToolCrack::~DialogSetToolCrack()
{
    delete ui;
}

void DialogSetToolCrack::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolCrack::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

	//Get Level 
    int inspect_level = Ensemble_Tool_Option_Crack_GetInspectLevel(GetId());
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(inspect_level) ;
	ui->label_feature_level->setText(QString::number(inspect_level));
}

void DialogSetToolCrack::OnSliderSetFeatureLevel(void)
{
    //get level
    int level = ui->horizontalSlider_feature_level->value() ;

	qDebug("%s : SetInspectLevel = %d", __func__, level) ;
	//set level
    Ensemble_Tool_Option_Crack_SetInspectLevel(GetId(), level);

	qDebug("%s : GetInspectLevel", __func__) ;
	
	//Get Level 
    int inspect_level = Ensemble_Tool_Option_Crack_GetInspectLevel(GetId());

	qDebug("%s : GetFeatureLevel = %d", __func__, inspect_level) ;
	
	//Set Slider
    ui->horizontalSlider_feature_level->setValue(inspect_level) ;
	ui->label_feature_level->setText(QString::number(inspect_level));

	//emit UpdateToolObjectImage();
	std::string	str_info = "Crack Level : " + std::to_string(inspect_level) ;		
	emit UpdateInfo(QString::fromUtf8(str_info.c_str())) ;
}

void DialogSetToolCrack::OnSliderMove(int value)
{
    ui->label_feature_level->setText(QString::number(value));
}

