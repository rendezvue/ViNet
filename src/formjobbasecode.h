#ifndef FORMJOBBASECODE_H
#define FORMJOBBASECODE_H

#include <QWidget>
#include <QHoverEvent>
#include <QEvent>
#include <QDebug>

#include "EnsembleCommon.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "cnetworkinfo.h"


namespace Ui {
class FormJobBaseCode;
}

class FormJobBaseCode : public QWidget, public CNetworkInfo
{
    Q_OBJECT

public:
    explicit FormJobBaseCode(QWidget *parent = 0);
    ~FormJobBaseCode();

	 void SetNameInfo(const std::string name) ;
    std::string GetNameInfo(void) ;

    void SetIdInfo(const std::string id) ;
    std::string GetIdInfo(void) ;
	
	void SetResultString(const std::string str_result) ;

	void SetType(const int type) ;
    int GetType(void) ;
	
protected:
	void showEvent(QShowEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool event(QEvent * e);

    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);
	
    int m_type ;
	std::string m_str_result ;
	
signals:
    void UpdateList(void);
	void UpdateResultImage(QString id) ;

public slots:
	void OnButtonDel(void);
    void OnButtonEdit(void) ;
	void OnUpdateName(QString name) ;
	void OnButtonRun(void) ;
	void OnRunCheckBoxToggled(bool checked) ;
	//void UpdateInfo(void) ;

private:
    Ui::FormJobBaseCode *ui;
};

#endif // FORMJOBBASECODE_H
