#ifndef FORMTOOLOPTIONWITHPROGRESS_H
#define FORMTOOLOPTIONWITHPROGRESS_H

//opencv
#include "opencv2/opencv.hpp"

#include <QWidget>
#include <QHoverEvent>
#include <QEvent>

//API
#include "EnsembleAPI.h"
#include "censemble.h"
#include "cnetworkinfo.h"

//dialog
#include "dialogsetcolorcompare.h"

#include <QPropertyAnimation>


namespace Ui {
class FormToolOptionWithProgress;
}

class FormToolOptionWithProgress : public QWidget, public CNetworkInfo
{
    Q_OBJECT

public:
    explicit FormToolOptionWithProgress(QWidget *parent = 0);
    ~FormToolOptionWithProgress();

void SetNameInfo(const std::string name) ;
	std::string GetNameInfo(void) ;

	void SetTypeInfo(const std::string type) ;
	std::string GetTypeInfo(void) ;

	void SetType(const int type) ;
	int GetType(void) ;

	void SetIdInfo(const std::string id) ;
	std::string GetIdInfo(void) ;

	void SetParentIdInfo(const std::string id) ;
	std::string GetParentIdInfo(void) ;

	void SetObjectImage(cv::Mat image) ;
	cv::Mat GetObjectImage(void) ;	

	void UpdateInformationString(QString str_info) ;

	void SetAlarm(const bool b_on_off) ;

	void SetProgress(const float value) ;
	
protected:
	void showEvent(QShowEvent *ev) override;

	void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool event(QEvent * e);
	
	void hoverEnter(QHoverEvent * event);
	void hoverLeave(QHoverEvent * event);
	void hoverMove(QHoverEvent * event);

public slots:
	void OnButtonDel(void);
	void OnButtonSet(void) ;
	//void OnBUpdateInfo(QString info) ;
	void OnRunCheckBoxToggled(bool checked) ;
	void OnViewCheckBoxToggled(bool checked) ;

signals:
	void UpdateList(void);
	void UpdateResultImage(QString id);

private:
    Ui::FormToolOptionWithProgress *ui;

private:
	std::string m_str_parent_id ;
	int m_type ;

	//Set Dialog : Color Compare
	DialogSetColorCompare m_dlg_set_tool_color_compare ;    
};

#endif // FORMTOOLOPTIONWITHPROGRESS_H
