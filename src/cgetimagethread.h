#ifndef CGETIMAGETHREAD_H
#define CGETIMAGETHREAD_H

#include "opencv2/opencv.hpp"
#include <QThread>

//API
#include "EnsembleAPI.h"
#include "ImgDec.h"

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
          , m_thread_run(true)
    {
    }
    ~CGetImageThread() {}
public:
    void SetResultId(const std::string id) ;
    void SetSourceType(const int type) ;

	bool m_thread_run ;
	
protected:
    void run(void) ;

private:
    cv::Mat m_mat_input_image ;
    cv::Mat m_mat_result_image ;
    int m_i_type ;
    std::string m_str_id ;

signals:
    void Done(cv::Mat image);
};

#endif // CGETIMAGETHREAD_H
