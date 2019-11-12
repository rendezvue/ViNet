#include "formjobtool.h"
#include "ui_formjobtool.h"

FormJobTool::FormJobTool(QWidget *parent) :
    QWidget(parent),
    m_type(-1),
    m_i_option_count(0) ,
    ui(new Ui::FormJobTool)
{
	setAttribute(Qt::WA_Hover);

    ui->setupUi(this);

    ui->pushButton_change->setVisible(false);	
	ui->pushButton_del->setVisible(false);
	ui->pushButton_option->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
	
	//button
    connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDel())) ;
    connect(ui->pushButton_change, SIGNAL(clicked()), this,  SLOT(OnButtonSetBase())) ;
	connect(ui->pushButton_option, SIGNAL(clicked()), this,  SLOT(OnButtonSetOption())) ;

    connect(&m_dlg_set_tool_object, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_object, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

    //connect(&m_dlg_set_tool_crack, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//line
	connect(&m_dlg_set_tool_line, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_line, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//circle
	connect(&m_dlg_set_tool_circle, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_circle, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//distance
	connect(&m_dlg_set_tool_distance, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//offset distance
	connect(&m_dlg_set_tool_offset_distance, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//update list
	connect(&m_dlg_select_option, SIGNAL(UpdateList()), this, SLOT(OnUpdateList())) ;

	
	//check box
	connect(ui->checkBox_run, SIGNAL(clicked(bool)), this, SLOT(OnRunCheckBoxToggled(bool)));
	connect(ui->checkBox_view, SIGNAL(clicked(bool)), this, SLOT(OnViewCheckBoxToggled(bool)));
}

FormJobTool::~FormJobTool()
{
    delete ui;
}

void FormJobTool::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

    //Set Initialize
    //Get Option Count
    m_i_option_count = Ensemble_Tool_Get_OptionList_Count(GetType()) ;

	OnUpdateImage() ;

	//run checkbox
	int run_option = Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
	ui->checkBox_run->setChecked(run_option);

	//view checkbox
	int view_option = Ensemble_Task_Get_View_Option(GetIdInfo()) ;
	ui->checkBox_view->setChecked(view_option);
}

void FormJobTool::OnButtonSetOption(void) 
{
	if( m_i_option_count > 0 )
	{
		m_dlg_select_option.SetToolType(GetType()) ;
		m_dlg_select_option.SetToolId(GetIdInfo()) ;

		int dialogCode = m_dlg_select_option.exec();

		//Update
	    if(dialogCode == QDialog::Accepted)
	    {
	    	OnUpdateList();
	    }
	}
}

void FormJobTool::OnButtonSetBase(void)
{
    int type = GetType() ;

	qDebug("%s : type=%d", __func__, type) ;
	
    if( type == ToolTypeList::TOOL_TYPE_DETECT_OBJECT )		//obejct
	{
		qDebug("Object Set") ;
		
	    //DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_object.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_object.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_object.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    if( type == ToolTypeList::TOOL_TYPE_DETECT_LINE )		//line
	{	
		qDebug("Line Set") ;
		
	    m_dlg_set_tool_line.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_line.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_line.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_DETECT_CIRCLE )		//circle
   	{
   		qDebug("Circle Set") ;
		
	    m_dlg_set_tool_circle.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_circle.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_circle.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_CALC_DISTANCE )
	{
		qDebug("Distance Set") ;
		
	    m_dlg_set_tool_distance.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_distance.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_distance.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_CALC_ANGLE )
	{
		qDebug("Angle Set") ;
		
	    m_dlg_set_tool_angle.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_angle.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_angle.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_OFFSET_DISTANCE )
	{
		qDebug("Offset Distance Set") ;
		
	    m_dlg_set_tool_offset_distance.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_offset_distance.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_offset_distance.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	
#if 0
    else if( type == ToolTypeList::TOOL_TYPE_INSPECT_DISTANCE )
	{
		qDebug("Inspect Distance Set") ;

		m_dlg_set_tool_inspect_distance.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_inspect_distance.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_inspect_distance.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_INSPECT_CRACK )		//crack
   	{
   		qDebug("Crack Set") ;
		
   		//DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_crack.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_crack.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_crack.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
#endif
}

void FormJobTool::OnUpdateList(void)
{
	emit UpdateList();
}

void FormJobTool::OnButtonDel(void)
{
    Ensemble_Tool_Del(GetIdInfo()) ;
	
	OnUpdateList();
}


void FormJobTool::SetNameInfo(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormJobTool::GetNameInfo(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormJobTool::SetIdInfo(const std::string id)
{
    ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormJobTool::GetIdInfo(void)
{
    QString id = ui->label_id->text() ;

    std::string str_id = id.toUtf8().constData();

    return str_id ;
}

void FormJobTool::SetParentIdInfo(const std::string id)
{
	m_str_parent_id = id ;
}

std::string FormJobTool::GetParentIdInfo(void)
{
	return m_str_parent_id ;
}

void FormJobTool::SetTypeInfo(const std::string type)
{
    ui->label_type->setText(QString::fromUtf8(type.c_str()));
}

std::string FormJobTool::GetTypeInfo(void)
{
    QString type = ui->label_type->text() ;

    std::string str_type = type.toUtf8().constData();

    return str_type ;
}

void FormJobTool::SetType(const int type)
{
	m_type = type ;
}

int FormJobTool::GetType(void)
{
	return m_type ;
}

bool FormJobTool::event(QEvent * e)
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

void FormJobTool::enterEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormJobTool::leaveEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}


void FormJobTool::hoverEnter(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(true);
	ui->pushButton_del->setVisible(true);
	if( m_i_option_count > 0 )	ui->pushButton_option->setVisible(true);

	//ui->checkBox_run->setVisible(true);
	//ui->checkBox_view->setVisible(true);

    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormJobTool::hoverLeave(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(false);
	ui->pushButton_del->setVisible(false);
	if( m_i_option_count > 0 )	ui->pushButton_option->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
	
    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormJobTool::hoverMove(QHoverEvent * event)
{
    //qDebug() << Q_FUNC_INFO << event->type() ;
}

void FormJobTool::OnUpdateImage(void)
{
	//Get Base Object Image
    unsigned char* get_object_image_data = NULL ;
    int object_image_width = 0 ;
    int object_image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
    Ensemble_Tool_Get_ObjectImage(GetIdInfo(), image_type+IMAGE_ICON, &get_object_image_data, &object_image_width, &object_image_height)  ;

	cv::Mat object_image ;
	if( object_image_width > 0 && object_image_height > 0 )
	{
		if( image_type == IMAGE_YUV420 )
		{
			//YUV420 
	        cv::Mat get_image(object_image_height + object_image_height / 2, object_image_width, CV_8UC1, get_object_image_data) ;

	        CImgDec cls_image_decoder ;
	        object_image = cls_image_decoder.Decoding(get_image) ;

		}
		else if( image_type == IMAGE_RGB888 )
		{
			cv::Mat get_image(object_image_height, object_image_width, CV_8UC3, get_object_image_data) ;
			cv::cvtColor(get_image, object_image, cv::COLOR_BGR2RGB) ;
		}
	}

    if( get_object_image_data != NULL )
    {
        delete [] get_object_image_data ;
        get_object_image_data = NULL ;
    }

	SetObjectImage(object_image) ;
	
}

void FormJobTool::SetObjectImage(cv::Mat image)
{
	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;
	
    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void FormJobTool::OnUpdateName(QString name)
{
	std::string str_name = name.toUtf8().constData();
	
	SetNameInfo(str_name) ;
}

void FormJobTool::OnRunCheckBoxToggled(bool checked)
{
	Ensemble_Task_Set_Run_Option(GetIdInfo(), checked) ;
		
	//run checkbox
	int run_option = Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
	ui->checkBox_run->setChecked(run_option);

	QString qstr_id = QString::fromStdString(GetParentIdInfo());
	emit UpdateResultImage(qstr_id) ;
}

void FormJobTool::OnViewCheckBoxToggled(bool checked)
{
	Ensemble_Task_Set_View_Option(GetIdInfo(), checked) ;
		
	//view checkbox
	int view_option = Ensemble_Task_Get_View_Option(GetIdInfo()) ;
	ui->checkBox_view->setChecked(view_option);

	QString qstr_id = QString::fromStdString(GetParentIdInfo());
	emit UpdateResultImage(qstr_id) ;
}


void FormJobTool::SetAlarm(const bool b_on_off)
{
}

