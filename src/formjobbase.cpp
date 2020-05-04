#include "formjobbase.h"
#include "ui_formjobbase.h"

FormJobBase::FormJobBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormJobBase)
{
    //ui->pushButton_change->setVisible(false);
    //setMouseTracking(true);
    setAttribute(Qt::WA_Hover);

    ui->setupUi(this);

    ui->pushButton_change->setVisible(false);
	ui->pushButton_run->setVisible(false);
	ui->pushButton_del->setVisible(false);
	ui->pushButton_set_calib->setVisible(false);
	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);

	//button
    connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDel())) ;
    connect(ui->pushButton_change, SIGNAL(clicked()), this,  SLOT(OnButtonSetBase())) ;
	connect(ui->pushButton_run, SIGNAL(clicked()), this,  SLOT(OnButtonRun())) ;

    connect(&m_dlg_setbase_detect_objet, SIGNAL(UpdateBaseImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_setbase_detect_objet, SIGNAL(UpdateBaseName(QString)), this, SLOT(OnUpdateName(QString))) ;
	connect(&m_dlg_setbase_detect_plane, SIGNAL(UpdateBaseImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_setbase_detect_plane, SIGNAL(UpdateBaseName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//set calibration button
	connect(ui->pushButton_set_calib, SIGNAL(clicked()), this, SLOT(OnButtonSetCalibration())) ;

	//check box
    connect(ui->checkBox_run, SIGNAL(clicked(bool)), this, SLOT(OnRunCheckBoxToggled(bool)));
    connect(ui->checkBox_view, SIGNAL(clicked(bool)), this, SLOT(OnViewCheckBoxToggled(bool)));

    //QSize size_result_bt =  ui->pushButton_result_more->sizeHint();
    //ui->pushButton_result_more->resize(20, size_result_bt.height());
    connect(ui->pushButton_result_more, SIGNAL(clicked()), this, SLOT(OnButtonResultInfo())) ;

	//
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
	        this, SLOT(ShowContextMenu(const QPoint &)));
}

FormJobBase::~FormJobBase()
{
    delete ui;
}

void FormJobBase::ShowContextMenu(const QPoint &pos) 
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
			
            //QAction action1(str_name.c_str(), this);
            //connect(&action1, SIGNAL(triggered()), this, SLOT(removeDataPoint()));

            //contextMenu.addAction(&action1);
            contextMenu.addAction(str_menu.c_str());
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
			if( type >= JobType::JOB_TYPE_TOOL && type < JobType::JOB_TYPE_TOOL+10000 )
			{
				const std::string str_id = GetIdInfo() ;
				
				qDebug("call API : CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Add_New(%s)", str_id.c_str()) ;
				
				CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Tool_Add_New(str_id, type) ;

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


void FormJobBase::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

	OnUpdateImage() ;

	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);

		//view checkbox
		int view_option = p_device->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
		ui->checkBox_view->setChecked(view_option);
	}
}


void FormJobBase::OnButtonDel(void)
{
	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
    	p_device->Ensemble_Job_Del(GetIdInfo()) ;
	}

	qDebug("emit UpdateList") ;
	
	emit UpdateList();
}

void FormJobBase::OnButtonRun(void)
{
	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		std::string str_result_xml = p_device->Ensemble_Job_Run(GetIdInfo()) ;

		qDebug("Job Run = %s", str_result_xml.c_str()) ;

		QString qstr_id = QString::fromStdString(GetIdInfo());
		emit UpdateResultImage(qstr_id) ;
	}
}


void FormJobBase::SetNameInfo(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormJobBase::GetNameInfo(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormJobBase::SetIdInfo(const std::string id)
{
    ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormJobBase::GetIdInfo(void)
{
    QString id = ui->label_id->text() ;

    std::string str_id = id.toUtf8().constData();

    return str_id ;
}

void FormJobBase::SetImage(cv::Mat image)
{
	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;
	
    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void FormJobBase::SetObjectImage(cv::Mat image)
{
	//cv::imshow("base object image", image) ;
	//cv::waitKey(1) ;
	
	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image_2->width(), ui->label_image_2->height()) ;
	
    ui->label_image_2->setPixmap(QPixmap::fromImage(qt_display_image));
}

void FormJobBase::OnButtonSetBase(void)
{
	int type = GetType() ;

	qDebug("%s : type=%d", __func__, type) ;
	
    if( type == BaseTypeList::BASE_TYPE_DETECT_OBJECT )		//obejct
	{
		qDebug("Detect Object Set") ;
		
	    m_dlg_setbase_detect_objet.SetId(GetIdInfo());

	    int dialogCode = m_dlg_setbase_detect_objet.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == BaseTypeList::BASE_TYPE_DETECT_PLANE )		//plane
	{
		qDebug("Detect Plane Set") ;
		
	    m_dlg_setbase_detect_plane.SetId(GetIdInfo());

	    int dialogCode = m_dlg_setbase_detect_plane.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
}

void FormJobBase::OnUpdateImage(void)
{
    qDebug("%s - %d 1", __func__, __LINE__) ;

	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		//Get Base Job Image
	    //unsigned char* get_job_image_data = NULL ;
	    //int job_image_width = 0 ;
	    //int job_image_height = 0 ;
	    ImageBuf image_buf ;
	    image_buf.image_width = 0 ;
	    image_buf.image_height = 0 ;

		const int image_type = IMAGE_RGB888 ;
	    //int get_image_type = 0 ;
	   p_device->Ensemble_Job_Get_Image(GetIdInfo(), image_type+IMAGE_THUMBNAIL+IMAGE_ALL_AREA, &image_buf)  ;

		CImageBuf2Mat cls_imagebuf2mat ;
		cv::Mat bae_image= cls_imagebuf2mat.Cvt(image_buf) ;

	    qDebug("%s - %d 2", __func__, __LINE__) ;

	    if( image_buf.p_buf != NULL )
	    {
	        delete [] image_buf.p_buf ;
	        image_buf.p_buf = NULL ;
	    }

	    qDebug("%s - %d 3", __func__, __LINE__) ;

		SetImage(bae_image) ;

		//Get Base Object Image
	    //unsigned char* get_object_image_data = NULL ;
	    //int object_image_width = 0 ;
	    //int object_image_height = 0 ;
	    //get_image_type = 0 ;
	    image_buf.image_width = 0 ;
	    image_buf.image_height = 0 ;
	    image_buf.image_type = 0 ;

	    int object_image_size = p_device->Ensemble_Job_Get_ObjectImage(GetIdInfo(), image_type+IMAGE_THUMBNAIL, &image_buf)  ;

		qDebug("%s - %d 4", __func__, __LINE__) ;

	    if( image_buf.p_buf != NULL )
	    {
			cv::Mat object_image= cls_imagebuf2mat.Cvt(image_buf) ; ;
	       
	        delete [] image_buf.p_buf ;
	        image_buf.p_buf = NULL ;
			
	        SetObjectImage(object_image) ;
	    }

	    qDebug("%s - %d 3", __func__, __LINE__) ;
	}
	
}

void FormJobBase::OnUpdateName(QString name)
{
	std::string str_name = name.toUtf8().constData();
	
	SetNameInfo(str_name) ;
}
	
bool FormJobBase::event(QEvent * e)
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

void FormJobBase::enterEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormJobBase::leaveEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}


void FormJobBase::hoverEnter(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(true);
	ui->pushButton_run->setVisible(true);
	ui->pushButton_del->setVisible(true);
	ui->pushButton_set_calib->setVisible(true);
	
	//ui->checkBox_run->setVisible(true);
	//ui->checkBox_view->setVisible(true);

    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormJobBase::hoverLeave(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(false);
	ui->pushButton_run->setVisible(false);
	ui->pushButton_del->setVisible(false);
	ui->pushButton_set_calib->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
	
    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormJobBase::hoverMove(QHoverEvent * event)
{
    //qDebug() << Q_FUNC_INFO << event->type() ;
}

void FormJobBase::OnRunCheckBoxToggled(bool checked)
{
	qDebug("Base Run Check = %d", checked) ;

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

void FormJobBase::OnViewCheckBoxToggled(bool checked)
{
    qDebug("Base View Check = %d", checked) ;

	std::string str_ip ;
	int port ;
	GetNetworkInfo(&str_ip, &port) ;
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_ip, port) ;

	if( p_device )
	{
		p_device->Ensemble_Task_Set_View_Option(GetIdInfo(), checked) ;
			
		//view checkbox
		int view_option = p_device->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
		ui->checkBox_view->setChecked(view_option);

		QString qstr_id = QString::fromStdString(GetIdInfo());
		//emit UpdateResultImage(qstr_id) ;
	}
}


void FormJobBase::OnButtonSetCalibration(void)
{
	m_dlg_set_calibration.SetId(GetIdInfo());

    int dialogCode = m_dlg_set_calibration.exec();

    if(dialogCode == QDialog::Accepted)
    {
    }
}

void FormJobBase::OnButtonResultInfo(void)
{
	DialogResultInfo dlg_result_info ;

	dlg_result_info.SetIdInfo(GetIdInfo()) ;
	dlg_result_info.SetNameInfo(GetNameInfo()) ;
    dlg_result_info.SetResultString(m_str_result) ;

    int dialogCode = dlg_result_info.exec();

    if(dialogCode == QDialog::Accepted)
    {
    }
}

/*
void FormJobBase::UpdateInfo(void)
{
	QString qstr_info = QString::fromStdString(m_str_info);

	qDebug("Update Info = %s", m_str_info.c_str()) ;
	
	ui->label_info->setText(qstr_info);
}
*/

void FormJobBase::SetInfo(const std::string info)
{
	m_str_info = info ;
}

void FormJobBase::SetResultString(const std::string str_result)
{
	m_str_result = str_result ;
}

void FormJobBase::SetAlarm(const bool b_on_off)
{
}

void FormJobBase::SetType(const int type)
{
	m_type = type ;
}

int FormJobBase::GetType(void)
{
	return m_type ;
}

