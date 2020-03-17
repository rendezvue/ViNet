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

	const int type = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Get_Type(GetId()) ;
	int inspect_level = -1 ;
	
	//Get Level 
	if( type == TOOL_TYPE_OPTION_INSPECT_CRACK || type == TOOL_TYPE_OPTION_INSPECT_CRACK2 )
	{
    	inspect_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_Crack_Get_InspectLevel(GetId());
	}
	else if( type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
	{
		inspect_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_ColorCompare_Get_InspectLevel(GetId());
	}
	
	//Set Slider
	ui->horizontalSlider_feature_level->setValue(inspect_level) ;
	ui->label_feature_level->setText(QString::number(inspect_level));
}

void DialogSetToolCrack::OnSliderSetFeatureLevel(void)
{
	const int type = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Get_Type(GetId()) ;

	//get level
    int level = ui->horizontalSlider_feature_level->value() ;
	//Get Level 
	int inspect_level = -1 ;
	    
	if( type == TOOL_TYPE_OPTION_INSPECT_CRACK || type == TOOL_TYPE_OPTION_INSPECT_CRACK2 )
	{
		//set level
	    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_Crack_Set_InspectLevel(GetId(), level);
	
		//Get Level 
	    inspect_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_Crack_Get_InspectLevel(GetId());
	}
	else if( type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
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

void DialogSetToolCrack::OnSliderMove(int value)
{
    ui->label_feature_level->setText(QString::number(value));
}

