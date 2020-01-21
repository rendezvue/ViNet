#include "dialogprogram.h"
#include "ui_dialogprogram.h"
#include "dialogprogram_jointmove.h"
#include <QMessageBox>

#define ROBOT_CMD_JOINTMOVE		"JOINTMOVE"
#define ROBOT_CMD_JOINTMOVE_REL "JOINTMOVE_REL"
#define ROBOT_CMD_WAYPOINT 		"WAYPOINT"
#define ROBOT_CMD_DETECTION  	"DETECTION"
#define ROBOT_CMD_RELATIVE  	"RELATIVE"
#define	ROBOT_CMD_GRIPPER		"GRIPPER"
#define	ROBOT_CMD_LOOP_START	"LOOP_START"
#define	ROBOT_CMD_LOOP_OUT		"LOOP_OUT"
#define	ROBOT_CMD_LOOP_END		"LOOP_END"
#define	ROBOT_CMD_IF_START		"IF_START"
#define	ROBOT_CMD_IF_END		"IF_END"
#define	ROBOT_CMD_DETECTMOVE	"DETECTMOVE"
#define	ROBOT_CMD_WAITING		"WAITING"
#define	ROBOT_CMD_VISION_IPM	"VISION_IPM"
#define	ROBOT_CMD_ASSIGNMENT	"ASSIGNMENT"
#define	ROBOT_CMD_ANGLEMOVE		"ANGLEMOVE"
#define	ROBOT_CMD_BOUNDARY_LEVEL"BOUNDARY_LEVEL"

DialogProgram::DialogProgram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgram)
{
    m_IndyDCP_connector = NULL;
    ui->setupUi(this);
    ui->lineEdit_ip_address->setInputMask("000.000.000.000;_");
    std::string str_ip_address = "192.168.212.19" ;
    ui->lineEdit_ip_address->setText(QString::fromUtf8(str_ip_address.c_str()));

    Init_ToolBox();
}

DialogProgram::~DialogProgram()
{
    delete ui;
}

void DialogProgram::Init_ToolBox()
{
    m_Widget_ToolBox = ui->listWidget;
    m_Widget_ToolBox->addItem(ROBOT_CMD_JOINTMOVE		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_JOINTMOVE_REL   );
    m_Widget_ToolBox->addItem(ROBOT_CMD_WAYPOINT 		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_DETECTION       );
    m_Widget_ToolBox->addItem(ROBOT_CMD_RELATIVE        );
    m_Widget_ToolBox->addItem(ROBOT_CMD_GRIPPER         );
    m_Widget_ToolBox->addItem(ROBOT_CMD_LOOP_START      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_LOOP_OUT		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_LOOP_END		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_IF_START		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_IF_END          );
    m_Widget_ToolBox->addItem(ROBOT_CMD_DETECTMOVE      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_WAITING         );
    m_Widget_ToolBox->addItem(ROBOT_CMD_VISION_IPM      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_ASSIGNMENT      );
    m_Widget_ToolBox->addItem(ROBOT_CMD_ANGLEMOVE		);
    m_Widget_ToolBox->addItem(ROBOT_CMD_BOUNDARY_LEVEL  );

    int count = m_Widget_ToolBox->count();

    for( int i = 0 ;i < count ; i++ )
    {
        QListWidgetItem *item = m_Widget_ToolBox->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(),30));
    }

}

void DialogProgram::on_pushButton_Connect_Indy7_clicked()
{
    QString qstr_ip_address = ui->lineEdit_ip_address->text() ;
    bool connection;
    if( m_IndyDCP_connector == NULL )
    {
        m_IndyDCP_connector = new IndyDCPConnector(qstr_ip_address.toStdString(),ROBOT_INDY7);
        connection = m_IndyDCP_connector->connect();
        qDebug("Connection = %d\n",connection);
    }
    if( connection == true )
    {
        QMessageBox::information(this,tr("INDY 7 Conenect OK"),tr("This is a ..."));
    }
    else
    {
        QMessageBox::information(this,tr("INDY 7 Conenect Fail"),tr("This is a ..."));
    }
    double ret[6];
    m_IndyDCP_connector->getTaskPosition(ret);
    qDebug("ret[0]=%f\n",ret[0]);
}

void DialogProgram::on_pushButton_clicked()
{
    dialogprogram_jointmove *dlg_jointmove = new dialogprogram_jointmove;
    QMdiSubWindow *window = ui->mdiArea->addSubWindow(dlg_jointmove,Qt::FramelessWindowHint);

//    dlg_jointmove->setWindowFlag(Qt::Dialog | Qt::FramelessWindowHint);
    dlg_jointmove->showMaximized();
    //ui->frame->setWindowFlag(Qt::Widget);
}
