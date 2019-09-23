#include "cmat2qimage.h"

CMat2QImage::CMat2QImage()
{

}

QImage CMat2QImage::cvtMat2QImage(cv::Mat& image, const int width, const int height)
{
	const int label_width = width ;
	const int label_height = height ;

	cv::Mat object_image = cv::Mat::zeros(cv::Size(label_width, label_height), CV_8UC3) ; 

    if( !image.empty() )
    {
    	float scale_w = (float)object_image.cols / (float)image.cols ;
		float scale_h = (float)object_image.rows / (float)image.rows ;
		float scale = std::min(scale_w, scale_h) ;

		if( scale < 1.0 )
		{
			cv::resize(image, image, cv::Size(), scale, scale) ;
		}

		
		cv::Rect rect_copy ;
		rect_copy.x = (object_image.cols - image.cols)/2 ;
		rect_copy.y = (object_image.rows - image.rows)/2 ;
		rect_copy.width = image.cols ;
		rect_copy.height = image.rows ;

        image.copyTo(object_image(rect_copy)) ;
    }
	
	QImage qt_display_image((const uchar *) object_image.data, object_image.cols, object_image.rows, object_image.step, QImage::Format_RGB888);
	qt_display_image.bits(); // enforce deep copy, see documentation 

	if( object_image.cols != label_width || object_image.rows != label_height )
	{
        qt_display_image = qt_display_image.scaled(label_width, label_height,Qt::KeepAspectRatio);
	}

	return qt_display_image ;
}

