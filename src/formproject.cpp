#include "formproject.h"
#include "ui_formproject.h"

FormProject::FormProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormProject)
{
    ui->setupUi(this);

	setAttribute(Qt::WA_Hover);

	ui->pushButton_change->setVisible(false);
	ui->pushButton_run->setVisible(false);
	ui->pushButton_del->setVisible(false);

	connect(ui->pushButton_change, SIGNAL(clicked()), this,  SLOT(OnButtonSetName())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDel())) ;

	connect(ui->pushButton_run, SIGNAL(clicked()), this,  SLOT(OnButtonRun())) ;

	//check box
    connect(ui->checkBox_trigger_run, SIGNAL(clicked(bool)), this, SLOT(OnTriggerRunCheckBoxToggled(bool)));

	//context menu
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
	        this, SLOT(ShowContextMenu(const QPoint &)));
}

FormProject::~FormProject()
{
    delete ui;
}

void FormProject::ShowContextMenu(const QPoint &pos)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
		QMenu contextMenu(tr("Context menu"), this);

		//Get Sub Job List
		const std::string str_addable_job_list_xml = p_device->Ensemble_Info_Get_Addable_Subjob_List_Xml(GetIdInfo()) ;

		//parsing
		CParsingAddableJobList cls_parsing_addable_job_list ;
		std::vector<AddableJobInfo> vec_list = cls_parsing_addable_job_list.GetAddableJobList(str_addable_job_list_xml) ;

        const int size_list = vec_list.size() ;
		for( int i=0 ; i<size_list ; i++ )
		{
            std::string str_menu ;
            str_menu = "New " + vec_list[i].name ;

			if( !vec_list[i].description.empty() )	str_menu += "(" + vec_list[i].description + ")" ;
			
            //QAction action1(str_menu.c_str(), this);
            //action1.setData(vec_list[i].type) ;
            QAction *action ;
            action = new  QAction(str_menu.c_str(), this) ;
            action->setData(vec_list[i].type);
            //connect(&action1, SIGNAL(triggered()), this, SLOT(removeDataPoint()));

            contextMenu.addAction(action);
            //contextMenu.addAction(str_menu.c_str());
            //contextMenu.exec(mapToGlobal(pos));
        }

        QAction* selectedItem = contextMenu.exec(mapToGlobal(pos));
        if( selectedItem )
        {
	        QString txt = selectedItem->text();
            std::string str_txt = txt.toUtf8().constData();
            const int type = selectedItem->data().toInt() ;

            qDebug("select item = %s, type = %d", str_txt.c_str(), type) ;

			//new job
			if( type >= JobType::JOB_TYPE_BASE && type < JobType::JOB_TYPE_BASE+10000 )
			{
				const std::string str_id = GetIdInfo() ;
				
				qDebug("call API : CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Add_New(%s)", str_id.c_str()) ;
				
				CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Add_New(str_id, type) ;

				emit UpdateList() ;
			}
			
			/*
			if( item_from_type >= JobType::JOB_TYPE_BASE && item_from_type < JobType::JOB_TYPE_BASE+10000 )
			{
				qDebug("call API : CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Add_New(%s)", str_target_id.c_str()) ;
				
				CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Job_Add_New(str_target_id, item_from_type) ;
			}
			*/
        }
	}
}

void FormProject::SetNameInfo(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormProject::GetNameInfo(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormProject::SetIdInfo(const std::string id)
{
	m_str_id = id ;
	
    ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormProject::GetIdInfo(void)
{
#if 0
	qDebug("Call FormProject::GetIdInfo") ;
	
    QString id = ui->label_id->text() ;

    std::string str_id = id.toUtf8().constData();

	qDebug("Call FormProject::GetIdInfo ID = %s", str_id.c_str()) ;
#endif

    return m_str_id ;
}

bool FormProject::event(QEvent * e)
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

void FormProject::enterEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormProject::leaveEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}


void FormProject::hoverEnter(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(true);
	ui->pushButton_run->setVisible(true);
	ui->pushButton_del->setVisible(true);

    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormProject::hoverLeave(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(false);
	ui->pushButton_run->setVisible(false);
	ui->pushButton_del->setVisible(false);
	
    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormProject::hoverMove(QHoverEvent * event)
{
    //qDebug() << Q_FUNC_INFO << event->type() ;
}

void FormProject::OnButtonSetName(void)
{
    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetIdInfo());
    dlg_change_name.SetName(GetNameInfo());

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
 		CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

		if( p_device )
		{ 
	        std::string change_name = dlg_change_name.GetName() ;

	        qDebug("Project Change Name = %s", change_name.c_str()) ;
			
	        if( !change_name.empty() )
	        {
	           p_device->Ensemble_Project_Set_Name(GetIdInfo(), change_name) ;
	        }

	        std::string project_name = p_device->Ensemble_Project_Get_Name(GetIdInfo()) ;
	        ui->label_name->setText(QString::fromUtf8(project_name.c_str()));

	        qDebug("Project Name = %s", project_name.c_str()) ;
		}
    }
}

void FormProject::OnButtonDel(void)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
	    p_device->Ensemble_Project_Del(GetIdInfo()) ;
	
		emit UpdateList();
	}
}

void FormProject::OnButtonRun(void)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		std::string str_result_xml = p_device->Ensemble_Poject_Run(GetIdInfo()) ;

		qDebug("Project Result = %s", str_result_xml.c_str()) ;

		QString qstr_id = QString::fromStdString(GetIdInfo());

		emit UpdateResultImage(qstr_id) ;
		emit UpdateResult(QString::fromStdString(str_result_xml)) ;
	}
}

void FormProject::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		//run checkbox
		int trigger_run_option = p_device->Ensemble_Poject_Get_Trigger_Run(GetIdInfo()) ;
		ui->checkBox_trigger_run->setChecked(trigger_run_option);
	}
}

void FormProject::OnTriggerRunCheckBoxToggled(bool checked)
{
	qDebug("Trigger Run Check = %d", checked) ;

	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		p_device->Ensemble_Poject_Set_Trigger_Run(GetIdInfo(), checked) ;
			
		int trigger_run_option = p_device->Ensemble_Poject_Get_Trigger_Run(GetIdInfo()) ;
		ui->checkBox_trigger_run->setChecked(trigger_run_option);
	}
}

