#include "cgetimagethread.h"

void CGetImageThread::SetResultId(const std::string id)
{
    m_str_id = id ;
}

void CGetImageThread::SetSourceType(const int type)
{
    m_i_type = type ;
}

void CGetImageThread::run(void)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");

    while(m_thread_run)
    {
        if( m_mat_input_image.empty() )
        {
            m_mat_input_image = cv::Mat::zeros(DISPLAY_IMAGE_HEIGHT, DISPLAY_IMAGE_WIDTH, CV_8UC3) ;
        }

        if( CEnsemble::getInstance()->m_cls_api.Ensemble_Network_IsOnline()  )
        {
            //unsigned char* get_data = NULL ;
            //int width = DISPLAY_IMAGE_WIDTH ;
            //int height = DISPLAY_IMAGE_HEIGHT ;
            ImageBuf image_buf ;
            image_buf.image_width = DISPLAY_IMAGE_WIDTH ;
            image_buf.image_height = DISPLAY_IMAGE_HEIGHT ;

            //unsigned char* get_data_result = NULL ;
            //int width_result = DISPLAY_IMAGE_WIDTH ;
            //int height_result = DISPLAY_IMAGE_HEIGHT ;
            ImageBuf result_buf ;
            result_buf.image_width = DISPLAY_IMAGE_WIDTH ;
            result_buf.image_height = DISPLAY_IMAGE_HEIGHT ;

            const int image_type = IMAGE_RGB888 ;
            //int get_result_image_type = 0 ;
            //int get_source_image_type = 0 ;
			
            int ret = 0 ;

            if( m_i_type == 1 )        //result
            {
                //ret = CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_ResultImage(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;
                ret += CEnsemble::getInstance()->m_cls_api.Ensemble_Result_Get_Image(m_str_id, image_type, &result_buf) ;

                //qDebug("Result Image Size = %d, %d", width_result, height_result) ;
            }
            else if( m_i_type == 2 )        //merge image = image + result
            {
                ret = CEnsemble::getInstance()->m_cls_api.Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &image_buf) ;
                //ret += CEnsemble::getInstance()->m_cls_api.Ensemble_Job_Get_ResultImage(m_str_id, image_type, &get_data_result, &width_result, &height_result) ;
                ret += CEnsemble::getInstance()->m_cls_api.Ensemble_Result_Get_Image(m_str_id, image_type, &result_buf) ;
            }
            else
            {
                ret = CEnsemble::getInstance()->m_cls_api.Ensemble_Source_Get_Image(GET_IMAGE_INPUT, std::string(), image_type+IMAGE_ADD_TIME+IMAGE_ADD_SOURCE_INFO, &image_buf) ;
            }

            //qDebug("Network return = %d", ret) ;
            //qDebug("Get Image Size = %d x %d", width, height) ;
            //Check Get Data(Input Image)
            if( image_buf.p_buf != NULL )
            {
                if( image_buf.image_width>0 && image_buf.image_height >0 )
                {
                    if( image_buf.image_type == IMAGE_YUV420 )
                    {
                        cv::Mat get_image(image_buf.image_height + image_buf.image_height / 2, image_buf.image_width, CV_8UC1, image_buf.p_buf) ;

                        CImgDec cls_image_decoder ;
                        m_mat_input_image = cls_image_decoder.Decoding(get_image) ;
                    }
                    else if( image_buf.image_type == IMAGE_RGB888 )
                    {
                        cv::Mat get_image(image_buf.image_height, image_buf.image_width, CV_8UC3, image_buf.p_buf) ;
                        cv::cvtColor(get_image, m_mat_input_image, cv::COLOR_BGR2RGB) ;
                    }
                     else if( image_buf.image_type == IMAGE_JPG )
                    {
                        cv::Mat get_image = cv::imdecode(cv::Mat(1,  image_buf.image_width*image_buf.image_height, CV_8UC1, image_buf.p_buf), cv::IMREAD_UNCHANGED);
                        cv::cvtColor(get_image, m_mat_input_image, cv::COLOR_BGR2RGB) ;
                    }
                }

                delete [] image_buf.p_buf ;
                image_buf.p_buf = NULL ;
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
            if( result_buf.p_buf )
            {

                if( result_buf.image_width>0 && result_buf.image_height >0 )
                {
                    if( result_buf.image_type == IMAGE_YUV420 )
                    {
                        cv::Mat get_image(result_buf.image_height + result_buf.image_height / 2, result_buf.image_width, CV_8UC1, result_buf.p_buf) ;

                        CImgDec cls_image_decoder ;
                        m_mat_result_image = cls_image_decoder.Decoding(get_image) ;
                    }
                    else if( result_buf.image_type == IMAGE_RGB888 )
                    {
                        cv::Mat get_image(result_buf.image_height, result_buf.image_width, CV_8UC3, result_buf.p_buf) ;
                        cv::cvtColor(get_image, m_mat_result_image, cv::COLOR_BGR2RGB) ;
                    }
                    else if( result_buf.image_type == IMAGE_JPG )
                    {
                        cv::Mat get_image = cv::imdecode(cv::Mat(1,  result_buf.image_width*result_buf.image_height, CV_8UC1, result_buf.p_buf), cv::IMREAD_UNCHANGED);
                        cv::cvtColor(get_image, m_mat_result_image, cv::COLOR_BGR2RGB) ;
                    }
                }

                delete [] result_buf.p_buf ;
                result_buf.p_buf = NULL ;
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
