#include "dialogselecttooloption.h"
#include "ui_dialogselecttooloption.h"

DialogSelectToolOption::DialogSelectToolOption(QWidget *parent) :
    QDialog(parent),
	m_i_type(0),
    ui(new Ui::DialogSelectToolOption)
{
    ui->setupUi(this);

	//update list
	connect(ui->pushButton_updast_list, SIGNAL(clicked()), this,  SLOT(OnButtonUpdateList())) ;

	//add option
	connect(ui->pushButton_add_option, SIGNAL(clicked()), this,  SLOT(OnButtonAddOption())) ;
}

DialogSelectToolOption::~DialogSelectToolOption()
{
    ui->listWidget_option_list->clear() ;

    delete ui;
}

void DialogSelectToolOption::SetToolType(const int type)
{
	m_i_type = type ;

	std::string str_type = "Unknown Tool" ;
	if( m_i_type == TOOL_TYPE_DETECT_OBJECT	) str_type = "(" + std::to_string(m_i_type) + ")" + " Detect Object" ;
	else if( m_i_type == TOOL_TYPE_DETECT_LINE	) str_type = "(" + std::to_string(m_i_type) + ")" + " Detect Line" ;
	else if( m_i_type == TOOL_TYPE_DETECT_CIRCLE ) str_type = "(" + std::to_string(m_i_type) + ")" + " Detect Circle" ;
	else if( m_i_type == TOOL_TYPE_CALC_DISTANCE ) str_type = "(" + std::to_string(m_i_type) + ")" + " Calc. Distance" ;
	else if( m_i_type == TOOL_TYPE_CALC_ANGLE ) str_type = "(" + std::to_string(m_i_type) + ")" + " Calc. Angle" ;
	
	ui->label_type->setText(QString::fromUtf8(str_type.c_str()));
}

void DialogSelectToolOption::SetToolId(const std::string id)
{
	m_str_id = id ;

	ui->label_id->setText(QString::fromUtf8(m_str_id.c_str()));
}

void DialogSelectToolOption::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Option List
    OnButtonUpdateList() ;
}

void DialogSelectToolOption::OnButtonAddOption(void)
{
	QModelIndexList list = ui->listWidget_option_list->selectionModel()->selectedIndexes();
	foreach(const QModelIndex &index, list)
	{
        unsigned int item_type = index.data(Qt::UserRole+1).toUInt() ;

		Ensemble_Tool_Add_NewOption(m_str_id, item_type) ;
		
        qDebug("Add New Option Type = %d", item_type) ;
	}

	//update job information list on mainwindow
	emit UpdateList() ;
}

void DialogSelectToolOption::OnButtonUpdateList(void)
{
	std::string str_able_tool_opion_list_xml = Ensemble_Tool_Get_OptionList(m_i_type) ;

	qDebug("tools option xml = %s", str_able_tool_opion_list_xml.c_str()) ;

	//Upate Job List
	//XML Parsing
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string((char *)(str_able_tool_opion_list_xml.c_str()));


    ui->listWidget_option_list->clear() ;

	if (!result)
	{
		qDebug("tools list xml parsing error") ;
	}
	else
	{
		for (pugi::xml_node tool: doc.child("Ability").child("Tools").children("Option"))
		{
            int type = tool.attribute("Type").as_int();
            std::string str_type = tool.attribute("TypeName").value();
			std::string str_name = tool.attribute("Name").value();
            std::string str_item = "(" + str_type + ") " + str_name ;

            qDebug("TypeName=%s, Name=%s, item=%s", str_type.c_str(), str_name.c_str(), str_item.c_str()) ;

            QString str_item_text = QString::fromUtf8(str_item.c_str()) ;
				
			QListWidgetItem *newItem = new QListWidgetItem;
		    newItem->setText(str_item_text);
		    ui->listWidget_option_list->addItem(newItem);

			//set type information
			newItem->setData(Qt::UserRole+1, type);
			
		}
	}
}


