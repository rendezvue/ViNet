#ifndef FORMJOBTOOL_H
#define FORMJOBTOOL_H

#include <QWidget>
#include <QHoverEvent>
#include <QEvent>

//API
#include "EnsembleAPI.h"

//dialog
#include "dialogsettoolobject.h"
#include "dialogsettoolcrack.h"
#include "dialogsettoolline.h"
#include "dialogsettoolcircle.h"
#include "dialogsettooldistance.h"
#include "dialogsettoolangle.h"
#include "dialogsettoolinspectdistance.h"
#include "dialogselecttooloption.h"
#include "dialogsettooloffsetdistance.h"

#include "ImgDec.h"

//opencv
#include "opencv2/opencv.hpp"

#include "cmat2qimage.h"


namespace Ui {
class FormJobTool;
}

class FormJobTool : public QWidget
{
    Q_OBJECT

public:
    explicit FormJobTool(QWidget *parent = 0);
    ~FormJobTool();

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

	void SetAlarm(const bool b_on_off) ;
	
protected:
	void showEvent(QShowEvent *ev);
	
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool event(QEvent * e);

    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);
	

private:
    Ui::FormJobTool *ui;
	std::string m_str_parent_id ;

    //Set Dialog : Object
    DialogSetToolObject m_dlg_set_tool_object ;
	//Set Dialog : Crack
	DialogSetToolCrack m_dlg_set_tool_crack ;
	//Set Dialog : Line
	DialogSetToolLine m_dlg_set_tool_line ;	
	//Set Dialog : Circle
	DialogSetToolCircle m_dlg_set_tool_circle ;	
	//Set Dialog : Distance
	DialogSetToolDistance m_dlg_set_tool_distance ;	
	//Set Dialog : Angle
	DialogSetToolAngle m_dlg_set_tool_angle ;	
	//Set Dialog : Inspect Distance
	DialogSetToolInspectDistance m_dlg_set_tool_inspect_distance ;	
	//Set Dialog : Offset Distance
	DialogSetToolOffsetDistance m_dlg_set_tool_offset_distance ;

	//Select Option Dialog
    DialogSelectToolOption m_dlg_select_option ;
	
	int m_type ;
	int m_i_option_count ;

signals:
	void UpdateList(void);
	void UpdateResultImage(QString id) ;

public slots:
	void OnUpdateList(void);
	void OnButtonDel(void);
    void OnButtonSetBase(void) ;
	void OnButtonSetOption(void) ;
	void OnUpdateImage(void) ;
	void OnUpdateName(QString name) ;
	void OnRunCheckBoxToggled(bool checked) ;
	void OnViewCheckBoxToggled(bool checked) ;

};

#endif // FORMJOBTOOL_H
