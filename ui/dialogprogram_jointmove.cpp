#include "dialogprogram_jointmove.h"
#include "ui_dialogprogram_jointmove.h"
#include "dialogprogram.h"

dialogprogram_jointmove::dialogprogram_jointmove(IndyDCPConnector* IndyDCP, QTreeWidget* TreeWidget,QWidget *parent) :
    QDialog(parent),m_TreeWidget(TreeWidget),m_IndyDCP(IndyDCP),
    ui(new Ui::dialogprogram_jointmove)
{
    ui->setupUi(this);   
    ParseString();
}

dialogprogram_jointmove::~dialogprogram_jointmove()
{
    delete ui;
}

void dialogprogram_jointmove::ParseString()
{
    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();
    QString curItem = SelectedItem[0]->text(0);

    DialogProgram* myParent = (DialogProgram*)this->parent();
    vector<string> line_vector = myParent->split(curItem.toStdString(),'|');
    for (int i = 0; i < line_vector.size(); ++i)
		cout << line_vector[i] << endl;

	string cmd = line_vector[0];
    string Desc;
    if( line_vector.size() == 1 )
    {
        Desc = "0,0,0,0,0,0";
    }
    else
    {
        Desc = line_vector[1];
    }

	Setup_UI_Joint_Move(cmd,Desc);
}

void dialogprogram_jointmove::Setup_UI_Joint_Move(std::string command, std::string desc)
{
	string line = desc;
    DialogProgram* myParent = (DialogProgram*)this->parent();
    vector<string> line_vector = myParent->split(line, ',');

	for (int i = 0; i < line_vector.size(); ++i)
		cout << line_vector[i] << endl;
	
	double joint1 = atof(line_vector[0].c_str()) ;
	double joint2 = atof(line_vector[1].c_str()) ;
	double joint3 = atof(line_vector[2].c_str()) ;
	double joint4 = atof(line_vector[3].c_str());
	double joint5 = atof(line_vector[4].c_str());
	double joint6 = atof(line_vector[5].c_str());

    QString str;
    str = QString::number(joint1);
    ui->Edit_J1->setText(str);
    str = QString::number(joint2);
    ui->Edit_J2->setText(str);
    str = QString::number(joint3);
    ui->Edit_J3->setText(str);
    str = QString::number(joint4);
    ui->Edit_J4->setText(str);
    str = QString::number(joint5);
    ui->Edit_J5->setText(str);
    str = QString::number(joint6);
    ui->Edit_J6->setText(str);

//	Program_Run_Joint_Move(joint1, joint2, joint3, joint4, joint5, joint6);
}


void dialogprogram_jointmove::on_pushButton_Add_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();

    double ret[6];
    QString str;
    str = ui->Edit_J1->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_J2->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_J3->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_J4->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_J5->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_J6->toPlainText();
    ret[5] = str.toDouble();

    QString joint_point;
    joint_point.sprintf("Joint|%f,%f,%f,%f,%f,%f",ret[0],ret[1],ret[2],ret[3],ret[4],ret[5]);

    //QString joint_point = "Joint|10,10,10,10,10,10";
    //QString joint_point = "Joint|10,10,10,10,10,10";

    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();

    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,joint_point);
    //myParent->on_TreeWidget_Put_Item(new_child);

    QTreeWidgetItem* parentItem = SelectedItem[0]->parent();




    if( SelectedItem[0]->text(0) == "MoveJ")
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

void dialogprogram_jointmove::on_pushButton_Get_clicked()
{
    double ret[6];
    m_IndyDCP->getTaskPosition(ret);

    QString str;
    str = QString::number(ret[0]);
    ui->Edit_J1->setText(str);
    str = QString::number(ret[1]);
    ui->Edit_J2->setText(str);
    str = QString::number(ret[2]);
    ui->Edit_J3->setText(str);
    str = QString::number(ret[3]);
    ui->Edit_J4->setText(str);
    str = QString::number(ret[4]);
    ui->Edit_J5->setText(str);
    str = QString::number(ret[5]);
    ui->Edit_J6->setText(str);
}

void dialogprogram_jointmove::on_pushButton_Set_clicked()
{
    double ret[6];
    QString str;
    str = ui->Edit_J1->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_J2->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_J3->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_J4->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_J5->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_J6->toPlainText();
    ret[5] = str.toDouble();

    m_IndyDCP->moveTaskTo(ret);
}

void dialogprogram_jointmove::on_pushButton_Relative_Init_clicked()
{
    QString str;
    str = QString::number(0);
    ui->Edit_J1->setText(str);
    str = QString::number(0);
    ui->Edit_J2->setText(str);
    str = QString::number(0);
    ui->Edit_J3->setText(str);
    str = QString::number(0);
    ui->Edit_J4->setText(str);
    str = QString::number(0);
    ui->Edit_J5->setText(str);
    str = QString::number(0);
    ui->Edit_J6->setText(str);
}

void dialogprogram_jointmove::on_pushButton_Relative_Set_clicked()
{
    double ret[6];
    QString str;
    str = ui->Edit_J1->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_J2->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_J3->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_J4->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_J5->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_J6->toPlainText();
    ret[5] = str.toDouble();

    m_IndyDCP->moveTaskBy(ret);
}

void dialogprogram_jointmove::on_pushButton_Relative_Add_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();

    double ret[6];
    QString str;
    str = ui->Edit_J1->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_J2->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_J3->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_J4->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_J5->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_J6->toPlainText();
    ret[5] = str.toDouble();

    QString joint_point;
    joint_point.sprintf("Relative|%f,%f,%f,%f,%f,%f",ret[0],ret[1],ret[2],ret[3],ret[4],ret[5]);

    //QString joint_point = "Relative|10,10,10,10,10,10";
    //QString joint_point = "Relative|10,10,10,10,10,10";

    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();

    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,joint_point);
    //myParent->on_TreeWidget_Put_Item(new_child);

    QTreeWidgetItem* parentItem = SelectedItem[0]->parent();




    if( SelectedItem[0]->text(0) == "MoveJ")
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
