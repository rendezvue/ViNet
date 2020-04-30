#include "dialogsetcolorcompare.h"
#include "ui_dialogsetcolorcompare.h"

DialogSetColorCompare::DialogSetColorCompare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetColorCompare)
{
    ui->setupUi(this);

	//slider
	connect(ui->horizontalSlider_sensitivity, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetSensitivityLevel()));
	connect(ui->horizontalSlider_sensitivity, SIGNAL(sliderMoved(int)), this, SLOT(OnSensitivitySliderMove(int)));
	
	connect(ui->horizontalSlider_feature_level, SIGNAL(sliderReleased()), this, SLOT(OnSliderSetInspectThresholdLevel()));
	connect(ui->horizontalSlider_feature_level, SIGNAL(sliderMoved(int)), this, SLOT(OnInspectThresholdLevelSliderMove(int)));
}

DialogSetColorCompare::~DialogSetColorCompare()
{
    delete ui;
}

void DialogSetColorCompare::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetColorCompare::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

	const int type = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Get_Type(GetId()) ;
	int inspect_level = -1 ;
	int sensitivity_level = -1 ;
	
	//Get Level 
	if( type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
	{
		inspect_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Get_InspectLevel(GetId());
		sensitivity_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Get_Sensitivity(GetId());
	}
	
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(inspect_level) ;
	ui->label_feature_level->setText(QString::number(inspect_level));

	ui->horizontalSlider_sensitivity->setValue(sensitivity_level) ;
	ui->label_sensitivity_level->setText(QString::number(sensitivity_level));
}

void DialogSetColorCompare::OnSliderSetInspectThresholdLevel(void)
{
	const int type = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Get_Type(GetId()) ;

	//get level
    int level = ui->horizontalSlider_feature_level->value() ;
	//Get Level 
	int inspect_level = -1 ;
	    
	if( type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
	{
		//set level
	    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Set_InspectLevel(GetId(), level);
	
		//Get Level 
	    inspect_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Get_InspectLevel(GetId());
	}
	
	//Set Slider
    ui->horizontalSlider_feature_level->setValue(inspect_level) ;
	ui->label_feature_level->setText(QString::number(inspect_level));

	//emit UpdateToolObjectImage();
	std::string	str_info = "Level : " + std::to_string(inspect_level) ;		
	//emit UpdateInfo(QString::fromUtf8(str_info.c_str())) ;
}

void DialogSetColorCompare::OnInspectThresholdLevelSliderMove(int value)
{
    ui->label_feature_level->setText(QString::number(value));
}

void DialogSetColorCompare::OnSliderSetSensitivityLevel(void)
{
	const int type = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Get_Type(GetId()) ;

	//get level
    int level = ui->horizontalSlider_sensitivity->value() ;
	//Get Level 
	int inspect_level = -1 ;
	    
	if( type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
	{
		//set level
	    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Set_Sensitivity(GetId(), level);
	
		//Get Level 
	    inspect_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Get_Sensitivity(GetId());
	}
	
	//Set Slider
    ui->horizontalSlider_sensitivity->setValue(inspect_level) ;
	ui->label_sensitivity_level->setText(QString::number(inspect_level));

	//emit UpdateToolObjectImage();
	std::string	str_info = "Level : " + std::to_string(inspect_level) ;		
	//emit UpdateInfo(QString::fromUtf8(str_info.c_str())) ;
}

void DialogSetColorCompare::OnSensitivitySliderMove(int value)
{
	ui->label_sensitivity_level->setText(QString::number(value));
}

