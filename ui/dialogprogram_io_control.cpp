#include "dialogprogram_io_control.h"
#include "ui_dialogprogram_io_control.h"

#include "EnsembleAPI.h"


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formproject.h"
#include "ui_formproject.h"

dialogprogram_io_control::dialogprogram_io_control(QTreeWidget* TreeWidget,QWidget *parent) :
    QDialog(parent),m_TreeWidget(TreeWidget),
    ui(new Ui::dialogprogram_io_control)
{
    ui->setupUi(this);

    ui->comboBox_Out_Pinnum->addItem("1");
    ui->comboBox_Out_Pinnum->addItem("2");
    ui->comboBox_Out_Pinnum->addItem("3");

    ui->comboBox_Out_Value->addItem("1");
    ui->comboBox_Out_Value->addItem("0");

    ui->comboBox_In_Pinnum->addItem("1");
    ui->comboBox_In_Pinnum->addItem("2");

    ui->comboBox_In_Value->addItem("1");
    ui->comboBox_In_Value->addItem("0");

    ui->comboBox_In_State->addItem("==");
    ui->comboBox_In_State->addItem("!=");

//    ui->comboBox_In_Pinnum->addItem("3");
//    ui->comboBox_In_Pinnum->addItem("4");

//    ui->comboBox_Out_Pinnum->addItem("4");
}

dialogprogram_io_control::~dialogprogram_io_control()
{
    delete ui;
}

void dialogprogram_io_control::on_pushButton_Get_Digital_IO_clicked()
{
    QString qstr_job_id;
    QList<QListWidgetItem*> listitems =  ui->listWidget_JobList->selectedItems();
    if( listitems.size() == 0 )
    {
        return;
    }
    else
    {
        qstr_job_id = listitems[0]->text();
    }

    int io_val = CEnsemble::getInstance()->m_cls_api.Ensemble_Digital_IO_GetIn(qstr_job_id.toStdString());
    qDebug("io_val = 0x%x\n", io_val);
    //Run_Job(qstr_job_id);
}

void dialogprogram_io_control::on_pushButton_Get_JobList_clicked()
{
    qDebug("%s", __func__) ;

    ui->listWidget_JobList->clear();
    //delete all item
//    ui->treeWidget_job->clear();

    std::string str_prj_list_xml = CEnsemble::getInstance()->m_cls_api.Ensemble_Project_Get_List() ;

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

            int project_count = 0 ;
            for (pugi::xml_node project: device.children("Project"))
            {
                std::string str_prj_id = project.attribute("ID").value();
                std::string str_prj_name = project.attribute("Name").value();

                qDebug("Project[%d]: id=%s, name=%s", project_count, str_prj_id.c_str(), str_prj_name.c_str()) ;


//                theWidgetItem->SetIdInfo(str_prj_id);



                project_count++ ;

//				//Jobs
                for (pugi::xml_node job: project.child("Jobs").children("Job"))
                {
                    std::string str_job_id = job.attribute("ID").value();

                    QString qstr_job_id = str_job_id.c_str();

                    ui->listWidget_JobList->addItem(qstr_job_id);

                    int type = job.attribute("Type").as_int();
                    std::string str_name = job.attribute("Name").value();

                    std::string str_tool_type_name = CEnsemble::getInstance()->m_cls_api.Ensemble_Info_Get_ToolTypeName(type) ;

                    qDebug("Job : Id = %s Type=%d, TypeName=%s, Name=%s", str_job_id.c_str(), type, str_tool_type_name.c_str(), str_name.c_str()) ;
                }
            }
        }
    }
}


void dialogprogram_io_control::on_pushButton_Out_Set_clicked()
{

    QString qstr_job_id;
    QList<QListWidgetItem*> listitems =  ui->listWidget_JobList->selectedItems();
    if( listitems.size() == 0 )
    {
        return;
    }
    else
    {
        qstr_job_id = listitems[0]->text();
    }

    CEnsemble::getInstance()->m_cls_api.Ensemble_Digital_IO_SetOut(qstr_job_id.toStdString(), 1,1);
}

void dialogprogram_io_control::on_pushButton_Out_Add_clicked()
{
    QString qstr_job_id;
    QList<QListWidgetItem*> listitems =  ui->listWidget_JobList->selectedItems();
    if( listitems.size() == 0 )
    {
        return;
    }
    else
    {
        qstr_job_id = listitems[0]->text();
    }
    CEnsemble::getInstance()->m_cls_api.Ensemble_Digital_IO_SetOut(qstr_job_id.toStdString(), 1,0);
}
