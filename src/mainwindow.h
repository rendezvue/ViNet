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
#include "dialogcheckforupdates.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

//
#include "ImgDec.h"
#include "pugixml.hpp"
#include "formdeviceinfo.h"
#include "formjobbase.h"
#include "formjobbasecode.h"
#include "formjobtool.h"
#include "formtooloption.h"
#include "formproject.h"
#include "formtooloptionwithprogress.h"

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
        while(1)
        {
        	int size = CEnsemble::getInstance()->GetSize() ;
			for( int i=0 ; i<size ; i++ )
			{	
				CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(i) ;
			
				if( p_device )
	            {
	                if( !p_device->Ensemble_Network_IsOnline() )
	                {
	                	std::string str_ip ;
						int port ;
	                	p_device->Ensemble_Network_GetInfo(&str_ip, &port);
						
	                    //qDebug("Try Re-Connect = %s:%d", str_ip.c_str(), port) ;
	                    //try re-connect
	                    p_device->Ensemble_Network_Disconnect() ;
	                    p_device->Ensemble_Network_Connect(str_ip.c_str(), port) ;
	                }
	            }
			}
			
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
    void UpdateJobTree(void) ;
	void UpdateResultImage(QString id) ;
	void UpdateResult(QString qstr_xml) ;
    void OnMenuConnect(void);
    void DropEventDoneOnTree(void) ;
    void OnSourceListDClick(const QModelIndex &index) ;
	void OnTreeViewDClick(const QModelIndex& index)  ;
	void OnTreeViewClick(const QModelIndex& index)  ;
    void tabSelected(void) ;
	void OnButtonUpdateSourceList(void) ;
};

#endif // MAINWINDOW_H
