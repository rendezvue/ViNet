#include "formtoollist.h"
#include "ui_formtoollist.h"

FormToolList::FormToolList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormToolList)
{
    ui->setupUi(this);

    ui->frame_border->setStyleSheet(".QFrame{border: 1px solid black; border-radius: 3px;}");
}

FormToolList::~FormToolList()
{
    //qDebug("%s delete", __func__) ;

    delete ui;
}

void FormToolList::SetName(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormToolList::GetName(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormToolList::SetTypeNumber(const int typenumber)
{
    ui->label_type_number->setText(QString::number(typenumber));

    int image_width = ui->label_icon->width() ;
    int image_height = ui->label_icon->height() ;

    cv::Mat icon_image = cv::Mat::zeros(cv::Size(image_width, image_height), CV_8UC3) ;
    cv::Mat read_image ;

    if( typenumber == ToolTypeList::TOOL_TYPE_DETECT_OBJECT )       //object detect
    {
         read_image = cv::imread("./image/tool_detect_icon.png") ;
    }
    else if( typenumber == ToolTypeList::TOOL_TYPE_DETECT_LINE )       //line detect
    {
        read_image = cv::imread("./image/tool_detect_line_icon.png") ;
    }

	//margin check
	if( (typenumber - JOB_TYPE_TOOL) % 100 )
	{
		//option
		//setContentsMargins(0, 0, 0, 0);
		ui->frame_border->setStyleSheet(".QFrame{top: 0px;}");
		//ui->QWidget->setStyleSheet(".QWidget{margin-top: 0px}");
	}

    if( !read_image.empty() )
    {
        //read_image.copyTo(icon_image) ;
        cv::cvtColor(read_image, icon_image, cv::COLOR_BGR2RGB) ;
    }

    if( icon_image.cols != image_width || icon_image.rows != image_height )
    {
        cv::resize(icon_image, icon_image, cv::Size(image_width, image_height)) ;
    }

    QImage qt_display_image = QImage((const unsigned char*)icon_image.data, icon_image.cols, icon_image.rows, QImage::Format_RGB888);

    ui->label_icon->setPixmap(QPixmap::fromImage(qt_display_image));
}

int FormToolList::GetTypeNumber(void)
{
    QString typenumber = ui->label_type_number->text() ;

    return typenumber.toInt() ;
}
