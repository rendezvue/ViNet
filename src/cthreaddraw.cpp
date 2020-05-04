#include "cthreaddraw.h"

CThreadDraw::CThreadDraw(QObject *parent) :
    QThread(parent)
{
}

CThreadDraw::~CThreadDraw(void)
{
}

void CThreadDraw::run()
{
    cv::VideoCapture cap(0); // open the default camera

     //qDebug("%s\n", __func__)  ;

    if( !cap.isOpened() )
    {
        //qDebug("Fail : Camera Open") ;
    }

    int count = 0 ;
    forever
    {
        if (QThread::currentThread()->isInterruptionRequested())
        {
            break;
        }

        //qDebug("Count = %d", count++)  ;

        if( cap.isOpened() )
        {
            cv::Mat image;

            cap >> image ;

            if( !image.empty() )
            {
                //cv::imshow("test", image) ;
                //QTest::qSleep(30);
                //QThread::msleep(30);

                emit SendImage(image);
            }
        }
    };

    cap.release() ;
}
