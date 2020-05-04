#include "formjobbasecode.h"
#include "ui_formjobbasecode.h"

FormJobBaseCode::FormJobBaseCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormJobBaseCode)
{
    //ui->pushButton_change->setVisible(false);
    //setMouseTracking(true);
    setAttribute(Qt::WA_Hover);

    ui->setupUi(this);

    ui->pushButton_change->setVisible(false);
	ui->pushButton_run->setVisible(false);
	ui->pushButton_del->setVisible(false);

	//button
    connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDel())) ;
    connect(ui->pushButton_change, SIGNAL(clicked()), this,  SLOT(OnButtonEdit())) ;
	connect(ui->pushButton_run, SIGNAL(clicked()), this,  SLOT(OnButtonRun())) ;

	connect(&m_dlg_coding, SIGNAL(UpdateBaseName(QString)), this, SLOT(OnUpdateName(QString))) ;
	//connect(&m_dlg_setbase_detect_objet, SIGNAL(UpdateBaseName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//check box
    connect(ui->checkBox_run, SIGNAL(clicked(bool)), this, SLOT(OnRunCheckBoxToggled(bool)));
}

FormJobBaseCode::~FormJobBaseCode()
{
    delete ui;
}

void FormJobBaseCode::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);
	}
}


void FormJobBaseCode::OnButtonDel(void)
{
	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
    	p_device->Ensemble_Job_Del(GetIdInfo()) ;
	}

	//qDebug("emit UpdateList") ;
	
	emit UpdateList();
}

void FormJobBaseCode::OnButtonRun(void)
{
	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		std::string str_result_xml = p_device->Ensemble_Job_Run(GetIdInfo()) ;

		//qDebug("Job Run = %s", str_result_xml.c_str()) ;

		QString qstr_id = QString::fromStdString(GetIdInfo());
		emit UpdateResultImage(qstr_id) ;
	}
}


void FormJobBaseCode::SetNameInfo(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormJobBaseCode::GetNameInfo(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormJobBaseCode::SetIdInfo(const std::string id)
{
    ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormJobBaseCode::GetIdInfo(void)
{
    QString id = ui->label_id->text() ;

    std::string str_id = id.toUtf8().constData();

    return str_id ;
}

void FormJobBaseCode::OnButtonEdit(void)
{
	int type = GetType() ;

	//qDebug("%s : type=%d", __func__, type) ;
	
    if( type == BaseTypeList::BASE_TYPE_PYTHON )		
	{
	    m_dlg_coding.SetId(GetIdInfo());

	    int dialogCode = m_dlg_coding.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
}

void FormJobBaseCode::OnUpdateName(QString name)
{
	std::string str_name = name.toUtf8().constData();
	
	SetNameInfo(str_name) ;
}
	
bool FormJobBaseCode::event(QEvent * e)
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

void FormJobBaseCode::enterEvent(QEvent * e)
{
    ////qDebug() << Q_FUNC_INFO << e->type();
}

void FormJobBaseCode::leaveEvent(QEvent * e)
{
    ////qDebug() << Q_FUNC_INFO << e->type();
}


void FormJobBaseCode::hoverEnter(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(true);
	ui->pushButton_run->setVisible(true);
	ui->pushButton_del->setVisible(true);
}

void FormJobBaseCode::hoverLeave(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(false);
	ui->pushButton_run->setVisible(false);
	ui->pushButton_del->setVisible(false);
}

void FormJobBaseCode::hoverMove(QHoverEvent * event)
{
    ////qDebug() << Q_FUNC_INFO << event->type() ;
}

void FormJobBaseCode::OnRunCheckBoxToggled(bool checked)
{
	//qDebug("Base Run Check = %d", checked) ;

	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		p_device->Ensemble_Task_Set_Run_Option(GetIdInfo(), checked) ;
			
		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);

		QString qstr_id = QString::fromStdString(GetIdInfo());
	}
	//emit UpdateResultImage(qstr_id) ;
}

void FormJobBaseCode::SetResultString(const std::string str_result)
{
	m_str_result = str_result ;
}

void FormJobBaseCode::SetType(const int type)
{
	m_type = type ;
}

int FormJobBaseCode::GetType(void)
{
	return m_type ;
}


