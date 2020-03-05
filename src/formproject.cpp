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
}

FormProject::~FormProject()
{
    delete ui;
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
        std::string change_name = dlg_change_name.GetName() ;

        qDebug("Project Change Name = %s", change_name.c_str()) ;
		
        if( !change_name.empty() )
        {
            CEnsemble::getInstance()->m_cls_api.Ensemble_Project_Set_Name(GetIdInfo(), change_name) ;
        }

        std::string project_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Project_Get_Name(GetIdInfo()) ;
        ui->label_name->setText(QString::fromUtf8(project_name.c_str()));

        qDebug("Project Name = %s", project_name.c_str()) ;
    }
}

void FormProject::OnButtonDel(void)
{
    CEnsemble::getInstance()->m_cls_api.Ensemble_Project_Del(GetIdInfo()) ;
	
	emit UpdateList();
}

void FormProject::OnButtonRun(void)
{
	std::string str_result_xml = CEnsemble::getInstance()->m_cls_api.Ensemble_Poject_Run(GetIdInfo()) ;

	qDebug("Project Result = %s", str_result_xml.c_str()) ;

	QString qstr_id = QString::fromStdString(GetIdInfo());

	emit UpdateResultImage(qstr_id) ;
	emit UpdateResult(QString::fromStdString(str_result_xml)) ;
}

void FormProject::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

	//run checkbox
	int trigger_run_option = CEnsemble::getInstance()->m_cls_api.Ensemble_Poject_Get_Trigger_Run(GetIdInfo()) ;
	ui->checkBox_trigger_run->setChecked(trigger_run_option);
}

void FormProject::OnTriggerRunCheckBoxToggled(bool checked)
{
	qDebug("Trigger Run Check = %d", checked) ;
	
	CEnsemble::getInstance()->m_cls_api.Ensemble_Poject_Set_Trigger_Run(GetIdInfo(), checked) ;
		
	int trigger_run_option = CEnsemble::getInstance()->m_cls_api.Ensemble_Poject_Get_Trigger_Run(GetIdInfo()) ;
	ui->checkBox_trigger_run->setChecked(trigger_run_option);
}

