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

#include <QEvent>

#include "opencv2/opencv.hpp"

//Dialog
#include "cdialogconnect.h"
#include "cdialognewproject.h"

//API
#include "EnsembleAPI.h"

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



//boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace Ui {
class MainWindow;
}

#define DISPLAY_IMAGE_WIDTH		640
#define DISPLAY_IMAGE_HEIGHT	480

#define DISPLAY_USING_YUV420	1

class CGetImageThread : public QThread
{
    Q_OBJECT

public:
    explicit CGetImageThread(QObject *parent = 0):
            QThread(parent)
          , m_i_type(0)
    {
    }
    ~CGetImageThread() {}
public:
	void SetResultId(const std::string id)
	{
		m_str_id = id ;
	}
	
    void SetSourceType(const int type)
    {
        m_i_type = type ;
    }

protected:
    void run()
    {
        qRegisterMetaType<cv::Mat>("cv::Mat");

        while(1)
        {
            if( m_mat_input_image.empty() )
            {
                m_mat_input_image = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
            }

            if( Ensemble_Network_IsOnline()  )
            {
                unsigned char* get_data = NULL ;
                int width = DISPLAY_IMAGE_WIDTH ;
                int height = DISPLAY_IMAGE_HEIGHT ;

				unsigned char* get_data_result = NULL ;
                int width_result = DISPLAY_IMAGE_WIDTH ;
                int height_result = DISPLAY_IMAGE_HEIGHT ;

				const int image_type = IMAGE_RGB888 ;
				
                int ret = 0 ;

                if( m_i_type == 1 )        //result
                {
                    //ret = Ensemble_Job_Get_ResultImage(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;
                    ret += Ensemble_Result_Get_Image(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;

                    //qDebug("Result Image Size = %d, %d", width_result, height_result) ;
                }
                else if( m_i_type == 2 )        //merge image = image + result
                {
                    ret = Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &get_data, &width, &height) ;
                    //ret += Ensemble_Job_Get_ResultImage(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;
                    ret += Ensemble_Result_Get_Image(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;
                }
                else
                {
                    ret = Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &get_data, &width, &height) ;
                }

                //qDebug("Network return = %d", ret) ;
                //qDebug("Get Image Size = %d x %d", width, height) ;
				//Check Get Data(Input Image)
                if( get_data != NULL )
                {
                    if( width>0 && height >0 )
                    {
                    	if( image_type == IMAGE_YUV420 )
                    	{
	                        cv::Mat get_image(height + height / 2, width, CV_8UC1, get_data) ;

	                        CImgDec cls_image_decoder ;
	                        m_mat_input_image = cls_image_decoder.Decoding(get_image) ;
                    	}
						else if( image_type == IMAGE_RGB888 )
						{
							cv::Mat get_image(height, width, CV_8UC3, get_data) ;
							cv::cvtColor(get_image, m_mat_input_image, cv::COLOR_BGR2RGB) ;
						}
                    }

                    delete [] get_data ;
                    get_data = NULL ;
                }
				else
				{
	                if( m_mat_input_image.empty() )
	                {
	                    m_mat_input_image = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
	                }
	                else
	                {
	                    m_mat_input_image = cv::Scalar(0) ;
	                }
				}

				//qDebug("Get Result Image") ;
				//Check Result Data
				if( get_data_result )
				{
					
                    if( width_result>0 && height_result >0 )
                    {
                    	if( image_type == IMAGE_YUV420 )
                    	{
	                        cv::Mat get_image(height_result + height_result / 2, width_result, CV_8UC1, get_data_result) ;

	                        CImgDec cls_image_decoder ;
	                        m_mat_result_image = cls_image_decoder.Decoding(get_image) ;
                    	}
						else if( image_type == IMAGE_RGB888 )
						{
                            cv::Mat get_image(height_result, width_result, CV_8UC3, get_data_result) ;
							cv::cvtColor(get_image, m_mat_result_image, cv::COLOR_BGR2RGB) ;
						}
                    }

                    delete [] get_data_result ;
                    get_data_result = NULL ;
				}
				else
				{
					if( m_mat_result_image.empty() )
	                {
	                    m_mat_result_image = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
	                }
	                else
	                {
	                    m_mat_result_image = cv::Scalar(0) ;
	                }
				}

				//qDebug("Get Image Mege") ;
				
				if( m_i_type == 1 )        //result
                {
                    //cv::imshow("result", m_mat_result_image) ;
                    //cv::waitKey(1) ;
                    //qDebug("input result image size = %d, %d", m_mat_result_image.cols, m_mat_result_image.rows) ;

                	m_mat_result_image.copyTo(m_mat_input_image );
				}
				else if( m_i_type == 2 )		//merge
				{
					cv::Rect resize_rect ;
					resize_rect.width = ((float)m_mat_input_image.cols/3.0) ;
					resize_rect.height = ((float)m_mat_input_image.rows/3.0) ;
					resize_rect.x = m_mat_result_image.cols - resize_rect.width - 10 ;
					resize_rect.y = m_mat_result_image.rows - resize_rect.height - 10 ;

					cv::Mat resize_input_image ;					
					cv::resize(m_mat_input_image, resize_input_image, cv::Size(resize_rect.width, resize_rect.height)) ;

					//merge
					resize_input_image.copyTo(m_mat_result_image(resize_rect)) ;

					cv::rectangle(m_mat_result_image, resize_rect, cv::Scalar(0,0,0), 1) ;

					m_mat_result_image.copyTo(m_mat_input_image );
				}

            }

            emit Done(m_mat_input_image);

            QThread::yieldCurrentThread() ;
            QThread::usleep(33000) ;
        }
    }

private:
    cv::Mat m_mat_input_image ;
	cv::Mat m_mat_result_image ;
    int m_i_type ;
	std::string m_str_id ;

signals:
    void Done(cv::Mat image);
};

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

protected:
    void run()
    {
        int retry_count = 0 ;

        while(1)
        {
            if( !m_str_ip_address.empty() )
            {
                if( !Ensemble_Network_IsOnline() )
                {
                    qDebug("(%d) Try Re-Connect = %s", retry_count++, m_str_ip_address.c_str()) ;
                    //try re-connect
                    Ensemble_Network_Disconnect() ;
                    Ensemble_Network_Connect(m_str_ip_address.c_str()) ;
                }
                else
                {
                    retry_count = 0 ;
                }
            }

            QThread::yieldCurrentThread() ;
            QThread::sleep(1) ;     //1sec
        }
    }

private:
    std::string m_str_ip_address ;

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

protected :
    void showEvent(QShowEvent *ev) override;

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
    void tabSelected(void) ;
	void OnButtonSaveAllTask(void) ;
	void OnButtonLoadAllTask(void) ;
};

#endif // MAINWINDOW_H
