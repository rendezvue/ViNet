#include "formtooloptionwithprogress.h"
#include "ui_formtooloptionwithprogress.h"

FormToolOptionWithProgress::FormToolOptionWithProgress(QWidget *parent) :
    QWidget(parent),
	 m_type(-1),
    ui(new Ui::FormToolOptionWithProgress)
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

FormToolOptionWithProgress::~FormToolOptionWithProgress()
{
    delete ui;
}

void FormToolOptionWithProgress::SetNameInfo(const std::string name)
{
    //ui->label_option_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormToolOptionWithProgress::GetNameInfo(void)
{
	std::string str_name ;
    //QString name = ui->label_option_name->text() ;

    //std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormToolOptionWithProgress::SetIdInfo(const std::string id)
{
    ui->label_option_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormToolOptionWithProgress::GetIdInfo(void)
{
    QString id = ui->label_option_id->text() ;

    std::string str_id = id.toUtf8().constData();

    return str_id ;
}

void FormToolOptionWithProgress::SetParentIdInfo(const std::string id)
{
	m_str_parent_id = id ;
}

std::string FormToolOptionWithProgress::GetParentIdInfo(void)
{
	return m_str_parent_id ;
}

void FormToolOptionWithProgress::SetTypeInfo(const std::string type)
{
    ui->label_option_type->setText(QString::fromUtf8(type.c_str()));
}

std::string FormToolOptionWithProgress::GetTypeInfo(void)
{
    QString type = ui->label_option_type->text() ;

    std::string str_type = type.toUtf8().constData();

    return str_type ;
}

void FormToolOptionWithProgress::SetType(const int type)
{
	m_type = type ;
}

int FormToolOptionWithProgress::GetType(void)
{
	return m_type ;
}


void FormToolOptionWithProgress::hoverEnter(QHoverEvent * event)
{
	ui->pushButton_option_del->setVisible(true);
	ui->pushButton_option_set->setVisible(true);

	//ui->checkBox_run->setVisible(true);
    //ui->checkBox_view->setVisible(true);
}

void FormToolOptionWithProgress::hoverLeave(QHoverEvent * event)
{
	ui->pushButton_option_del->setVisible(false);
	ui->pushButton_option_set->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
}

void FormToolOptionWithProgress::hoverMove(QHoverEvent * event)
{
}

bool FormToolOptionWithProgress::event(QEvent * e)
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

void FormToolOptionWithProgress::enterEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormToolOptionWithProgress::leaveEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormToolOptionWithProgress::OnButtonDel(void)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
	    p_device->Ensemble_Tool_Del_Option(GetIdInfo()) ;

		emit UpdateList() ;
	}
}

void FormToolOptionWithProgress::OnButtonSet(void)
{
	const int type = GetType() ;

	qDebug("Set : Type = %d", type) ;

    if( type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE || type == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_COLOR_COMPARE2 )
	{
		//DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_color_compare.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_color_compare.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_color_compare.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
			
	}
}

void FormToolOptionWithProgress::showEvent(QShowEvent *ev)
{
    QWidget::showEvent( ev );

	SetProgress(0.0) ;
	
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		std::string str_info = "Information" ;
		
		//Get Information
	    if( GetType() == ToolTypeList::TOOL_TYPE_OPTION_INSPECT_CRACK )
	    {
	        int inspect_crack_level = p_device->Ensemble_Tool_Option_Crack_Get_InspectLevel(GetIdInfo());
			str_info = "Crack Level : " + std::to_string(inspect_crack_level) ;		
	    }

		UpdateInformationString(QString::fromUtf8(str_info.c_str())) ;

		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);

		//view checkbox
		int view_option = p_device->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
		ui->checkBox_view->setChecked(view_option);
	}
}

/*
void FormToolOptionWithProgress::OnBUpdateInfo(QString info)
{
	UpdateInformationString(info) ;
}
*/

void FormToolOptionWithProgress::UpdateInformationString(QString str_info)
{
	//ui->label_info->setText(str_info);
}

void FormToolOptionWithProgress::OnRunCheckBoxToggled(bool checked)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		p_device->Ensemble_Task_Set_Run_Option(GetIdInfo(), checked) ;
			
		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);

		QString qstr_id = QString::fromStdString(GetParentIdInfo());
		//emit UpdateResultImage(qstr_id) ;
	}
}

void FormToolOptionWithProgress::OnViewCheckBoxToggled(bool checked)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		p_device->Ensemble_Task_Set_View_Option(GetIdInfo(), checked) ;
			
		//view checkbox
		int view_option = p_device->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
		ui->checkBox_view->setChecked(view_option);

		QString qstr_id = QString::fromStdString(GetParentIdInfo());
		//emit UpdateResultImage(qstr_id) ;
	}
}

void FormToolOptionWithProgress::SetAlarm(const bool b_on_off)
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
		 ui->label_bg->setStyleSheet(QString("background-color: rgb(255, 255, 255);"));		
	}
}

void FormToolOptionWithProgress::SetProgress(const float value)
{
	ui->progressBar_quality->setValue(value) ;
}



