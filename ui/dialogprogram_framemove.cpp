#include "dialogprogram_framemove.h"
#include "ui_dialogprogram_framemove.h"
#include "dialogprogram.h"

dialogprogram_framemove::dialogprogram_framemove(IndyDCPConnector* IndyDCP, QTreeWidget* TreeWidget,QWidget *parent) :
    QDialog(parent),m_TreeWidget(TreeWidget),m_IndyDCP(IndyDCP),
    ui(new Ui::dialogprogram_framemove)
{
    ui->setupUi(this);
    ParseString();
}

dialogprogram_framemove::~dialogprogram_framemove()
{
    delete ui;
}

void dialogprogram_framemove::ParseString()
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

    Setup_UI_Frame_Move(cmd,Desc);
}

void dialogprogram_framemove::Setup_UI_Frame_Move(std::string command, std::string desc)
{
    string line = desc;
    DialogProgram* myParent = (DialogProgram*)this->parent();
    vector<string> line_vector = myParent->split(line, ',');

    for (int i = 0; i < line_vector.size(); ++i)
        cout << line_vector[i] << endl;

    double X = atof(line_vector[0].c_str()) ;
    double Y = atof(line_vector[1].c_str()) ;
    double Z = atof(line_vector[2].c_str()) ;
    double U = atof(line_vector[3].c_str());
    double V = atof(line_vector[4].c_str());
    double W = atof(line_vector[5].c_str());

    QString str;
    str = QString::number(X);
    ui->Edit_X->setText(str);
    str = QString::number(Y);
    ui->Edit_Y->setText(str);
    str = QString::number(Z);
    ui->Edit_Z->setText(str);
    str = QString::number(U);
    ui->Edit_U->setText(str);
    str = QString::number(V);
    ui->Edit_V->setText(str);
    str = QString::number(W);
    ui->Edit_W->setText(str);

//	Program_Run_Joint_Move(X, Y, Z, U, V, W);
}


void dialogprogram_framemove::on_pushButton_Add_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();

    double ret[6];
    QString str;
    str = ui->Edit_X->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_Y->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_Z->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_U->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_V->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_W->toPlainText();
    ret[5] = str.toDouble();

    QString joint_point;
    joint_point.sprintf("Frame|%f,%f,%f,%f,%f,%f",ret[0],ret[1],ret[2],ret[3],ret[4],ret[5]);

    //QString joint_point = "Frame|10,10,10,10,10,10";
    //QString joint_point = "Frame|10,10,10,10,10,10";

    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();

    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,joint_point);
    //myParent->on_TreeWidget_Put_Item(new_child);

    QTreeWidgetItem* parentItem = SelectedItem[0]->parent();




    if( SelectedItem[0]->text(0) == "MoveF")
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

void dialogprogram_framemove::on_pushButton_Get_clicked()
{
    double ret[6];
    m_IndyDCP->getJointPosition(ret);

    QString str;
    str = QString::number(ret[0]);
    ui->Edit_X->setText(str);
    str = QString::number(ret[1]);
    ui->Edit_Y->setText(str);
    str = QString::number(ret[2]);
    ui->Edit_Z->setText(str);
    str = QString::number(ret[3]);
    ui->Edit_U->setText(str);
    str = QString::number(ret[4]);
    ui->Edit_V->setText(str);
    str = QString::number(ret[5]);
    ui->Edit_W->setText(str);
}

void dialogprogram_framemove::on_pushButton_Set_clicked()
{
    double ret[6];
    QString str;
    str = ui->Edit_X->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_Y->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_Z->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_U->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_V->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_W->toPlainText();
    ret[5] = str.toDouble();

    m_IndyDCP->moveJointTo(ret);
}

void dialogprogram_framemove::on_pushButton_Relative_Init_clicked()
{
    QString str;
    str = QString::number(0);
    ui->Edit_X->setText(str);
    str = QString::number(0);
    ui->Edit_Y->setText(str);
    str = QString::number(0);
    ui->Edit_Z->setText(str);
    str = QString::number(0);
    ui->Edit_U->setText(str);
    str = QString::number(0);
    ui->Edit_V->setText(str);
    str = QString::number(0);
    ui->Edit_W->setText(str);
}

void dialogprogram_framemove::on_pushButton_Relative_Set_clicked()
{
    double ret[6];
    QString str;
    str = ui->Edit_X->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_Y->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_Z->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_U->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_V->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_W->toPlainText();
    ret[5] = str.toDouble();

    m_IndyDCP->moveJointBy(ret);
}

void dialogprogram_framemove::on_pushButton_Relative_Add_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();

    double ret[6];
    QString str;
    str = ui->Edit_X->toPlainText();
    ret[0] = str.toDouble();
    str = ui->Edit_Y->toPlainText();
    ret[1] = str.toDouble();
    str = ui->Edit_Z->toPlainText();
    ret[2] = str.toDouble();
    str = ui->Edit_U->toPlainText();
    ret[3] = str.toDouble();
    str = ui->Edit_V->toPlainText();
    ret[4] = str.toDouble();
    str = ui->Edit_W->toPlainText();
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




    if( SelectedItem[0]->text(0) == "MoveF")
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
