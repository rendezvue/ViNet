#ifndef DIALOGPROGRAM_LOOP_H
#define DIALOGPROGRAM_LOOP_H

#include <QDialog>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>
#include <vector>

namespace Ui {
class dialogprogram_loop;
}

class dialogprogram_loop : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_loop(QTreeWidget* TreeWidget, QWidget *parent = 0);
    ~dialogprogram_loop();

private slots:
    void on_radioButton_Count_pressed();

    void on_radioButton_Unlimit_pressed();

    void on_pushButton_LoopType_Add_clicked();

private:

    QTreeWidget* m_TreeWidget;
    Ui::dialogprogram_loop *ui;
};

#endif // DIALOGPROGRAM_LOOP_H
