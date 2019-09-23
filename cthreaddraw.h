#ifndef CTHREADDRAW_H
#define CTHREADDRAW_H

#include <QThread>

#include "opencv2/opencv.hpp"

class CThreadDraw : public QThread
{
    Q_OBJECT
        void run() override;
public:
    explicit CThreadDraw(QObject *parent = 0);
    ~CThreadDraw(void) ;

private:
    //void run();

public slots:

signals:
    void SendImage(cv::Mat image);
};

#endif // CTHREADDRAW_H
