#include "formcandidatetool.h"
#include "ui_formcandidatetool.h"

FormCandidateTool::FormCandidateTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCandidateTool)
{
    ui->setupUi(this);
}

FormCandidateTool::~FormCandidateTool()
{
    delete ui;
}

void FormCandidateTool::SetBackground(QPixmap &image)
{
    //qDebug("%s", __func__) ;

#if 0
    image = image.scaled(this->size(), Qt::IgnoreAspectRatio); //set scale of pic to match the window

    QPalette palette;
    palette.setBrush((this)->backgroundRole(), QBrush(image));
    (this)->setPalette(palette);
#endif

    ui->label_bg_image->setPixmap(image);
}

void FormCandidateTool::SetText(const std::string text)
{
    ui->label->setText(QString::fromUtf8(text.c_str()));
}
