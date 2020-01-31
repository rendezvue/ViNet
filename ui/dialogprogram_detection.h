#ifndef DIALOGPROGRAM_DETECTION_H
#define DIALOGPROGRAM_DETECTION_H

#include "IndyDCPConnector.h"
#include <QDialog>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>
#include <vector>
using namespace NRMKIndy::Service::DCP;
namespace Ui {
class dialogprogram_detection;
}

typedef struct detection_result{
	int src_width;
	int src_height;
    float pixel_center_x;
    float pixel_center_y;
    float angle;
    float score;
    float mm_center_x;
    float mm_center_y;
};

class dialogprogram_detection : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_detection(IndyDCPConnector* IndyDCP, QTreeWidget* TreeWidget, QWidget *parent = 0);
    ~dialogprogram_detection();

private slots:
    void on_pushButton_Get_JobList_clicked();

    void on_pushButton_Add_Job_clicked();

    void on_Run_Job_clicked();

    void on_pushButton_Go_PickPosition_clicked();

    void on_pushButton_Get_clicked();

    void on_pushButton_Set_Base_Position_clicked();

    void on_pushButton_Set_Obj_Pos_clicked();

    void on_pushButton_Set_Pick_Pos_clicked();

    void on_pushButton_Go_Base_Pos_clicked();

    void on_pushButton_Set_Calc_Offset_clicked();

    void on_pushButton_Set_Pick_Angle_clicked();

    void on_pushButton_Add_Pick_clicked();

private:
    QTreeWidget* m_TreeWidget;
    IndyDCPConnector* m_IndyDCP;
    Ui::dialogprogram_detection *ui;

	void UpdateResult(std::string result_xml);
    std::vector<detection_result> m_detection_result;

	void Run_Job(QString qstr_job_id);
    void Run_PickPos(QString qstr_offset_x, QString qstr_offset_y, QString qstr_offset_z, QString qstr_offset_angle  );
 
    double calc_PickAngle(int dest_angle);

public:
	void Program_Run_Job(std::string command, std::string desc);
	void Program_Run_PickPos(std::string command, std::string desc);
	

public:

	double m_Pos_Base_Pos[6];

	double m_Pos_Obj_Pos[6];

    double m_Pos_Pick_Pos[6];    
    double m_Pos_Calc_Offset[6];

	double m_Angle_UserPick;
    
};

#endif // DIALOGPROGRAM_DETECTION_H
