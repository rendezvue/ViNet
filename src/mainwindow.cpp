#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_p_cls_getimage(NULL)
{
    ui->setupUi(this);

    //image tab
    //ui->tabWidget_image->addTab(new FormImage(), tr("Image"));
    //ui->tabWidget_image->addTab(new FormImage(), tr("Result"));
    //ui->tabWidget_image->addTab(new FormImage(), tr("Image+Result"));
    connect(ui->tabWidget_image, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));

    m_source_list_model = new QStringListModel(this); ;

    //button
    connect(ui->pushButton_newjob, SIGNAL(clicked()), this,  SLOT(OnButtonNewProject())) ;
    connect(ui->pushButton_update_tool, SIGNAL(clicked()), this,  SLOT(UpdateToolsList())) ;
    connect(ui->pushButton_update_job_tree, SIGNAL(clicked()), this,  SLOT(UpdateJobTree())) ;
	connect(ui->pushButton_update_job, SIGNAL(clicked()), this,  SLOT(UpdateJobsList())) ;

	connect(ui->pushButton_task_save, SIGNAL(clicked()), this,  SLOT(OnButtonSaveAllTask())) ;
	connect(ui->pushButton_task_load, SIGNAL(clicked()), this,  SLOT(OnButtonLoadAllTask())) ;

    //Menu
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(OnMenuConnect()));

    //Source list
    connect(ui->listView_source, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OnSourceListDClick(const QModelIndex &)));

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
    connect(m_p_cls_getimage, SIGNAL(Done(cv::Mat)), this, SLOT(updatePicture(cv::Mat))) ;
    m_p_cls_getimage->start();
#endif

    //drop on tree
    connect(ui->treeWidget_job, SIGNAL(SignalDropDone()), this, SLOT(DropEventDoneOnTree())) ;
}

MainWindow::~MainWindow()
{
    if( m_p_cls_getimage != NULL )
    {
        delete m_p_cls_getimage ;
        m_p_cls_getimage = NULL ;
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


void MainWindow::OnSourceListDClick(const QModelIndex &index)
{
    //QModelIndex index = ui->listView->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();

    std::string str_source = itemText.toUtf8().constData() ;
    Ensemble_Source_Set(str_source) ;

    //tab : Result Image
    ui->tabWidget_image->setCurrentIndex(0);
    //qDebug("source item text = %s". itemText.toUtf8().constData()) ;
}

void MainWindow::updatePicture(cv::Mat image)
{
    QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);

    //ui->ui_label->setPixmap(QPixmap::fromImage(qt_display_image));
    int tab_index = ui->tabWidget_image->currentIndex() ;

    if( tab_index == 0 )
    {
        ui->image->setPixmap(QPixmap::fromImage(qt_display_image));
    }
    else if( tab_index == 1 )
    {
        ui->result->setPixmap(QPixmap::fromImage(qt_display_image));
    }
    else if( tab_index == 2 )
    {
        ui->merge->setPixmap(QPixmap::fromImage(qt_display_image));
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

        std::string str_ip_address = dlg_connect.GetIpAddress() ;

        qDebug("Connect Info : %s", str_ip_address.c_str()) ;

        int ret = Ensemble_Network_Connect(str_ip_address.c_str());

        qDebug("Connect Ensemble : %d", ret ) ;


        if( ret & ENSEMBLE_CONNECT_CONTROL_PORT )
        {
            qDebug(" - Success : Control Port") ;

			UpdateToolsListFromDevice(ui->listWidget_items) ;
			UpdateJobsListFromDevice(ui->listWidget_items_job) ;
       
            qDebug(" - Success : Image Port") ;

			std::string str_source_list_xml = Ensemble_Source_Get_List() ;

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

        //ini
        boost::property_tree::ptree pt;
        pt.put("network.ip", str_ip_address);
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
	std::string str_able_tools_list_xml = Ensemble_Info_Get_ToolList() ;

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
            qDebug("theWidgetItem size = %d, %d", item_size.width(), item_size.height()) ;

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
	std::string str_able_tools_list_xml = Ensemble_Job_Type_Get_List_Xml() ;

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
            qDebug("theWidgetItem size = %d, %d", item_size.width(), item_size.height()) ;

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

void MainWindow::OnButtonNewProject(void)
{
    //New Job Dialog
    CDialogNewProject dlg_new_project ;

    int dialogCode = dlg_new_project.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        //EnsembleJobNew(dlg_new_project.GetName()) ;
        Ensemble_Project_Add_New(dlg_new_project.GetName()) ;

        UpdateJobTree();
    }
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


void MainWindow::UpdateJobTree(void)
{
	qDebug("%s", __func__) ;
	
    //delete all item
    ui->treeWidget_job->clear();

    std::string str_prj_list_xml = Ensemble_Project_Get_List() ;

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

        for (pugi::xml_node device : doc.children("Ensemble") )
        {
            std::string str_device_model_name = device.attribute("Model").value() ;
            std::string str_device_mac = device.attribute("Mac").value() ;

            qDebug("xml: Device MAC address=%s", str_device_mac.c_str()) ;
            //AddTreeRoot(QString::fromStdString(str_device), QString::fromStdString("test")) ;

            std::string str_device_info = str_device_model_name + "(" + str_device_mac + ")" ;

            //Add root
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget_job);
            treeItem->setExpanded(true);
            //treeItem->setText(0, QString::fromStdString(str_device_info));

            //Adding the item to the listwidget
            ui->treeWidget_job->addTopLevelItem(treeItem);

            //Creating an object of the designed widget which is to be added to the listwidget
            FormDeviceInfo *theWidgetItem = new FormDeviceInfo;

            //---
            //Set Informationupda
            if( Ensemble_Network_IsOnline() & ENSEMBLE_CONNECT_CONTROL_PORT )
            {
                unsigned char* get_data = NULL ;
                int width = 0 ;
                int height = 0 ;
				const int image_type = IMAGE_RGB888 ;
                Ensemble_Source_Get_Image(GET_IMAGE_DEVICE_ICON, std::string(), image_type, &get_data, &width, &height) ;

                //qDebug("Get Image Size = %d x %d", width, height) ;

                if( get_data != NULL )
                {
                    if( width>0 && height >0 )
                    {
                    	cv::Mat get_image ;
						cv::Mat icon ;
                    	if( image_type == IMAGE_YUV420 )
						{
							//YUV420 
					        cv::Mat get_image(height + width / 2, width, CV_8UC1, get_data) ;

					        CImgDec cls_image_decoder ;
					        icon = cls_image_decoder.Decoding(get_image) ;

						}
						else if( image_type == IMAGE_RGB888 )
						{
							cv::Mat get_image(height, width, CV_8UC3, get_data) ;
							cv::cvtColor(get_image, icon, cv::COLOR_BGR2RGB) ;
						}
						
   						CMat2QImage cls_mat_2_qimage ;
						QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(icon, icon.cols, icon.rows) ;

						theWidgetItem->SetIconInfo(qt_display_image);
                    }

                    delete [] get_data ;
                    get_data = NULL ;
                }
            }

            theWidgetItem->SetDeviceNameInfo(str_device_model_name);
            theWidgetItem->SetMacAddressInfo(str_device_mac);
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

                FormProject *theWidgetItem = new FormProject;

				connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
				connect(theWidgetItem, SIGNAL(UpdateResultImage(QString)), this, SLOT(UpdateResultImage(QString))) ;
				
                theWidgetItem->SetNameInfo(str_prj_name);
                theWidgetItem->SetIdInfo(str_prj_id);

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
					
					std::string str_tool_type_name = Ensemble_Info_Get_ToolTypeName(type) ;
					
                    qDebug("Job : Type=%d, TypeName=%s, Name=%s", type, str_tool_type_name.c_str(), str_name.c_str()) ;

                    QTreeWidgetItem *treeJobItem = new QTreeWidgetItem(JobType::JOB_TYPE_BASE);
                    treeChileItem->setExpanded(true);
                    treeChileItem->addChild(treeJobItem);

					treeJobItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_job_id));
					
                    FormJobBase *theWidgetItem = new FormJobBase;
					theWidgetItem->SetNameInfo(str_name);
                	theWidgetItem->SetIdInfo(str_job_id);
				
					connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
					connect(theWidgetItem, SIGNAL(UpdateResultImage(QString)), this, SLOT(UpdateResultImage(QString))) ;
					
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
						
						std::string str_tool_type_name = Ensemble_Info_Get_ToolTypeName(type) ;
						
	                    qDebug("Tool Type=%d, TypeName=%s, Name=%s", type, str_tool_type_name.c_str(), str_name.c_str()) ;

	                    QTreeWidgetItem *treeToolItem = new QTreeWidgetItem(JobType::JOB_TYPE_TOOL);
	                    treeJobItem->setExpanded(true);
	                    treeJobItem->addChild(treeToolItem);

						treeToolItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_id));
						
	                    FormJobTool *theWidgetItem = new FormJobTool;
	                    theWidgetItem->SetNameInfo(str_name);
	                    theWidgetItem->SetIdInfo(str_id);
						theWidgetItem->SetParentIdInfo(str_job_id);
	                    theWidgetItem->SetTypeInfo(str_tool_type_name);
						theWidgetItem->SetType(type) ;

						connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
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
							
							std::string str_tool_option_type_name = Ensemble_Info_Get_ToolTypeName(option_type) ;
							
		                    qDebug("Tool Option Type=%d, TypeName=%s, Name=%s", option_type, str_tool_option_type_name.c_str(), str_option_name.c_str()) ;

		                    QTreeWidgetItem *treeToolOptionItem = new QTreeWidgetItem(JobType::JOB_TYPE_TOOL_OPTION);
		                    treeToolItem->setExpanded(true);
		                    treeToolItem->addChild(treeToolOptionItem);

							treeToolOptionItem->setData(0, Qt::UserRole+1, QString::fromStdString(str_option_id));
							
		                    FormToolOption *theWidgetItem = new FormToolOption;
		                    theWidgetItem->SetNameInfo(str_option_name);
		                    theWidgetItem->SetIdInfo(str_option_id);
							theWidgetItem->SetParentIdInfo(str_id);			//Parent Type is Tool
		                    theWidgetItem->SetTypeInfo(str_tool_option_type_name);
							theWidgetItem->SetType(option_type) ;

							connect(theWidgetItem, SIGNAL(UpdateList()), this, SLOT(UpdateJobTree())) ;
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

void MainWindow::OnButtonSaveAllTask(void)
{
	Ensemble_Task_File_Save() ;
}

void MainWindow::OnButtonLoadAllTask(void)
{
	Ensemble_Task_File_Load() ;

	UpdateJobTree() ;
}

