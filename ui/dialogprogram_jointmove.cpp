#include "dialogprogram_jointmove.h"
#include "ui_dialogprogram_jointmove.h"

dialogprogram_jointmove::dialogprogram_jointmove(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogprogram_jointmove)
{
    ui->setupUi(this);
}

dialogprogram_jointmove::~dialogprogram_jointmove()
{
    delete ui;
}
