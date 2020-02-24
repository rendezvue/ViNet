#include "dialogprogram_loop.h"
#include "ui_dialogprogram_loop.h"

dialogprogram_loop::dialogprogram_loop(QTreeWidget* TreeWidget,QWidget *parent) :
    QDialog(parent),m_TreeWidget(TreeWidget),
    ui(new Ui::dialogprogram_loop)
{
    ui->setupUi(this);
}

dialogprogram_loop::~dialogprogram_loop()
{
    delete ui;
}

void dialogprogram_loop::on_radioButton_Count_pressed()
{
    ui->radioButton_Unlimit->released();
}

void dialogprogram_loop::on_radioButton_Unlimit_pressed()
{
    ui->radioButton_Count->released();
}

void dialogprogram_loop::on_pushButton_LoopType_Add_clicked()
{
    //DialogProgram* myParent = (DialogProgram*)this->parent();

    QString count_str = ui->textEdit->toPlainText();

    QString job_str;
    if( ui->radioButton_Count->isChecked() == true )
    {
        job_str.sprintf("Loop|%d", count_str.toInt() );
    }
    else if (ui->radioButton_Unlimit->isChecked() == true)
    {
        job_str.sprintf("Loop|%s","Unlimit"  );
    }


    QList<QTreeWidgetItem*> SelectedItem = m_TreeWidget->selectedItems();

    SelectedItem[0]->setText(0,job_str);

/*
    QTreeWidgetItem* new_child = new QTreeWidgetItem;
    new_child->setText(0,job_str);
    //myParent->on_TreeWidget_Put_Item(new_child);

    QTreeWidgetItem* parentItem = SelectedItem[0]->parent();


    if( SelectedItem[0]->text(0) == "Loop")
    {
        SelectedItem[0]->addChild(new_child);
        SelectedItem[0]->setExpanded(true);
    }
    else
    {
        int Index_selectedItem = parentItem->indexOfChild(SelectedItem[0]);
        parentItem->insertChild(Index_selectedItem+1,new_child);
    }
*/
}
