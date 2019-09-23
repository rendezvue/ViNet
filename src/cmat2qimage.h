#ifndef CMAT2QIMAGE_H
#define CMAT2QIMAGE_H

#include <QWidget>
#include <QDebug>

//opencv
#include "opencv2/opencv.hpp"

class CMat2QImage
{
public:
    CMat2QImage();

	QImage cvtMat2QImage(cv::Mat& image, const int width, const int height) ;
};

#endif // CMAT2QIMAGE_H