#include "dialogdblist.h"
#include "ui_dialogdblist.h"

DialogDbList::DialogDbList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDbList)
{
    ui->setupUi(this);

	m_db_list_model = new QStringListModel(this); ;
}

DialogDbList::~DialogDbList()
{
	if( m_db_list_model != NULL )
    {
        delete m_db_list_model ;
        m_db_list_model = NULL ;
    }
	
    delete ui;
}

//<?xml version='1.0' encoding='UTF-8' standalone='no' ?><DB ID="20200303145335_nyOO2465" Name="Songunsa" />
void DialogDbList::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{ 
		std::string str_db_list_xml = p_device->Ensemble_DB_Get_List() ;

		//qDebug("db list = %s", str_db_list_xml.c_str()) ;

		if( !str_db_list_xml.empty() )
		{
			//xml parsing
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_string((char *)(str_db_list_xml.c_str()));

			if (!result)
			{
				//qDebug("DB list xml parsing error") ;
			}
			else
			{
				ui->listView_db_list->reset();

				QStringList stringListSource;
		        for (pugi::xml_node db: doc.children("DB"))
		        {
		            std::string str_db_id = db.attribute("ID").value();
					std::string str_db_name = db.attribute("Name").value();

					std::string str_db_item = "[" + str_db_id + "] " + str_db_name ;
		            stringListSource << str_db_item.c_str() ;
		        }

		        // Create model
		        m_db_list_model->setStringList(stringListSource);

		        // Glue model and view together
		        ui->listView_db_list->setModel(m_db_list_model);
			}
		}
	}
}

std::vector<std::string> DialogDbList::Get_Sel_DB(void)
{
	std::vector<std::string> vec_path ;

	QStringList list;
	foreach(const QModelIndex &index, ui->listView_db_list->selectionModel()->selectedIndexes())
	{
		QString itemText = index.data(Qt::DisplayRole).toString();

		std::string str_db = itemText.toUtf8().constData() ;

	    //list.append(model->itemFromIndex(index)->text());

		//ID string parsing
		int start_id = str_db.find("[", 0)+1 ;
		int end_id = str_db.find("]", 0) ;

		std::string str_db_id = str_db.substr(start_id, (end_id - start_id));

		//qDebug("DB sel id = %s(%s)", str_db.c_str(), str_db_id.c_str() ) ;

		if( !str_db_id.empty() )
		{
			vec_path.push_back(str_db_id) ;
		}
	}
	
	return vec_path ;
}

