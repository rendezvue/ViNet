#ifndef DIALOGRESULTINFO_H
#define DIALOGRESULTINFO_H

#include <QDialog>

namespace Ui {
class DialogResultInfo;
}

class DialogResultInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResultInfo(QWidget *parent = 0);
    ~DialogResultInfo();

	void SetIdInfo(const std::string id) ;
	void SetNameInfo(const std::string name) ;
	void SetResultString(const std::string result) ;

private:
    Ui::DialogResultInfo *ui;
	std::string m_str_result ;

protected:
	void showEvent(QShowEvent *ev);

};

#endif // DIALOGRESULTINFO_H
