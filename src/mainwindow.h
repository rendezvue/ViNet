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

            if( Ensemble_Network_IsOnline() & ENSEMBLE_CONNECT_CONTROL_PORT )
            {
                unsigned char* get_data = NULL ;
                int width = DISPLAY_IMAGE_WIDTH ;
                int height = DISPLAY_IMAGE_HEIGHT ;

				unsigned char* get_data_result = NULL ;
                int width_result = DISPLAY_IMAGE_WIDTH ;
                int height_result = DISPLAY_IMAGE_HEIGHT ;

				const int image_type = IMAGE_RGB888 ;
				
                if( m_i_type == 1 )        //result
                {
                    Ensemble_Job_Get_ResultImage(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;
                }
                else if( m_i_type == 2 )        //merge image = image + result
                {
                    Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &get_data, &width, &height) ;
                    Ensemble_Job_Get_ResultImage(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;                    
                }
                else
                {
                    Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &get_data, &width, &height) ;
                }


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
							cv::Mat get_image(height, width, CV_8UC3, get_data_result) ;
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
				
				//merge
				if( m_i_type == 1 )        //result
                {
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

	//tree
	void AddTreeRoot(QString name, QString description);
    void AddTreeChild(QTreeWidgetItem *parent,
                      QString name, QString description);

	void UpdateToolsListFromDevice(QListWidget *listWidget) ;
    void UpdateJobsListFromDevice(QListWidget *listWidget) ;

    QStringListModel *m_source_list_model ;

public slots:
    void updatePicture(cv::Mat image);
    void UpdateToolsList(void) ;
    void UpdateJobsList(void) ;
    void UpdateJobTree(void) ;
	void UpdateResultImage(QString id) ;
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
