#ifndef CDIALOGCONNECT_H
#define CDIALOGCONNECT_H

#include <QDialog>

//boost
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/iostreams/categories.hpp>  // sink_tag
#include <iterator>  // back_inserter


namespace Ui {
class CDialogConnect;
}

class CDialogConnect : public QDialog
{
    Q_OBJECT

public:
    explicit CDialogConnect(QWidget *parent = 0);
    ~CDialogConnect();

    void SetIpAddress(const std::string str_ip) ;
    std::string GetIpAddress(void) ;

	void SetPortNumber(const int port) ;
	int GetPortNumber(void) ;

private:
    Ui::CDialogConnect *ui;

    std::string m_str_ip_address ;
	int m_i_port ;

private slots :
    void ConnectOK(void);

};

#endif // CDIALOGCONNECT_H
