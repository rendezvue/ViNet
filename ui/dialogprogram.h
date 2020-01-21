#ifndef DIALOGPROGRAM_H
#define DIALOGPROGRAM_H

#include <QDialog>
#include "IndyDCPConnector.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

#include <QtWidgets/QListWidget>
using namespace std;
using namespace NRMKIndy::Service::DCP;

namespace Ui {
class DialogProgram;
}

class DialogProgram : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProgram(QWidget *parent = 0);
    ~DialogProgram();
    std::string m_str_ip_address ;
    IndyDCPConnector *m_IndyDCP_connector = NULL;
private slots:
    void on_pushButton_Connect_Indy7_clicked();

    void on_pushButton_clicked();

private:
    QListWidget* m_Widget_ToolBox;
    Ui::DialogProgram *ui;

    void Init_ToolBox();
};

#endif // DIALOGPROGRAM_H
