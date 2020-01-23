#include "dialogprogram.h"
#include "ui_dialogprogram.h"
#include "dialogprogram_jointmove.h"
#include "dialogprogram_framemove.h"
#include <QMessageBox>

#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QStatusBar>
#include <QtWidgets/qtreewidget.h>
#define ROBOT_CMD_MoveJ        	"MoveJ"
#define ROBOT_CMD_MoveF         "MoveF"

/*
#define ROBOT_CMD_DETECTION  	"DETECTION"
#define ROBOT_CMD_RELATIVE  	"RELATIVE"
#define	ROBOT_CMD_GRIPPER		"GRIPPER"
#define	ROBOT_CMD_LOOP_START	"LOOP_START"
#define	ROBOT_CMD_LOOP_OUT		"LOOP_OUT"
#define	ROBOT_CMD_LOOP_END		"LOOP_END"
#define	ROBOT_CMD_IF_START		"IF_START"
#define	ROBOT_CMD_IF_END		"IF_END"
#define	ROBOT_CMD_DETECTMOVE	"DETECTMOVE"
#define	ROBOT_CMD_WAITING		"WAITING"
#define	ROBOT_CMD_VISION_IPM	"VISION_IPM"
#define	ROBOT_CMD_ASSIGNMENT	"ASSIGNMENT"
#define	ROBOT_CMD_ANGLEMOVE		"ANGLEMOVE"
#define	ROBOT_CMD_BOUNDARY_LEVEL"BOUNDARY_LEVEL"*/

#define ROBOT_CMD_TOP_ITEM       "Robot Program"

DialogProgram::DialogProgram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgram)
{
    m_IndyDCP_connector = NULL;
    ui->setupUi(this);
    ui->lineEdit_ip_address->setInputMask("000.000.000.000;_");
    std::string str_ip_address = "192.168.212.19" ;
    ui->lineEdit_ip_address->setText(QString::fromUtf8(str_ip_address.c_str()));

    Init_ToolBox();
    Init_ProgramBox();

    m_dlg_jointmove = NULL;
    m_dlg_framemove = NULL;
}

DialogProgram::~DialogProgram()
{
    delete ui;
}

void DialogProgram::Init_ProgramBox()
{
    QList <QTreeWidgetItem*> itemlist =  ui->treeWidget_Program->findItems(ROBOT_CMD_TOP_ITEM, Qt::MatchContains|Qt::MatchRecursive,0);
    itemlist[0]->setSelected(true);
}

void DialogProgram::Init_ToolBox()
{
    m_Widget_ToolBox = ui->listWidget_ToolBox;
    m_Widget_ToolBox->addItem(ROBOT_CMD_MoveJ   		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_MoveF    		);


/*    m_Widget_ToolBox->addItem(ROBOT_CMD_DETECTION       );
    m_Widget_ToolBox->addItem(ROBOT_CMD_RELATIVE        );
    m_Widget_ToolBox->addItem(ROBOT_CMD_GRIPPER         );
    m_Widget_ToolBox->addItem(ROBOT_CMD_LOOP_START      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_LOOP_OUT		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_LOOP_END		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_IF_START		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_IF_END          );
    m_Widget_ToolBox->addItem(ROBOT_CMD_DETECTMOVE      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_WAITING         );
    m_Widget_ToolBox->addItem(ROBOT_CMD_VISION_IPM      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_ASSIGNMENT      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_ANGLEMOVE		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_BOUNDARY_LEVEL  );*/

    int count = m_Widget_ToolBox->count();

    for( int i = 0 ;i < count ; i++ )
    {
        QListWidgetItem *item = m_Widget_ToolBox->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(),30));
    }

}

void DialogProgram::on_pushButton_Connect_Indy7_clicked()
{
    QString qstr_ip_address = ui->lineEdit_ip_address->text() ;
    bool connection;
    if( m_IndyDCP_connector == NULL )
    {
        m_IndyDCP_connector = new IndyDCPConnector(qstr_ip_address.toStdString(),ROBOT_INDY7);
        connection = m_IndyDCP_connector->connect();
        qDebug("Connection = %d\n",connection);
    }
    if( connection == true )
    {
        QMessageBox::information(this,tr("INDY 7 Conenect OK"),tr("This is a ..."));
    }
    else
    {
        QMessageBox::information(this,tr("INDY 7 Conenect Fail"),tr("This is a ..."));
    }
    double ret[6];
    m_IndyDCP_connector->getTaskPosition(ret);
    qDebug("ret[0]=%f\n",ret[0]);
}

void DialogProgram::on_listWidget_ToolBox_itemClicked(QListWidgetItem *item)
{
    qDebug("item click!\n");
    on_TreeWidget_Put_Item(item);
}

void DialogProgram::on_TreeWidget_Put_Item(QListWidgetItem *Listitem)
{
    QList <QTreeWidgetItem*> itemtree = ui->treeWidget_Program->selectedItems();
    QTreeWidgetItem* parentItem = itemtree[0]->parent();
    QTreeWidgetItem* selectedItem = itemtree[0];

    /*****************New Item ****************/
    QString item_text = Listitem->text();
    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,item_text);
    /******************************************/

    /**********put tree***************************/

    QTreeWidgetItem* Calc_Parent;
    int Calc_Index;

    bool first_child = false;

    if( selectedItem->text(0) == ROBOT_CMD_TOP_ITEM )
    {
        selectedItem->addChild(new_child);
        selectedItem->setExpanded(true);

//        Calc_Parent = selectedItem;
//        first_child = true;
    }
    else
    {
        if( item_text == ROBOT_CMD_MoveJ )
        {
            if( parentItem->text(0) ==ROBOT_CMD_MoveJ || parentItem->text(0) == ROBOT_CMD_MoveF )
            {
                return;
            }
        }
        if( item_text == ROBOT_CMD_MoveF )
        {
            if( parentItem->text(0) == ROBOT_CMD_MoveF || parentItem->text(0) == ROBOT_CMD_MoveJ )
            {
                return;
            }
        }


        int childCount = parentItem->childCount();

        if( childCount == 0 )
        {
            parentItem->addChild(new_child);
            parentItem->setExpanded(true);
        }
        else
        {
            int Index_selectedItem = parentItem->indexOfChild(selectedItem);
            parentItem->insertChild(Index_selectedItem+1,new_child);
        }
    }
}

void DialogProgram::on_treeWidget_Program_itemClicked(QTreeWidgetItem *item, int column)
{
//    QList <QTreeWidgetItem*> itemtree = ui->treeWidget_Program->selectedItems();
    QString item_text=item->text(0);
    QString parent_text = "";
    if(item->parent())
    {
        parent_text = item->parent()->text(0);
    }
    if( item_text == ROBOT_CMD_MoveJ || parent_text == ROBOT_CMD_MoveJ )
    {        
        if( m_dlg_jointmove == NULL )
        {
            m_dlg_jointmove = new dialogprogram_jointmove(m_IndyDCP_connector,ui->treeWidget_Program,this);
            QMdiSubWindow *window = ui->mdiArea_SubSetting->addSubWindow(m_dlg_jointmove,Qt::FramelessWindowHint);
        }

        m_dlg_jointmove->showMaximized();
        m_dlg_jointmove->ParseString();
    }
    if( item_text == ROBOT_CMD_MoveF || parent_text == ROBOT_CMD_MoveF )
    {
        if( m_dlg_framemove == NULL )
        {
            m_dlg_framemove = new dialogprogram_framemove(m_IndyDCP_connector,ui->treeWidget_Program,this);
            QMdiSubWindow *window = ui->mdiArea_SubSetting->addSubWindow(m_dlg_framemove,Qt::FramelessWindowHint);
        }

        m_dlg_framemove->showMaximized();
        m_dlg_framemove->ParseString();
    }
}

vector<string> DialogProgram::split(string str, char delimiter)
{
    vector<string> internal;
    stringstream ss(str);
    string temp;

    while (getline(ss, temp, delimiter)) {
        internal.push_back(temp);
    }
    return internal;
}

void DialogProgram::writeIndexEntry(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item)
{
    xmlWriter->writeStartElement("attribute");
    xmlWriter->writeAttribute("Item", item->text(0));
    QString valueString = item->text(1);
    if (!valueString.isEmpty()) {
        QStringList values = valueString.split(", ");
        foreach (QString value, values)
            xmlWriter->writeTextElement("value", value);
    }
    for (int i = 0; i < item->childCount(); ++i)
        writeIndexEntry(xmlWriter, item->child(i));
    xmlWriter->writeEndElement();
}
bool DialogProgram::writeXml(const QString &fileName, QTreeWidget *treeWidget)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
    //    QDebug()>>"Error: Cannot write file ";

        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("cell");
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
        writeIndexEntry(&xmlWriter, treeWidget->topLevelItem(i));
    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
  //      QDebug()>> "Error: Cannot write file ";
        return false;
    }
    return true;
}


void DialogProgram::on_pushButton_Load_clicked()
{
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Open Bookmark File"),
                                            QDir::currentPath(),
                                            tr("XBEL Files (*.xbel *.xml)"));
   if (fileName.isEmpty())
       return;

   //treeWidget->clear();
   ui->treeWidget_Program->clear();


   QFile file(fileName);
   if (!file.open(QFile::ReadOnly | QFile::Text)) {
       QMessageBox::warning(this, tr("QXmlStream Bookmarks"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
       return;
   }

   XbelReader reader(ui->treeWidget_Program);
   reader.read(&file);
}

void DialogProgram::on_pushButton_Save_clicked()
{
    writeXml("TEST.xml", ui->treeWidget_Program);


}
