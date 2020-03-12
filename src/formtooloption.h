#ifndef FORMTOOLOPTION_H
#define FORMTOOLOPTION_H

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
#include "dialogsettoolcrack.h"
#include "dialogsettoolcolor.h"
#include "dialogsettooldistanceinspect.h"
#include "dialogsettoolangleinspect.h"
#include "dialogsettooldiameterinspect.h"

#include <QPropertyAnimation>


namespace Ui {
class FormToolOption;
}

class FormToolOption : public QWidget, public CNetworkInfo
{
    Q_OBJECT

public:
    explicit FormToolOption(QWidget *parent = 0);
    ~FormToolOption();

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
    Ui::FormToolOption *ui;

	std::string m_str_parent_id ;
	int m_type ;

	//Set Dialog : Crack
	DialogSetToolCrack m_dlg_set_tool_crack ;
	//Set Dialog : Color
	DialogSetToolColor m_dlg_set_tool_color ;
	//Set Dialog : Distance
	DialogSetToolDistanceInspect m_dlg_set_tool_inspect_distance ;
	//Set Dialog : Angle
	DialogSetToolAngleInspect m_dlg_set_tool_inspect_angle ;
	//Set Dialog : Diameter
	DialogSetToolDiameterInspect m_dlg_set_tool_inspect_diameter ;
};

#endif // FORMTOOLOPTION_H
