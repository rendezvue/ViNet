#include "cdialogconnect.h"
#include "ui_cdialogconnect.h"
#include "IndyDCPConnector.h"

CDialogConnect::CDialogConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialogConnect) ,
    m_str_ip_address(""),
    m_i_port(4000)
{
    ui->setupUi(this);

    ui->lineEdit_ip_address->setInputMask("000.000.000.000;_");

    //button
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ConnectOK()));

	//initial data
	try
	{
		std::string str_ip_address = "192.168.88.200" ;
		int port = 4000 ;
		
	    if( boost::filesystem::exists(".run.ini") )
	    {
			//last environment from ini
			boost::property_tree::ptree pt;
			boost::property_tree::ini_parser::read_ini(".run.ini", pt);
			str_ip_address = pt.get<std::string>("network.ip") ;
			port = pt.get<int>("network.port") ;
	    }

        ui->lineEdit_ip_address->setText(QString::fromUtf8(str_ip_address.c_str()));
		ui->lineEdit_port->setText(QString::number(port));

        qDebug("ip info from ini : ip=%s", str_ip_address.c_str()) ;
	}
    catch (const std::exception& ex)
	{
        qDebug("%s", ex.what()) ;
	}
}

CDialogConnect::~CDialogConnect()
{
    delete ui;
}

void CDialogConnect::ConnectOK(void)
{
    QString qstr_ip_address = ui->lineEdit_ip_address->text() ;
	int port = ui->lineEdit_port->text().toInt() ;
	
    SetIpAddress(qstr_ip_address.toStdString());
	SetPortNumber(port) ;
}

void CDialogConnect::SetIpAddress(const std::string str_ip)
{
    m_str_ip_address = str_ip ;
}

std::string CDialogConnect::GetIpAddress(void)
{
    return m_str_ip_address ;
}

void CDialogConnect::SetPortNumber(const int port)
{
	m_i_port = port ;
}

int CDialogConnect::GetPortNumber(void)
{
	return m_i_port ;
}

