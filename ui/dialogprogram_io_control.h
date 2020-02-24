#ifndef DIALOGPROGRAM_IO_CONTROL_H
#define DIALOGPROGRAM_IO_CONTROL_H

#include <QDialog>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>
#include <vector>
namespace Ui {
class dialogprogram_io_control;
}

class dialogprogram_io_control : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_io_control(QTreeWidget* TreeWidget, QWidget *parent = 0);
    ~dialogprogram_io_control();

private slots:
    void on_pushButton_Get_Digital_IO_clicked();

    void on_pushButton_Get_JobList_clicked();

    void on_pushButton_Out_Set_clicked();

    void on_pushButton_Out_Add_clicked();

private:
    QTreeWidget* m_TreeWidget;
    Ui::dialogprogram_io_control *ui;
};

#endif // DIALOGPROGRAM_IO_CONTROL_H
