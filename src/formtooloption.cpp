#include "formtooloption.h"
#include "ui_formtooloption.h"

FormToolOption::FormToolOption(QWidget *parent) :
    QWidget(parent),
	 m_type(-1),
    ui(new Ui::FormToolOption)
{
	setAttribute(Qt::WA_Hover);
	
    ui->setupUi(this);

	ui->pushButton_option_del->setVisible(false);
	ui->pushButton_option_set->setVisible(false);
	
	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);

	//button
    connect(ui->pushButton_option_del, SIGNAL(clicked()), this,  SLOT(OnButtonDel())) ;
	connect(ui->pushButton_option_set, SIGNAL(clicked()), this,  SLOT(OnButtonSet())) ;	

	//dialog crack signal
	//connect(&m_dlg_set_tool_crack, SIGNAL(UpdateInfo(QString)), this,  SLOT(OnBUpdateInfo(QString))) ;	

	//check box
	connect(ui->checkBox_run, SIGNAL(clicked(bool)), this, SLOT(OnRunCheckBoxToggled(bool)));
	connect(ui->checkBox_view, SIGNAL(clicked(bool)), this, SLOT(OnViewCheckBoxToggled(bool)));

}

FormToolOption::~FormToolOption()
{
    delete ui;
}

void FormToolOption::SetNameInfo(const std::string name)
{
    //ui->label_option_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormToolOption::GetNameInfo(void)
{
	std::string str_name ;
    //QString name = ui->label_option_name->text() ;

    //std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormToolOption::SetIdInfo(const std::string id)
{
    ui->label_option_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormToolOption::GetIdInfo(void)
{
    QString id = ui->label_option_id->text() ;

    std::string str_id = id.toUtf8().constData();

    return str_id ;
}

void FormToolOption::SetParentIdInfo(const std::string id)
{
	m_str_parent_id = id ;
}

std::string FormToolOption::GetParentIdInfo(void)
{
	return m_str_parent_id ;
}

void FormToolOption::SetTypeInfo(const std::string type)
{
    ui->label_option_type->setText(QString::fromUtf8(type.c_str()));
}

std::string FormToolOption::GetTypeInfo(void)
{
    QString type = ui->label_option_type->text() ;

    std::string str_type = type.toUtf8().constData();

    return str_type ;
}

void FormToolOption::SetType(const int type)
{
	m_type = type ;
}

int FormToolOption::GetType(void)
{
	return m_type ;
}


void FormToolOption::hoverEnter(QHoverEvent * event)
{
	ui->pushButton_option_del->setVisible(true);
	ui->pushButton_option_set->setVisible(true);

	//ui->checkBox_run->setVisible(true);
    //ui->checkBox_view->setVisible(true);
}

void FormToolOption::hoverLeave(QHoverEvent * event)
{
	ui->pushButton_option_del->setVisible(false);
	ui->pushButton_option_set->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
}

void FormToolOption::hoverMove(QHoverEvent * event)
{
}

bool FormToolOption::event(QEvent * e)
{
    switch(e->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(e));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

void FormToolOption::enterEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormToolOption::leaveEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormToolOption::OnButtonDel(void)
{
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Del_Option(GetIdInfo()) ;

	emit UpdateList() ;
}

void FormToolOption::OnButtonSet(void)
{
	const int type = GetType() ;

	qDebug("Set : Type = %d", type) ;

	if( type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_CRACK || type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_CRACK2 || type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
	{
		//DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_crack.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_crack.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_crack.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_COLOR || type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_COLOR2 )
	{
		//DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_color.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_color.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_color.exec();

	    //if(dialogCode == QDialog::Accepted)
	    {
	    	int base_value = m_dlg_set_tool_color.GetBaseValue() ;

			int tol_max_value = m_dlg_set_tool_color.GetToleranceMax() ;
			int tol_min_value = m_dlg_set_tool_color.GetToleranceMin() ;

			std::string str_info ;
            //str_info += std::to_string(base_value) + "pix" ;
            str_info += std::to_string(base_value) ;
            str_info += " (" + std::to_string(tol_min_value) + "~"  + std::to_string(tol_max_value) + ")" ;
            UpdateInformationString(QString::fromUtf8(str_info.c_str())) ;
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_DISTANCE )
	{
		
		m_dlg_set_tool_inspect_distance.SetParentId(GetParentIdInfo());
		m_dlg_set_tool_inspect_distance.SetToolId(GetIdInfo());
				
		int dialogCode = m_dlg_set_tool_inspect_distance.exec();

		float base_value = m_dlg_set_tool_inspect_distance.GetBaseValue() ;
		float tol_max_value = m_dlg_set_tool_inspect_distance.GetToleranceMax() ;
		float tol_min_value = m_dlg_set_tool_inspect_distance.GetToleranceMin() ;
			
		std::string str_info ;
        str_info += std::to_string(base_value) ;
        str_info += " (" + std::to_string(tol_min_value) + "~"  + std::to_string(tol_max_value) + ")" ;
        UpdateInformationString(QString::fromUtf8(str_info.c_str())) ;
	}
	else if( type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_ANGLE )
	{
		
		m_dlg_set_tool_inspect_angle.SetParentId(GetParentIdInfo());
		m_dlg_set_tool_inspect_angle.SetToolId(GetIdInfo());
				
		int dialogCode = m_dlg_set_tool_inspect_angle.exec();

		float base_value = m_dlg_set_tool_inspect_angle.GetBaseValue() ;
		float tol_max_value = m_dlg_set_tool_inspect_angle.GetToleranceMax() ;
		float tol_min_value = m_dlg_set_tool_inspect_angle.GetToleranceMin() ;
			
		std::string str_info ;
        str_info += std::to_string(base_value) ;
        str_info += " (" + std::to_string(tol_min_value) + "~"  + std::to_string(tol_max_value) + ")" ;
        UpdateInformationString(QString::fromUtf8(str_info.c_str())) ;
	}
	else if( type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_DIAMETER )
	{
		//m_dlg_set_tool_inspect_diameter
		m_dlg_set_tool_inspect_diameter.SetParentId(GetParentIdInfo());
		m_dlg_set_tool_inspect_diameter.SetToolId(GetIdInfo());
				
		int dialogCode = m_dlg_set_tool_inspect_diameter.exec();

		float base_value = m_dlg_set_tool_inspect_diameter.GetBaseValue() ;
		float tol_max_value = m_dlg_set_tool_inspect_diameter.GetToleranceMax() ;
		float tol_min_value = m_dlg_set_tool_inspect_diameter.GetToleranceMin() ;
			
		std::string str_info ;
        str_info += std::to_string(base_value) ;
        str_info += " (" + std::to_string(tol_min_value) + "~"  + std::to_string(tol_max_value) + ")" ;
        UpdateInformationString(QString::fromUtf8(str_info.c_str())) ;
	}
}

void FormToolOption::showEvent(QShowEvent *ev)
{
    QWidget::showEvent( ev );

	std::string str_info = "Information" ;
	
	//Get Information
    if( GetType() == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_CRACK )
    {
        int inspect_crack_level = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Option_Crack_Get_InspectLevel(GetIdInfo());
		str_info = "Crack Level : " + std::to_string(inspect_crack_level) ;		
    }

	UpdateInformationString(QString::fromUtf8(str_info.c_str())) ;

	//run checkbox
	int run_option = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
	ui->checkBox_run->setChecked(run_option);

	//view checkbox
	int view_option = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
	ui->checkBox_view->setChecked(view_option);
}

/*
void FormToolOption::OnBUpdateInfo(QString info)
{
	UpdateInformationString(info) ;
}
*/

void FormToolOption::UpdateInformationString(QString str_info)
{
	//ui->label_info->setText(str_info);
}

void FormToolOption::OnRunCheckBoxToggled(bool checked)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Set_Run_Option(GetIdInfo(), checked) ;
		
	//run checkbox
	int run_option = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
	ui->checkBox_run->setChecked(run_option);

	QString qstr_id = QString::fromStdString(GetParentIdInfo());
	//emit UpdateResultImage(qstr_id) ;
}

void FormToolOption::OnViewCheckBoxToggled(bool checked)
{
	CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Set_View_Option(GetIdInfo(), checked) ;
		
	//view checkbox
	int view_option = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
	ui->checkBox_view->setChecked(view_option);

	QString qstr_id = QString::fromStdString(GetParentIdInfo());
	//emit UpdateResultImage(qstr_id) ;
}

void FormToolOption::SetAlarm(const bool b_on_off)
{
	if( b_on_off )
	{
        ui->label_bg->setStyleSheet(QString("background-color: rgb(255, 0, 0);"));
		
        qDebug("%s : Tool Option Alarm Set!", GetIdInfo().c_str()) ;
#if 0
		QPropertyAnimation *animation = new QPropertyAnimation(ui->label_bg, "color");
		animation->setDuration(2000);
		animation->setStartValue(QColor(0, 0, 0));
		animation->setEndValue(QColor(240, 240, 240));
		animation->start();
#endif		
	}
	else
	{
	}
}


