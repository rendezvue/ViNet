#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_p_cls_getimage(NULL) ,
    m_p_cls_check_network(NULL)
{
    ui->setupUi(this);

    //image tab
    //ui->tabWidget_image->addTab(new FormImage(), tr("Image"));
    //ui->tabWidget_image->addTab(new FormImage(), tr("Result"));
    //ui->tabWidget_image->addTab(new FormImage(), tr("Image+Result"));
    connect(ui->tabWidget_image, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

    m_source_list_model = new QStringListModel(this); ;

    //button
    connect(ui->pushButton_update_tool, SIGNAL(clicked()), this,  SLOT(UpdateToolsList())) ;
    connect(ui->pushButton_update_job_tree, SIGNAL(clicked()), this,  SLOT(UpdateJobTree())) ;
	connect(ui->pushButton_update_job, SIGNAL(clicked()), this,  SLOT(UpdateJobsList())) ;

	connect(ui->pushButton_update_source_list, SIGNAL(clicked()), this,  SLOT(OnButtonUpdateSourceList())) ;
		
    //Menu
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(OnMenuConnect()));

    //Source list
    connect(ui->listView_source, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OnSourceListDClick(const QModelIndex &)));
	
    //background color
    ui->image_bg->setStyleSheet("QLabel { background-color : black; }");
    ui->merge_bg->setStyleSheet("QLabel { background-color : black; }");
    ui->result_bg->setStyleSheet("QLabel { background-color : black; }");

    //connect(ui->treeWidget_job, SIGNAL(itemEntered(QTreeWidgetItem*,int))

    //items
    /*
    ui->listWidget_items->addItem("Detect: Object");
    ui->listWidget_items->addItem("Detect: Line");
    ui->listWidget_items->addItem("Detect: Circle");
    ui->listWidget_items->addItem("Calc: Distance");
    ui->listWidget_items->addItem("Calc: Angle");
    ui->listWidget_items->addItem("Calc: Color");
    ui->listWidget_items->addItem("Inspection: Distance");
    ui->listWidget_items->addItem("Inspection: Angle");
    ui->listWidget_items->addItem("Inspection: Color");
	ui->listWidget_items->addItem("Inspection: Crack");
    */

    //tree
    // Set the number of columns in the tree
	connect(ui->treeWidget_job, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnTreeViewDClick(QModelIndex)));
	connect(ui->treeWidget_job, SIGNAL(clicked(QModelIndex)), this, SLOT(OnTreeViewClick(QModelIndex)));
	
    ui->treeWidget_job->show();
    ui->treeWidget_job->setColumnCount(1);

    //Change Header Text
    if(QTreeWidgetItem* header = ui->treeWidget_job->headerItem()) {
      header->setText(0, "JOB");
    } else {
      ui->treeWidget_job->setHeaderLabel("JOB");
    }

    //drop
    //connect(ui->treeWidget_job, SIGNAL(), this, SLOT(OnTreeJobDropItem(QDropEvent*)));


    // Add root nodes
    //AddTreeRoot("A", "Root_first");
    //AddTreeRoot("B", "Root_second");
    //AddTreeRoot("C", "Root_third");

    //Timer
    //m_p_timer = new QTimer(this);
    //connect(m_p_timer, SIGNAL(timeout()), this, SLOT(updatePicture()));
    //m_p_timer->start(33);

    //Get Image Thread
#if 1
    m_p_cls_getimage = new CGetImageThread(this) ;
	m_p_cls_getimage->m_thread_run = true ;

    connect(m_p_cls_getimage, SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture(cv::Mat))) ;
    m_p_cls_getimage->start();

    m_p_cls_check_network = new CCheckNetwork(this) ;
    m_p_cls_check_network->start() ;
#endif

    //drop on tree
    connect(ui->treeWidget_job, SIGNAL(SignalDropDone()), this, SLOT(DropEventDoneOnTree())) ;
}

MainWindow::~MainWindow()
{
    if( m_p_cls_getimage != NULL )
    {
    	m_p_cls_getimage->m_thread_run = false ;
		
    	while( m_p_cls_getimage->isFinished() == false )
		{
		} ;
		
    	m_p_cls_getimage->quit();
		
        delete m_p_cls_getimage ;
        m_p_cls_getimage = NULL ;
    }

    if( m_p_cls_check_network != NULL )
    {
        delete m_p_cls_check_network ;
        m_p_cls_check_network = NULL ;
    }

    if( m_source_list_model != NULL )
    {
        delete m_source_list_model ;
        m_source_list_model = NULL ;
    }

    delete ui;
}

void MainWindow::OnTreeViewDClick(const QModelIndex& index) 
{
    QString item_user_data_is_str_id = index.data(Qt::UserRole+1).toString() ;

	std::string str_id = item_user_data_is_str_id.toUtf8().constData() ;

	if( m_p_cls_getimage != NULL )
	{
		m_p_cls_getimage->SetResultId(str_id);
	}	
}

void MainWindow::OnTreeViewClick(const QModelIndex& index) 
{
    QString qstr_device_ip = index.data(Qt::UserRole+100).toString() ;
	QString qstr_device_port = index.data(Qt::UserRole+101).toString() ;

	std::string str_ip = qstr_device_ip.toUtf8().constData() ;
	std::string str_port = qstr_device_port.toUtf8().constData() ;

	if( !str_ip.empty() && !str_port.empty() )
	{
		int port = std::stoi(str_port) ;

		qDebug("OnTreeViewClick : ip = %s, port = %s(%d)", str_ip.c_str(), str_port.c_str(), port) ;

		m_str_select_ip_address = str_ip ;
		m_i_select_port = port ;

		CEnsemble::getInstance()->SelectDevice(m_str_select_ip_address, m_i_select_port) ;
	}
}


void MainWindow::OnSourceListDClick(const QModelIndex &index)
{
    //QModelIndex index = ui->listView->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();

    std::string str_source = itemText.toUtf8().constData() ;
    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Source_Set(str_source) ;

    //tab : Result Image
    ui->tabWidget_image->setCurrentIndex(0);
    //qDebug("source item text = %s". itemText.toUtf8().constData()) ;
}

void MainWindow::updatePicture(cv::Mat image)
{
    //ui->ui_label->setPixmap(QPixmap::fromImage(qt_display_image));
    int tab_index = ui->tabWidget_image->currentIndex() ;

    QLabel *p_image_label_bg = NULL ;
    QLabel *p_image_label = NULL ;

    if( tab_index == 0 )
    {
        p_image_label = ui->image ;
        p_image_label_bg = ui->image_bg ;
    }
    else if( tab_index == 1 )
    {
        p_image_label = ui->result ;
        p_image_label_bg = ui->result_bg ;
    }
    else if( tab_index == 2 )
    {
        p_image_label = ui->merge ;
        p_image_label_bg = ui->merge_bg ;
    }

    if( p_image_label && p_image_label_bg )
    {
        const int draw_width = p_image_label_bg->width();
        const int draw_height = p_image_label_bg->height();

        float rescale_w = (float)draw_width / (float)image.cols ;
        float rescale_h = (float)draw_height / (float)image.rows ;

        float min_rescale = std::fmin(rescale_w, rescale_h) ;
        if( min_rescale < 1.0 )
        {
            cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
        }

        //fit image label by image isze
        int pos_x = (int)((float)p_image_label_bg->x() + (float)(draw_width - image.cols)/2.0) ;
        int pos_y = (int)((float)p_image_label_bg->y() + (float)(draw_height - image.rows)/2.0) ;

        p_image_label->setGeometry(pos_x, pos_y, image.cols, image.rows);

		CMat2QImage cls_mat_2_qimage ;
		QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, p_image_label->width(), p_image_label->height()) ;
        p_image_label->setPixmap(QPixmap::fromImage(qt_display_image));
    }
}

void MainWindow::tabSelected()
{
    int index = ui->tabWidget_image->currentIndex() ;

    qDebug("%s : index=%d", __func__, index) ;

    if( m_p_cls_getimage != NULL )
    {
        qDebug("%s : index=%d set", __func__, index) ;

        m_p_cls_getimage->SetSourceType(index);
    }

}

void MainWindow::OnMenuConnect(void)
{
    qDebug("%s", __func__);

#if 1
    CDialogConnect dlg_connect ;

    int dialogCode = dlg_connect.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked

        //m_p_cls_dlg_connect->ui->lineEdit_ip_address->
        qDebug("Aceept") ;

        m_str_ip_address = dlg_connect.GetIpAddress() ;
        m_p_cls_check_network->SetIPAddress(m_str_ip_address) ;

        qDebug("Connect Info : %s", m_str_ip_address.c_str()) ;

		m_i_port = dlg_connect.GetPortNumber() ;
		m_p_cls_check_network->SetPort(m_i_port) ;

		if( CEnsemble::getInstance()->CheckDevice(m_str_ip_address, m_i_port) == false )		//don't has device
		{
			CEnsemble::getInstance()->New(m_str_ip_address, m_i_port) ;
			CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(m_str_ip_address, m_i_port) ;
			
			if( p_device )
			{
				if( p_device->Ensemble_Network_IsOnline() )
				{			
					p_device->Ensemble_Task_File_Load() ;
					
					m_str_select_ip_address = m_str_ip_address ;
					m_i_select_port = m_i_port ;
					CEnsemble::getInstance()->SelectDevice(m_str_select_ip_address, m_i_select_port) ;
					
					qDebug(" - Success : Control Port") ;

					UpdateToolsListFromDevice(ui->listWidget_items) ;
					UpdateJobsListFromDevice(ui->listWidget_items_job) ;
		       
		            qDebug(" - Success : Image Port") ;

					OnButtonUpdateSourceList() ;
				}
			}
#if 0		
	        int ret = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Network_Connect(m_str_ip_address.c_str(), m_i_port);

	        qDebug("Connect Ensemble : %d", ret ) ;

	        if( CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Network_IsOnline() )
	        {
	            qDebug(" - Success : Control Port") ;

				UpdateToolsListFromDevice(ui->listWidget_items) ;
				UpdateJobsListFromDevice(ui->listWidget_items_job) ;
	       
	            qDebug(" - Success : Image Port") ;

				OnButtonUpdateSourceList() ;
	        }
#endif			
		}

        //ini
        boost::property_tree::ptree pt;
        pt.put("network.ip", m_str_ip_address);
		pt.put("network.port", m_i_port);
        boost::property_tree::ini_parser::write_ini( ".run.ini" , pt );

        UpdateJobTree() ;
    }
    else if(dialogCode == QDialog::Rejected)
    { // NoButton clicked
        qDebug("Reject") ;
    }

    //m_p_cls_dlg_connect->setModal(true);
    //m_p_cls_dlg_connect->show();

#else
    CDialogConnect cls_dlg_connect ;

    cls_dlg_connect.setModal(true);
    cls_dlg_connect.show();
#endif
    //QDialog my_progress_dialog( this );
    //my_progress_dialog.setModal( true );
    //my_progress_dialog.show();
}

void MainWindow::UpdateToolsListFromDevice(QListWidget *listWidget) 
{
    qDebug("test1") ;

	std::string str_able_tools_list_xml = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Type_Get_Tool_List_Xml() ;

	qDebug("tools info xml = %s", str_able_tools_list_xml.c_str()) ;

	//Upate Job List
	//XML Parsing
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string((char *)(str_able_tools_list_xml.c_str()));

    //listWidget->reset();
    //delete listWidget->currentIndex()
    //listWidget->clear();
    //qDeleteAll(listWidget->selectedItems());
    //delete all item
#if 1
    listWidget->clear();
#else
    while(auto item = listWidget->takeItem(0))
    {
       //do something with item
       delete item ;
    }
#endif

	if (!result)
	{
		qDebug("tools list xml parsing error") ;
	}
	else
	{
		for (pugi::xml_node tool: doc.child("Ability").child("Tools").children("Tool"))
		{
            int type = tool.attribute("Type").as_int();
            std::string str_type = tool.attribute("TypeName").value();
			std::string str_name = tool.attribute("Name").value();

            qDebug("TypeName=%s, Name=%s", str_type.c_str(), str_name.c_str()) ;

			std::string str_tool = str_type + ": " + str_name ;

            //QListWidgetItem *listWidgetItem = new QListWidgetItem(listWidget);
            //listWidgetItem->setData(Qt::UserRole, 1);

            //listWidget->addItem(str_tool.c_str());

            //Creating a new list widget item whose parent is the listwidget itself
            QListWidgetItem *listWidgetItem = new QListWidgetItem(listWidget);

            //Adding the item to the listwidget
            listWidget->addItem(listWidgetItem);

            listWidgetItem->setData(Qt::UserRole+1, type);
            //listWidgetItem->setText(QString::fromUtf8(str_tool.c_str()));     //display item

            //Creating an object of the designed widget which is to be added to the listwidget
            FormToolList *theWidgetItem = new FormToolList;

            theWidgetItem->SetName(str_tool);
            theWidgetItem->SetTypeNumber(type);

            //Making sure that the listWidgetItem has the same size as the TheWidgetItem
            QSize item_size = theWidgetItem->size() ;
            qDebug("UpdateToolsListFromDevice theWidgetItem size = %d, %d", item_size.width(), item_size.height()) ;

            //QSize list_size = listWidget->sizeHint() ;
            //listWidgetItem->setSizeHint();
            listWidgetItem->setSizeHint(item_size);

            //listWidgetItem->setSizeHint(item_size);
            //listWidget->setFixedHeight(item_size.height());
            //listWidgetItem->setSizeHint(theWidgetItem->sizeHint());

            //Finally adding the itemWidget to the list
            listWidget->setItemWidget(listWidgetItem, theWidgetItem);
		}
	}

}

void MainWindow::UpdateJobsListFromDevice(QListWidget *listWidget) 
{
	std::string str_able_tools_list_xml = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Type_Get_Job_List_Xml() ;

	qDebug("tools info xml = %s", str_able_tools_list_xml.c_str()) ;

	//Upate Job List
	//XML Parsing
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string((char *)(str_able_tools_list_xml.c_str()));

    //listWidget->reset();
    //delete listWidget->currentIndex()
    //listWidget->clear();
    //qDeleteAll(listWidget->selectedItems());
    //delete all item
#if 1
    listWidget->clear();
#else
    while(auto item = listWidget->takeItem(0))
    {
       //do something with item
       delete item ;
    }
#endif

	if (!result)
	{
		qDebug("tools list xml parsing error") ;
	}
	else
	{
		for (pugi::xml_node tool: doc.child("Ability").child("Bases").children("Base"))
		{
            int type = tool.attribute("Type").as_int();
            std::string str_type = tool.attribute("TypeName").value();
			std::string str_name = tool.attribute("Name").value();

            qDebug("TypeName=%s, Name=%s", str_type.c_str(), str_name.c_str()) ;

			std::string str_tool = str_type + ": " + str_name ;

            //QListWidgetItem *listWidgetItem = new QListWidgetItem(listWidget);
            //listWidgetItem->setData(Qt::UserRole, 1);

            //listWidget->addItem(str_tool.c_str());

            //Creating a new list widget item whose parent is the listwidget itself
            QListWidgetItem *listWidgetItem = new QListWidgetItem(listWidget);

            //Adding the item to the listwidget
            listWidget->addItem(listWidgetItem);

            listWidgetItem->setData(Qt::UserRole+1, type);
            //listWidgetItem->setText(QString::fromUtf8(str_tool.c_str()));     //display item

            //Creating an object of the designed widget which is to be added to the listwidget
            FormToolList *theWidgetItem = new FormToolList;

            theWidgetItem->SetName(str_tool);
            theWidgetItem->SetTypeNumber(type);

            //Making sure that the listWidgetItem has the same size as the TheWidgetItem
            QSize item_size = theWidgetItem->size() ;
            qDebug("UpdateJobsListFromDevice theWidgetItem size = %d, %d", item_size.width(), item_size.height()) ;

            //QSize list_size = listWidget->sizeHint() ;
            //listWidgetItem->setSizeHint();
            listWidgetItem->setSizeHint(item_size);

            //listWidgetItem->setSizeHint(item_size);
            //listWidget->setFixedHeight(item_size.height());
            //listWidgetItem->setSizeHint(theWidgetItem->sizeHint());

            //Finally adding the itemWidget to the list
            listWidget->setItemWidget(listWidgetItem, theWidgetItem);
		}
	}

}

void MainWindow::AddTreeRoot(QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget_job);

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);
    //AddTreeChild(treeItem, name + "A", "Child_first");
    //AddTreeChild(treeItem, name + "B", "Child_second");
}

void MainWindow::AddTreeChild(QTreeWidgetItem *parent,
                  QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}
				  
void MainWindow::UpdateToolsList(void)
{
    UpdateToolsListFromDevice(ui->listWidget_items) ;
}

void MainWindow::UpdateJobsList(void)
{
    UpdateJobsListFromDevice(ui->listWidget_items_job) ;
}


void MainWindow::UpdateResultImage(QString id)
{
	qDebug("UpdateResultImage : ID(%s)", qUtf8Printable(id)) ;
	
    if( m_p_cls_getimage != NULL )
    {
        std::string str_id = id.toUtf8().constData() ;
        m_p_cls_getimage->SetResultId(str_id);
    }

    //tab : Result Image
    ui->tabWidget_image->setCurrentIndex(1);
}

void MainWindow::UpdateResult(QString qstr_xml)
{
	qDebug("UpdateResult : xml = %s", qUtf8Printable(qstr_xml)) ;

	/*
	//Example
	<?xml version="1.0" encoding="UTF-8" standalone="no" ?>	
	<Result ID='20191018125524_Myic8212'>
		<Jobs ID='20191018125526_eDEe2175' TYPE='30000' FindCount='1'>    
			<Job>        
				<Pose CenterX='494.000000' CenterY='143.000000' Angle='0' Roi_TL_X='413.000000' Roi_TL_Y='40.000000' Roi_TR_X='575.000000' Roi_TR_Y='40.000000' Roi_BR_X='575.000000' Roi_BR_Y='247.000000' Roi_BL_X='413.000000' Roi_BL_Y='247.000000' />        
				<Matching Score='1.000000'/>        
				<Specific>        
				</Specific>        
				<Tools>        
				</Tools>    
			</Job>
		</Jobs>
		<Jobs ID='20191018125535_wOUZ1363' TYPE='30000' FindCount='1'>    
			<Job>        
				<Pose CenterX='208.000000' CenterY='316.000000' Angle='0' Roi_TL_X='125.000000' Roi_TL_Y='225.000000' Roi_TR_X='292.000000' Roi_TR_Y='225.000000' Roi_BR_X='292.000000' Roi_BR_Y='408.000000' Roi_BL_X='125.000000' Roi_BL_Y='408.000000' />        
				<Matching Score='1.000000'/>        
				<Specific>        
				</Specific>        
				<Tools>        
				</Tools>    
			</Job>
		</Jobs>	
	</Result>
	*/

	std::string str_xml = qstr_xml.toStdString();

	//XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result_xml_parsing = doc.load_string((char *)(str_xml.c_str()));
	
	if (!result_xml_parsing)
	{
		qDebug("xml parsing error") ;
	}
	else
	{
        std::vector<std::string> vec_result_change_color_id ;
		for (pugi::xml_node result : doc.children("Result") )
        {
        	std::string str_project_id = result.attribute("ID").value() ;

			qDebug("xml parsing : project id =%s", str_project_id.c_str()) ;

			for (pugi::xml_node jobs: result.children("Jobs"))
            {
            	std::string str_jobs_id = jobs.attribute("ID").value() ;
				int jobs_type = jobs.attribute("TYPE").as_int() ;
				int jobs_find_count = jobs.attribute("FindCount").as_int() ;

				if( jobs_find_count <= 0 )
				{
					vec_result_change_color_id.push_back(str_jobs_id) ;
				}

				std::string str_result ;
				qDebug("Job ID=%s, TYPE=%d, FindCount=%d", str_jobs_id.c_str(), jobs_type, jobs_find_count) ;
				str_result = "<p><strong> Job ID=" + str_jobs_id + ", TYPE=" + std::to_string(jobs_type) + ", FindCount=" + std::to_string(jobs_find_count) + "</strong></p>" ;
				
				int job_count = 0 ;
				str_result += "<ul>" ;
				for (pugi::xml_node job: jobs.children("Job"))
            	{
            		float center_x = job.child("Pose").attribute("CenterX").as_float() ;
					float center_y = job.child("Pose").attribute("CenterY").as_float() ;
					float angle = job.child("Pose").attribute("Angle").as_float() ;
					float score = job.child("Matching").attribute("Score").as_float() ;
					
					float roi_tl_x = job.child("Pose").attribute("Roi_TL_X").as_float() ;
					float roi_tl_y = job.child("Pose").attribute("Roi_TL_Y").as_float() ;
					float roi_tr_x = job.child("Pose").attribute("Roi_TR_X").as_float() ;
					float roi_tr_y = job.child("Pose").attribute("Roi_TR_Y").as_float() ;
					float roi_br_x = job.child("Pose").attribute("Roi_BR_X").as_float() ;
					float roi_br_y = job.child("Pose").attribute("Roi_BR_Y").as_float() ;
					float roi_bl_x = job.child("Pose").attribute("Roi_BL_X").as_float() ;
					float roi_bl_y = job.child("Pose").attribute("Roi_BL_Y").as_float() ;
					
                    qDebug("[%d] Center(%.2f, %.2f), Angle(%.2f), Score(%.2f)", job_count,center_x,center_y, angle, score) ;
					qDebug("    - ROI : (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)", roi_tl_x,roi_tl_y,roi_tr_x,roi_tr_y,roi_br_x,roi_br_y,roi_bl_x,roi_bl_y) ;

					str_result +=   "<li>[" + std::to_string(job_count) + "] Center(" + std::to_string(center_x) + ", " + std::to_string(center_y) + "), Angle(" + std::to_string(angle) + "), Score(" + std::to_string(score) + ")</li>" ;

					//---------------------------------------------------------------------------------------
					//Result Tool 
					str_result += "<ul>" ;
					for (pugi::xml_node tools: job.children("Tools"))
            		{
            			for (pugi::xml_node tool: tools.children("Tool"))
            			{
            				std::string str_tool_id = tool.attribute("ID").value() ;
							int tool_type = tool.attribute("TYPE").as_int() ;

							qDebug("Tool ID=%s, TYPE=%d", str_tool_id.c_str(), tool_type) ;

							float center_x = tool.child("Pose").attribute("CenterX").as_float() ;
							float center_y = tool.child("Pose").attribute("CenterY").as_float() ;
							float angle = tool.child("Pose").attribute("Angle").as_float() ;
							float score = tool.child("Matching").attribute("Score").as_float() ;
							
							float roi_tl_x = tool.child("Pose").attribute("Roi_TL_X").as_float() ;
							float roi_tl_y = tool.child("Pose").attribute("Roi_TL_Y").as_float() ;
							float roi_tr_x = tool.child("Pose").attribute("Roi_TR_X").as_float() ;
							float roi_tr_y = tool.child("Pose").attribute("Roi_TR_Y").as_float() ;
							float roi_br_x = tool.child("Pose").attribute("Roi_BR_X").as_float() ;
							float roi_br_y = tool.child("Pose").attribute("Roi_BR_Y").as_float() ;
							float roi_bl_x = tool.child("Pose").attribute("Roi_BL_X").as_float() ;
							float roi_bl_y = tool.child("Pose").attribute("Roi_BL_Y").as_float() ;
							
		                    qDebug("    - Center(%.2f, %.2f), Angle(%.2f), Score(%.2f)", center_x,center_y, angle, score) ;
							qDebug("    - ROI : (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)", roi_tl_x,roi_tl_y,roi_tr_x,roi_tr_y,roi_br_x,roi_br_y,roi_bl_x,roi_bl_y) ;

							//CSearchTreeItem cls_search_tree_item ;
							//QTreeWidgetItem *item = cls_search_tree_item.GetItem(ui->treeWidget_job, str_tool_id) ;
							str_result +=   "<li>[Tool ID=" + str_tool_id + " Type=" + std::to_string(tool_type) + "] Center(" + std::to_string(center_x) + ", " + std::to_string(center_y) + "), Angle(" + std::to_string(angle) + "), Score(" + std::to_string(score) + ")</li>" ;

							//---------------------------------------------------------------------------------------
							//Result Option
							str_result += "<ul>" ;
							for (pugi::xml_node options: tool.children("Options"))
		            		{
		            			for (pugi::xml_node option: options.children("Option"))
		            			{
                                    std::string str_option_id = option.attribute("ID").value() ;
                                    int option_type = option.attribute("TYPE").as_int() ;

									//specific
                                    int option_specific_pass = option.child("Specific").child("Pass").text().as_int() ;
											
                                    qDebug("Option ID=%s, TYPE=%d", str_option_id.c_str(), option_type) ;

                                    std::string str_result_style = "style='color:black;background-color:white;'" ;
									if( option_specific_pass == 0 )
									{
										//fail
										str_result_style = "style='color:red;background-color:white;'" ;

										vec_result_change_color_id.push_back(str_option_id) ;
									}
                                    str_result +=   "<li " + str_result_style + ">[Option ID=" + str_option_id + " Type=" + std::to_string(option_type) + "] Inspect Pass(" + std::to_string(option_specific_pass) + ")</li>" ;
		            			}
							}
							str_result += "</ul>" ;
							//Result Option
							//---------------------------------------------------------------------------------------						
            			}
					}
					str_result += "</ul>" ;
					//Result Tool 
					//---------------------------------------------------------------------------------------
					
					job_count++ ;
				}
				str_result += "</ul>" ;

				//Find Job in List				
				qDebug("Update Job Form : Find %s item", str_jobs_id.c_str()) ;
				CSearchTreeItem cls_search_tree_item ;
				QTreeWidgetItem *item = cls_search_tree_item.GetItem(ui->treeWidget_job, str_jobs_id) ;
				if( item )
				{
					qDebug("Search Item") ;

                    FormJobBase* p_FromJobBase = dynamic_cast<FormJobBase*>(ui->treeWidget_job->itemWidget(item, 0));
                
                    if( p_FromJobBase )
                    {
                        qDebug("Item is Job") ;

                        std::string str_info = "Find Count : " + std::to_string(jobs_find_count) ;

                        p_FromJobBase->SetInfo(str_info) ;
						p_FromJobBase->SetResultString(str_result) ;
						
                    }
				}				
			}
		}

		int size_alarm = vec_result_change_color_id.size() ;
		for( int i=0 ; i<size_alarm ; i++ )
		{
			CSearchTreeItem cls_search_tree_item ;
			QTreeWidgetItem *item = cls_search_tree_item.GetItem(ui->treeWidget_job, vec_result_change_color_id[i]) ;
			if( item )
			{
                FormJobBase* p_FromJobBase = dynamic_cast<FormJobBase*>(ui->treeWidget_job->itemWidget(item, 0));
				FormJobTool* p_FromJobTool = dynamic_cast<FormJobTool*>(ui->treeWidget_job->itemWidget(item, 0));
				FormToolOption* p_FromJobToolOption = dynamic_cast<FormToolOption*>(ui->treeWidget_job->itemWidget(item, 0));
            
                if( p_FromJobBase )
                {		
                	p_FromJobBase->SetAlarm(true) ;
                }
				else if( p_FromJobTool )
				{
					p_FromJobTool->SetAlarm(true) ;
				}
				else if( p_FromJobToolOption )
				{
					p_FromJobToolOption->SetAlarm(true) ;
				}
			}				
		}

		//emit UpdateFormInfo() ;
	}
}

void MainWindow::UpdateJobTree(void)
{
    //delete all item
    ui->treeWidget_job->clear();

    //std::string str_prj_list_xml = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Project_Get_List() ;
	std::string str_prj_list_xml = CEnsemble::getInstance()->GetDeviceJobInfo() ;
	

    qDebug("project list xml (%d, %d) = %s", str_prj_list_xml.size(), str_prj_list_xml.length(), str_prj_list_xml.c_str()) ;

    //XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_prj_list_xml.c_str()));

    if (!result)
    {
        qDebug("xml parsing error") ;
    }
    else
    {

        for (pugi::xml_node device : doc.child("Ensembles").children("Ensemble") )
        {
            std::string str_device_model_name = device.attribute("Model").value() ;
            std::string str_device_mac = device.attribute("Mac").value() ;
			std::string str_device_ip_addr = device.attribute("IP").value() ;
			std::string str_device_port = device.attribute("Port").value() ;
			int device_port = std::stoi(str_device_port) ;

			CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(str_device_ip_addr, device_port) ;
			
            qDebug("xml: Device MAC address=%s, IP=%s, Port=%s(%d)", str_device_mac.c_str(), str_device_ip_addr.c_str(), str_device_port.c_str(), device_port) ;
            //AddTreeRoot(QString::fromStdString(str_device), QString::fromStdString("test")) ;

            std::string str_device_info = str_device_model_name + "(" + str_device_mac + ")" ;

            //Add root
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget_job);
            treeItem->setExpanded(true);
            //treeItem->setText(0, QString::fromStdString(str_device_info));

			treeItem->setData(0, Qt::UserRole+100, QString::fromStdString(str_device_ip_addr));
			treeItem->setData(0, Qt::UserRole+101, QString::fromStdString(str_device_port));

            //Adding the item to the listwidget
            ui->treeWidget_job->addTopLevelItem(treeItem);

            //Creating an object of the designed widget which is to be added to the listwidget
            FormDeviceInfo *theWidgetItem = new FormDeviceInfo;

			connect(theWidgetItem, SIGNAL(signal_Change_Task()), this, SLOT(UpdateJobTree())) ;
			
            //---
            //Set Informationupda
            //if( CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Network_IsOnline() & ENSEMBLE_CONNECT_CONTROL_PORT )
            {
                //unsigned char* get_data = NULL ;
                //int width = 0 ;
                //int height = 0 ;
                ImageBuf image_buf ;
                image_buf.image_width = 0 ;
                image_buf.image_height = 0 ;

				const int image_type = IMAGE_RGB888 ;
                //int get_image_type = 0 ;
                //CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Source_Get_Image(GET_IMAGE_DEVICE_ICON, std::string(), image_type, &image_buf) ;
				if( p_device ) 	p_device->Ensemble_Source_Get_Image(GET_IMAGE_DEVICE_ICON, std::string(), image_type, &image_buf) ;

                //qDebug("Get Image Size = %d x %d", width, height) ;

                if( image_buf.p_buf != NULL )
                {
                    if( image_buf.image_width>0 && image_buf.image_height >0 )
                    {
                    	CImageBuf2Mat cls_imagebuf2mat ;
						cv::Mat icon= cls_imagebuf2mat.Cvt(image_buf) ; ;
						
   						CMat2QImage cls_mat_2_qimage ;
						QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(icon, icon.cols, icon.rows) ;

						theWidgetItem->SetIconInfo(qt_display_image);
                    }

                    delete [] image_buf.p_buf ;
                    image_buf.p_buf = NULL ;
                }
            }

            theWidgetItem->SetDeviceNameInfo(str_device_model_name);
            theWidgetItem->SetMacAddressInfo(str_device_mac, str_device_ip_addr, str_device_port);
            //Set Information
            //---

            //Making sure that the listWidgetItem has the same size as the TheWidgetItem
            QSize item_size = theWidgetItem->size() ;

            treeItem->setSizeHint(0, item_size);

            //ui->treeWidget_job->setItemWidget(treeItem, theWidgetItem);
            // Inserting widgets
            ui->treeWidget_job->setItemWidget(treeItem, 0, theWidgetItem);

			int project_count = 0 ;
			for (pugi::xml_node project: device.children("Project"))
            {
            	std::string str_prj_id = project.attribute("ID").value();
                std::string str_prj_name = project.attribute("Name").value();

				qDebug("Project[%d]: id=%s, name=%s", project_count, str_prj_id.c_str(), str_prj_name.c_str()) ;

				QTreeWidgetItem *treeChileItem = new QTreeWidgetItem(JobType::JOB_TYPE_PROJECT);
                treeItem->addChild(treeChileItem);

                treeChileItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_prj_id));
				treeChileItem->setData(0, Qt::UserRole+100, QString::fromStdString(str_device_ip_addr));
				treeChileItem->setData(0, Qt::UserRole+101, QString::fromStdString(str_device_port));
			
                FormProject *theWidgetItem = new FormProject;

				connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
				connect(theWidgetItem, SIGNAL(UpdateResultImage(QString)), this, SLOT(UpdateResultImage(QString))) ;
				connect(theWidgetItem, SIGNAL(UpdateResult(QString)), this, SLOT(UpdateResult(QString))) ;
				
                theWidgetItem->SetNameInfo(str_prj_name);
                theWidgetItem->SetIdInfo(str_prj_id);
				theWidgetItem->SetNetworkInfo(str_device_ip_addr, device_port) ;

                QSize item_size = theWidgetItem->size() ;
                treeChileItem->setSizeHint(0, item_size);

                ui->treeWidget_job->setItemWidget(treeChileItem, 0, theWidgetItem);
				
				project_count++ ;

				//Jobs
				
				for (pugi::xml_node job: project.child("Jobs").children("Job"))
				{
					std::string str_job_id = job.attribute("ID").value();
                    int type = job.attribute("Type").as_int();
                    std::string str_name = job.attribute("Name").value();
					
					//std::string str_tool_type_name = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Info_Get_ToolTypeName(type) ;
					std::string str_tool_type_name ;
					if( p_device ) 	str_tool_type_name = p_device->Ensemble_Info_Get_ToolTypeName(type) ;
					
                    qDebug("Job : Type=%d, TypeName=%s, Name=%s", type, str_tool_type_name.c_str(), str_name.c_str()) ;

                    QTreeWidgetItem *treeJobItem = new QTreeWidgetItem(JobType::JOB_TYPE_BASE);
                    treeChileItem->setExpanded(true);
                    treeChileItem->addChild(treeJobItem);

					treeJobItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_job_id));
					treeJobItem->setData(0, Qt::UserRole+100, QString::fromStdString(str_device_ip_addr));
					treeJobItem->setData(0, Qt::UserRole+101, QString::fromStdString(str_device_port));
				
                    FormJobBase *theWidgetItem = new FormJobBase;
					theWidgetItem->SetNameInfo(str_name);
                	theWidgetItem->SetIdInfo(str_job_id);
					theWidgetItem->SetType(type) ;
					theWidgetItem->SetNetworkInfo(str_device_ip_addr, device_port) ;
				
					connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
					connect(theWidgetItem, SIGNAL(UpdateResultImage(QString)), this, SLOT(UpdateResultImage(QString))) ;
					//connect(this, SIGNAL(UpdateFormInfo()), theWidgetItem, SLOT(UpdateInfo())) ;		//mainwindow(UpdateInfoJob) --> FormJobBase(UpdateInfo)
					
                    QSize item_size = theWidgetItem->size() ;
                    treeJobItem->setSizeHint(0, item_size);

                    ui->treeWidget_job->setItemWidget(treeJobItem, 0, theWidgetItem);

					//---------------------------
	                //Tool list
	                for (pugi::xml_node tool: job.child("Tools").children("Tool"))
	                {
	                    std::string str_id = tool.attribute("ID").value();
	                    int type = tool.attribute("Type").as_int();
	                    std::string str_name = tool.attribute("Name").value();
						
						std::string str_tool_type_name ;
						if( p_device ) str_tool_type_name = p_device->Ensemble_Info_Get_ToolTypeName(type) ;
						
	                    qDebug("Tool Type=%d, TypeName=%s, Name=%s", type, str_tool_type_name.c_str(), str_name.c_str()) ;

	                    QTreeWidgetItem *treeToolItem = new QTreeWidgetItem(JobType::JOB_TYPE_TOOL);
	                    treeJobItem->setExpanded(true);
	                    treeJobItem->addChild(treeToolItem);

						treeToolItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_id));
						treeToolItem->setData(0, Qt::UserRole+100, QString::fromStdString(str_device_ip_addr));
						treeToolItem->setData(0, Qt::UserRole+101, QString::fromStdString(str_device_port));
					
	                    FormJobTool *theWidgetItem = new FormJobTool;
	                    theWidgetItem->SetNameInfo(str_name);
	                    theWidgetItem->SetIdInfo(str_id);
						theWidgetItem->SetParentIdInfo(str_job_id);
	                    theWidgetItem->SetTypeInfo(str_tool_type_name);
						theWidgetItem->SetType(type) ;
						theWidgetItem->SetNetworkInfo(str_device_ip_addr, device_port) ;

						connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
                        //connect(this, SIGNAL(UpdateFormInfo()), theWidgetItem, SLOT(UpdateInfo())) ;		//mainwindow(UpdateInfoJob) --> FormJobBase(UpdateInfo)
						//connect(theWidgetItem, SIGNAL(UpdateResultImage(QString)), this, SLOT(UpdateResultImage(QString))) ;
						
	                    QSize item_size = theWidgetItem->size() ;
	                    treeToolItem->setSizeHint(0, item_size);

	                    ui->treeWidget_job->setItemWidget(treeToolItem, 0, theWidgetItem);

						//---------------------------
		                //Option list
		                for (pugi::xml_node option: tool.child("Options").children("Option"))
		                {
		                	std::string str_option_id = option.attribute("ID").value();
		                    int option_type = option.attribute("Type").as_int();
		                    std::string str_option_name = option.attribute("Name").value();
							
							std::string str_tool_option_type_name ;
							if( p_device ) str_tool_option_type_name = p_device->Ensemble_Info_Get_ToolTypeName(option_type) ;
							
		                    qDebug("Tool Option Type=%d, TypeName=%s, Name=%s", option_type, str_tool_option_type_name.c_str(), str_option_name.c_str()) ;

		                    QTreeWidgetItem *treeToolOptionItem = new QTreeWidgetItem(JobType::JOB_TYPE_TOOL_OPTION);
		                    treeToolItem->setExpanded(true);
		                    treeToolItem->addChild(treeToolOptionItem);

							treeToolOptionItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_option_id));
							treeToolOptionItem->setData(0, Qt::UserRole+100, QString::fromStdString(str_device_ip_addr));
							treeToolOptionItem->setData(0, Qt::UserRole+101, QString::fromStdString(str_device_port));
						
		                    FormToolOption *theWidgetItem = new FormToolOption;
		                    theWidgetItem->SetNameInfo(str_option_name);
		                    theWidgetItem->SetIdInfo(str_option_id);
							theWidgetItem->SetParentIdInfo(str_id);			//Parent Type is Tool
		                    theWidgetItem->SetTypeInfo(str_tool_option_type_name);
							theWidgetItem->SetType(option_type) ;
							theWidgetItem->SetNetworkInfo(str_device_ip_addr, device_port) ;

							connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
							//connect(this, SIGNAL(UpdateFormInfo()), theWidgetItem, SLOT(UpdateInfo())) ;		//mainwindow(UpdateInfoJob) --> FormJobBase(UpdateInfo)
							//connect(theWidgetItem, SIGNAL(UpdateResultImage(QString)), this, SLOT(UpdateResultImage(QString))) ;
							
		                    QSize item_size = theWidgetItem->size() ;
		                    treeToolOptionItem->setSizeHint(0, item_size);

		                    ui->treeWidget_job->setItemWidget(treeToolOptionItem, 0, theWidgetItem);
		                }
	                }
	                //Tool list
	                //---------------------------
				}
			}
        }
    }
}

void MainWindow::DropEventDoneOnTree(void)
{
    UpdateJobTree() ;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev) ;

    //tab : Result Image
    ui->tabWidget_image->setCurrentIndex(0);
}

void MainWindow::OnButtonUpdateSourceList(void)
{
	std::string str_source_list_xml = CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Source_Get_List() ;

	qDebug(" - Get Source List : %s", str_source_list_xml.c_str()) ;
	
	//xml parsing
	//XML Parsing
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string((char *)(str_source_list_xml.c_str()));

    if (!result)
    {

        qDebug("xml parsing error") ;
    }
    else
    {
        ui->listView_source->reset();

		QStringList stringListSource;
        stringListSource << "[CAMERA]" ;
        for (pugi::xml_node job: doc.child("Ensemble").children("Image"))
        {
            std::string str_path = job.child("PATH").text().get();
            std::string str_path2 = str_path ;

            stringListSource << str_path2.c_str() ;
        }

        // Create model
        m_source_list_model->setStringList(stringListSource);

        // Glue model and view together
        ui->listView_source->setModel(m_source_list_model);
    }
}

