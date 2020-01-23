#ifndef DIALOGPROGRAM_JOINTMOVE_H
#define DIALOGPROGRAM_JOINTMOVE_H

#include "IndyDCPConnector.h"
#include <QDialog>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>

using namespace NRMKIndy::Service::DCP;

namespace Ui {
class dialogprogram_jointmove;
}

class dialogprogram_jointmove : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_jointmove(IndyDCPConnector* IndyDCP, QTreeWidget* TreeWidget, QWidget *parent = 0);
    ~dialogprogram_jointmove();

    void ParseString();
private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Get_clicked();

    void on_pushButton_Set_clicked();

    void on_pushButton_Relative_Init_clicked();

    void on_pushButton_Relative_Set_clicked();

    void on_pushButton_Relative_Add_clicked();

private:
    QTreeWidget* m_TreeWidget;
    IndyDCPConnector* m_IndyDCP;
    Ui::dialogprogram_jointmove *ui;

	void Setup_UI_Joint_Move(std::string command, std::string desc);

};

#endif // DIALOGPROGRAM_JOINTMOVE_H
