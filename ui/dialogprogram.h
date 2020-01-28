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
#include <vector>
#include <string>

#include "xbelwriter.h"
#include "xbelreader.h"

#include "dialogprogram_jointmove.h"
#include "dialogprogram_framemove.h"

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
    void on_TreeWidget_Put_Item(QListWidgetItem *item);
private slots:
    void on_pushButton_Connect_Indy7_clicked();
    void on_listWidget_ToolBox_itemClicked(QListWidgetItem *item);



    void on_treeWidget_Program_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_Load_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_RunProgram_clicked();

    void on_pushButton_StopProgram_clicked();

private:
    QListWidget* m_Widget_ToolBox;
    Ui::DialogProgram *ui;

    void Init_ToolBox();

    void Init_ProgramBox();

	void writeIndexEntry(QXmlStreamWriter *xmlWriter, QTreeWidgetItem *item);
    bool writeXml(const QString &fileName, QTreeWidget *treeWidget);


public:
    vector<string> split(string str, char delimiter);

    dialogprogram_jointmove *m_dlg_jointmove;
    dialogprogram_framemove *m_dlg_framemove;
};

#endif // DIALOGPROGRAM_H
