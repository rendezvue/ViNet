#ifndef DIALOGPROGRAM_FRAMEMOVE_H
#define DIALOGPROGRAM_FRAMEMOVE_H

#include "IndyDCPConnector.h"
#include <QDialog>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>

using namespace NRMKIndy::Service::DCP;

namespace Ui {
class dialogprogram_framemove;
}

class dialogprogram_framemove : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_framemove(IndyDCPConnector* IndyDCP, QTreeWidget* TreeWidget, QWidget *parent = 0);
    ~dialogprogram_framemove();

    void ParseString();
private slots:
    void on_pushButton_Get_clicked();

    void on_pushButton_Set_clicked();

    void on_pushButton_Add_clicked();

    void on_pushButton_Relative_Init_clicked();

    void on_pushButton_Relative_Set_clicked();

    void on_pushButton_Relative_Add_clicked();

private:
    QTreeWidget* m_TreeWidget;
    IndyDCPConnector* m_IndyDCP;
    Ui::dialogprogram_framemove *ui;

    void Setup_UI_Frame_Move(std::string command, std::string desc);
};

#endif // DIALOGPROGRAM_FRAMEMOVE_H
