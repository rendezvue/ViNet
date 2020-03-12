#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QTreeWidget>
#include <QThread>
#include <QListWidget>
#include <QMetaType>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QEvent>

#include "opencv2/opencv.hpp"

//Dialog
#include "cdialogconnect.h"
#include "cdialognewproject.h"
#include "ui/dialogcheckforupdates.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

//
#include "ImgDec.h"
#include "pugixml.hpp"
#include "formtoollist.h"
#include "formdeviceinfo.h"
#include "formjobbase.h"
#include "formjobtool.h"
#include "formtooloption.h"
#include "formproject.h"

#include "cmat2qimage.h"


#include "csearchtreeitem.h"

#include "cgetimagethread.h"

#include "censemble.h"
//boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Ui {
class MainWindow;
}

class CCheckNetwork : public QThread
{
    Q_OBJECT

public:
    explicit CCheckNetwork(QObject *parent = 0):
            QThread(parent)
    {
    }
    ~CCheckNetwork() {}
public:

    void SetIPAddress(std::string str_ip)
    {
        m_str_ip_address = str_ip;
    }

	void SetPort(const int port)
	{
		m_i_port = port ;
	}

protected:
    void run()
    {
        int retry_count = 0 ;

        while(1)
        {
#if 0        
            if( !m_str_ip_address.empty() )
            {
                if( !CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Network_IsOnline() )
                {
                    qDebug("(%d) Try Re-Connect = %s", retry_count++, m_str_ip_address.c_str()) ;
                    //try re-connect
                    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Network_Disconnect() ;
                    CEnsemble::getInstance()->GetSelectDevice()->Ensemble_Network_Connect(m_str_ip_address.c_str(), m_i_port) ;
                }
                else
                {
                    retry_count = 0 ;
                }
            }
#endif
            QThread::yieldCurrentThread() ;
            QThread::sleep(1) ;     //1sec
        }
    }

private:
    std::string m_str_ip_address ;
	int m_i_port ;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //Timer
    QTimer *m_p_timer;

    CGetImageThread* m_p_cls_getimage ;
    CCheckNetwork* m_p_cls_check_network ;

	//tree
	void AddTreeRoot(QString name, QString description);
    void AddTreeChild(QTreeWidgetItem *parent,
                      QString name, QString description);

	void UpdateToolsListFromDevice(QListWidget *listWidget) ;
    void UpdateJobsListFromDevice(QListWidget *listWidget) ;

    QStringListModel *m_source_list_model ;

    std::string m_str_ip_address ;
	int m_i_port ;

	std::string m_str_select_ip_address ;
	int m_i_select_port ;

protected :
    void showEvent(QShowEvent *ev) override;

//signals:
//    void UpdateFormInfo(void);

public slots:
    void updatePicture(cv::Mat image);
    void UpdateToolsList(void) ;
    void UpdateJobsList(void) ;
    void UpdateJobTree(void) ;
	void UpdateResultImage(QString id) ;
	void UpdateResult(QString qstr_xml) ;
    void OnMenuConnect(void);
    void OnButtonNewProject(void) ;
    void DropEventDoneOnTree(void) ;
    void OnSourceListDClick(const QModelIndex &index) ;
	void OnTreeViewDClick(const QModelIndex& index)  ;
	void OnTreeViewClick(const QModelIndex& index)  ;
    void tabSelected(void) ;
	void OnButtonSaveAllTask(void) ;
	void OnButtonLoadAllTask(void) ;
	void OnButtonUpdateSourceList(void) ;
    void OnMenuCheckforUpdates(void);
};

#endif // MAINWINDOW_H
