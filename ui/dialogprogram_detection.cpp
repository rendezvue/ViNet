#include "dialogprogram_detection.h"
#include "ui_dialogprogram_detection.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "formproject.h"
#include "ui_formproject.h"

dialogprogram_detection::dialogprogram_detection(IndyDCPConnector* IndyDCP, QTreeWidget* TreeWidget,QWidget *parent) :
    QDialog(parent),m_TreeWidget(TreeWidget),m_IndyDCP(IndyDCP),
    ui(new Ui::dialogprogram_detection)
{
    ui->setupUi(this);
    ui->Edit_Z->setText("0");
}

dialogprogram_detection::~dialogprogram_detection()
{
    delete ui;
}

void dialogprogram_detection::on_pushButton_Get_JobList_clicked()
{
	qDebug("%s", __func__) ;

    ui->listWidget_JobList->clear();
    //delete all item
//    ui->treeWidget_job->clear();

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
					
					std::string str_tool_type_name = Ensemble_Info_Get_ToolTypeName(type) ;
					
                    qDebug("Job : Id = %s Type=%d, TypeName=%s, Name=%s", str_job_id.c_str(), type, str_tool_type_name.c_str(), str_name.c_str()) ;
				}
			}
        }
    }

}

void dialogprogram_detection::on_pushButton_Add_Job_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();
    QString item_str;
    QList<QListWidgetItem*> listitems =  ui->listWidget_JobList->selectedItems();
    if( listitems.size() == 0 )
    {
        return;
    }
    else
    {
        item_str = listitems[0]->text();
    }

    QString job_str;
    job_str.sprintf("Job|%s",item_str.toStdString().c_str());

    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();

    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,job_str);
    //myParent->on_TreeWidget_Put_Item(new_child);

    QTreeWidgetItem* parentItem = SelectedItem[0]->parent();

    if( SelectedItem[0]->text(0) == "Detection")
    {
        SelectedItem[0]->addChild(new_child);
        SelectedItem[0]->setExpanded(true);
    }
    else
    {
        int Index_selectedItem = parentItem->indexOfChild(SelectedItem[0]);
        parentItem->insertChild(Index_selectedItem+1,new_child);
    }
}

void dialogprogram_detection::on_Run_Job_clicked()
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

    Run_Job(qstr_job_id);
}

void dialogprogram_detection::Run_Job(QString qstr_job_id)
{
	std::string str_result_xml = Ensemble_Job_Run(qstr_job_id.toStdString().c_str()) ;

	qDebug("Project Result = %s", str_result_xml.c_str()) ;

//	QString qstr_id = QString::fromStdString(GetIdInfo());

//	emit UpdateResultImage(qstr_id) ;
//	emit UpdateResult(QString::fromStdString(str_result_xml)) ;

    UpdateResult(str_result_xml);

    if( m_detection_result.size() > 0 )
    {
        for( int i = 0 ; i < m_detection_result.size() ; i++ )
        {
            float pixel_x = m_detection_result[i].pixel_center_x / m_detection_result[i].src_width;
            float pixel_y = m_detection_result[i].pixel_center_y / m_detection_result[i].src_height;
            float robot_x = 0;
            float robot_y = 0;

            qDebug("x,y,angle,score=%f/%f/%f/%f\n",pixel_x,pixel_y,m_detection_result[i].angle, m_detection_result[i].score);

            Ensemble_Job_Calibration_GetPoint(qstr_job_id.toStdString(), pixel_x, pixel_y, &robot_x, &robot_y);
            m_detection_result[i].mm_center_x = robot_x;
            m_detection_result[i].mm_center_y = robot_y;
            qDebug("m_detection_result[%d].mm_center_x = %f\n",i,m_detection_result[i].mm_center_x);
            qDebug("m_detection_result[%d].mm_center_y = %f\n",i,m_detection_result[i].mm_center_y);

        }
    }
}

void dialogprogram_detection::UpdateResult(std :: string result_xml)
{
 //   qDebug("UpdateResult : xml = %s", qUtf8Printable(qstr_xml)) ;
    m_detection_result.clear();
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

    std::string str_xml = result_xml;//qstr_xml.toStdString();

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
            		int src_width = job.child("Src").attribute("Width").as_int();
            		int src_height = job.child("Src").attribute("Height").as_int();
            		float center_x = job.child("Pose").attribute("CenterX").as_float() ;
					float center_y = job.child("Pose").attribute("CenterY").as_float() ;
					float angle = job.child("Pose").attribute("Angle").as_float() ;
					float score = job.child("Matching").attribute("Score").as_float() ;
					
                    detection_result det_result;
                    det_result.pixel_center_x = center_x;
                    det_result.pixel_center_y = center_y;
                    det_result.angle = angle;
                    det_result.score = score;
                    det_result.src_width = src_width;
                    det_result.src_height = src_height;
                    m_detection_result.push_back(det_result);

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

			/*	//Find Job in List				
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
				}	*/			
			}
		}

	/*	int size_alarm = vec_result_change_color_id.size() ;
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
		}*/

		//emit UpdateFormInfo() ;
	}
}

void dialogprogram_detection::on_pushButton_Go_PickPosition_clicked()
{
    on_pushButton_Get_clicked();
#if 0
    QString qstr_z = ui->Edit_Z->toPlainText();
    QString qstr_u = ui->Edit_U->toPlainText();
    QString qstr_v = ui->Edit_V->toPlainText();
    QString qstr_w = ui->Edit_W->toPlainText();
#else
    QString qstr_x = ui->label_X_SetPos->text();
    QString qstr_y = ui->label_Y_SetPos->text();
    QString qstr_z = ui->label_Z_SetPos->text();
    QString qstr_u = ui->label_U_SetPos->text();
    QString qstr_v = ui->label_V_SetPos->text();
    QString qstr_w = ui->label_W_SetPos->text();
#endif
    Run_PickPos(qstr_x, qstr_y,qstr_z,qstr_u,qstr_v,qstr_w);
}

void dialogprogram_detection::Run_PickPos(QString qstr_x, QString qstr_y, QString qstr_z, QString qstr_u, QString qstr_v, QString qstr_w )
{
    if( m_detection_result.size() < 1 )
    {
        return;
    }

    /*************get robot pos**********/
    double ret[6];
    m_IndyDCP->getTaskPosition(ret);
    double dist_unit = 1000.0;
    /****************************************/

    float offset_x = 0;
    float offset_y = 0;

    float target_z = 0;
    float target_u = 180;
    float target_v = 0;
    float target_w = 0;

    if( qstr_x != "" )
    {
        offset_x = qstr_x.toFloat() - test_base_task[0];
        //offset_x = qstr_x.toFloat();
    }
    if( qstr_y != "" )
    {
        offset_y = qstr_y.toFloat() - test_base_task[1];
        //offset_y = qstr_y.toFloat();
    }
    if( qstr_z != "" )
    {
        target_z = qstr_z.toFloat();
    }
    if( qstr_u != "" )
    {
        target_u = qstr_u.toFloat();
    }
    if( qstr_v != "" )
    {
        target_v = qstr_v.toFloat();
    }
    if( qstr_w != "" )
    {
        target_w = qstr_w.toFloat();
    }

    float cur_robot_x = ret[0]*dist_unit;
    float cur_robot_y = ret[1]*dist_unit;
    float cur_robot_z = ret[2]*dist_unit;

    int index = 0;

    float target_x = m_detection_result[index].mm_center_x;
    float target_y = m_detection_result[index].mm_center_y;

    qDebug("target x,y,z (%f/%f/%f)\n",target_x, target_y, target_z);


    double final_ret[6];

	QString qstr_angle = ui->Edit_Angle->toPlainText();
	double dest_angle = qstr_angle.toFloat();
	int CalcAngle_Final = calc_PickAngle(dest_angle);

    final_ret[0] = (target_x+offset_x) / dist_unit;
    final_ret[1] = (target_y+offset_y) / dist_unit;
    final_ret[2] = target_z / dist_unit;
    final_ret[3] = target_u ;
    final_ret[4] = target_v ;
    final_ret[5] = target_w;//ret[5] - CalcAngle_Final;

    m_IndyDCP->moveTaskTo(final_ret);

	int loop_cnt = 0;
	while (1)
	{
		bool check_finish;
        m_IndyDCP->isMoveFinished(check_finish);
		if (check_finish == true)
		{
			break;
		}
        usleep(50*1000);
		loop_cnt += 50;
		if (loop_cnt > 3000)
		{
			break;
		}
	}


/*    DialogProgram* myParent = (DialogProgram*)this->parent();
    myParent->m_dlg_framemove->Program_Run_Frame_Move(
                final_ret[0] ,
                final_ret[1] ,
                final_ret[2] ,
                final_ret[3] ,
                final_ret[4] ,
                final_ret[5] );*/

}

double dialogprogram_detection::calc_PickAngle(int dest_angle)
{
	double ret[6];
    m_IndyDCP->getJointPosition(ret);

	int cur_joint6_angle = ret[5]; // joint6 의 각도

    int index = 0;
    int angle = m_detection_result[index].angle;

    qDebug("=================================\n");
    qDebug("Obj Angle = %d\n", angle);
    qDebug("BOT Angle = %d\n", cur_joint6_angle);
    qDebug("Dst Angle = %d\n", dest_angle);

	int CalcAngle_Final;
	int CalcAngle = dest_angle + angle;// -cur_joint6_angle + angle;

    qDebug("1. Calc Angle = %d\n", CalcAngle);
	CalcAngle_Final = CalcAngle;
	int CalcAngle2;
	if ((CalcAngle+ cur_joint6_angle) > 180)
	{
		CalcAngle2 = CalcAngle - 360;
        qDebug(" 2. Calc Angle 2 = %d\n", CalcAngle2);
		CalcAngle_Final = CalcAngle2;
	}
	else if ((CalcAngle + cur_joint6_angle) < -180)
	{
		CalcAngle2 = CalcAngle + 360;
        qDebug(" 2. Calc Angle 2 = %d\n", CalcAngle2);
		CalcAngle_Final = CalcAngle2;
	}
    qDebug("Calc Angle Final = %d\n", CalcAngle);
    return CalcAngle_Final;
}

void dialogprogram_detection::Run_PickAngle(QString qstr_angle)
{
    if( m_detection_result.size() < 1 )
    {
        return;
    }

	int dest_angle = qstr_angle.toInt();

	int CalcAngle_Final = calc_PickAngle(dest_angle);

    double move_joint[6] = {0,};
    move_joint[5] = CalcAngle_Final;
    m_IndyDCP->moveJointBy(move_joint);

    int loop_cnt = 0;
	while (1)
	{
		bool check_finish;
        m_IndyDCP->isMoveFinished(check_finish);
		if (check_finish == true)
		{
			break;
		}
        usleep(50*1000);
		loop_cnt += 50;
		if (loop_cnt > 3000)
		{
			break;
		}
	}
}

void dialogprogram_detection::on_pushButton_Add_Go_PickPosition_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();

    QString qstr_z = ui->label_Z_SetPos->text();
    QString qstr_u = ui->label_U_SetPos->text();
    QString qstr_v = ui->label_V_SetPos->text();
    QString qstr_w = ui->label_W_SetPos->text();

    QString job_str;
    job_str.sprintf("PickPos|%s,%s,%s,%s",
                    qstr_z.toStdString().c_str() ,
                    qstr_u.toStdString().c_str() ,
                    qstr_v.toStdString().c_str() ,
                    qstr_w.toStdString().c_str() );

    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();

    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,job_str);
    //myParent->on_TreeWidget_Put_Item(new_child);

    QTreeWidgetItem* parentItem = SelectedItem[0]->parent();

    if( SelectedItem[0]->text(0) == "Detection")
    {
        SelectedItem[0]->addChild(new_child);
        SelectedItem[0]->setExpanded(true);
    }
    else
    {
        int Index_selectedItem = parentItem->indexOfChild(SelectedItem[0]);
        parentItem->insertChild(Index_selectedItem+1,new_child);
    }
}

void dialogprogram_detection::Program_Run_Job(std::string command, std::string desc)
{
	string line = desc;
    DialogProgram* myParent = (DialogProgram*)this->parent();
    vector<string> line_vector = myParent->split(line, ',');
 
    for (int i = 0; i < line_vector.size(); ++i)
        cout << line_vector[i] << endl;

	string job_id = line_vector[0];

	QString qstr_job_id = job_id.c_str();
	Run_Job(qstr_job_id);
}
void dialogprogram_detection::Program_Run_PickPos(std::string command, std::string desc)
{
	string line = desc;
    DialogProgram* myParent = (DialogProgram*)this->parent();
    vector<string> line_vector = myParent->split(line, ',');
 
    for (int i = 0; i < line_vector.size(); ++i)
        cout << line_vector[i] << endl;

    string str_target_z = line_vector[0];
    string str_target_u = line_vector[1];
    string str_target_v = line_vector[2];
    string str_target_w = line_vector[3];

    QString qstr_z = str_target_z.c_str();
    QString qstr_u = str_target_u.c_str();
    QString qstr_v = str_target_v.c_str();
    QString qstr_w = str_target_w.c_str();

 //   Run_PickPos(qstr_z,qstr_u,qstr_v,qstr_w);
}

void dialogprogram_detection::on_pushButton_Go_AnglePosition_clicked()
{
    QString qstr_angle = ui->Edit_Angle->toPlainText();
    Run_PickAngle(qstr_angle);
}

void dialogprogram_detection::on_pushButton_Get_clicked()
{
    double ret[6];
    m_IndyDCP->getTaskPosition(ret);

    double dist_unit = 1000.0;

    QString str;
    str = QString::number(ret[0]*dist_unit);
    ui->Edit_X->setText(str);
    str = QString::number(ret[1]*dist_unit);
    ui->Edit_Y->setText(str);
    str = QString::number(ret[2]*dist_unit);
    ui->Edit_Z->setText(str);
    str = QString::number(ret[3]);
    ui->Edit_U->setText(str);
    str = QString::number(ret[4]);
    ui->Edit_V->setText(str);
    str = QString::number(ret[5]);
    ui->Edit_W->setText(str);
}

void dialogprogram_detection::on_pushButton_Set_PickPos_clicked()
{
    ui->label_X_SetPos->setText(ui->Edit_X->toPlainText());
    ui->label_Y_SetPos->setText(ui->Edit_Y->toPlainText());
    ui->label_Z_SetPos->setText(ui->Edit_Z->toPlainText());
    ui->label_U_SetPos->setText(ui->Edit_U->toPlainText());
    ui->label_V_SetPos->setText(ui->Edit_V->toPlainText());
    ui->label_W_SetPos->setText(ui->Edit_W->toPlainText());
}

void dialogprogram_detection::on_pushButton_Set_BasePos_clicked()
{
    double ret[6];
    m_IndyDCP->getTaskPosition(ret);

    double dist_unit = 1000.0;

    QString str;
    test_base_task[0] = ret[0]*dist_unit;

    test_base_task[1] = (ret[1]*dist_unit);

    test_base_task[2] = (ret[2]*dist_unit);

    test_base_task[3] = (ret[3]);

    test_base_task[4] = (ret[4]);

    test_base_task[5] = (ret[5]);

}
