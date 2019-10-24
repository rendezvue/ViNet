#ifndef FORMJOBBASE_H
#define FORMJOBBASE_H

#include <QWidget>
#include <QHoverEvent>
#include <QEvent>
#include <QDebug>

#include "EnsembleCommon.h"

//API
#include "EnsembleAPI.h"

//dialog
#include "dialogsetbase.h"
#include "dialogsetcalibration.h"
#include "dialogresultinfo.h"


#include "ImgDec.h"

//opencv
#include "opencv2/opencv.hpp"

#include "cmat2qimage.h"

namespace Ui {
class FormJobBase;
}

class FormJobBase : public QWidget
{
    Q_OBJECT

public:
    explicit FormJobBase(QWidget *parent = 0);
    ~FormJobBase();

    void SetNameInfo(const std::string name) ;
    std::string GetNameInfo(void) ;

    void SetIdInfo(const std::string id) ;
    std::string GetIdInfo(void) ;

    int GetType(void) ;

	void SetImage(cv::Mat image) ;

	void SetObjectImage(cv::Mat image) ;

	void SetInfo(const std::string info) ;
	void SetResultString(const std::string str_result) ;

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
    Ui::FormJobBase *ui;

    const int m_type ;
	std::string m_str_info ;
	std::string m_str_result ;

	DialogSetBase m_dlg_setbase ;
	DialogSetCalibration m_dlg_set_calibration ;
	
signals:
    void UpdateList(void);
	void UpdateResultImage(QString id) ;

public slots:
	void OnButtonDel(void);
    void OnButtonSetBase(void) ;
	void OnUpdateImage(void) ;
	void OnUpdateName(QString name) ;
	void OnButtonRun(void) ;
	void OnRunCheckBoxToggled(bool checked) ;
	void OnViewCheckBoxToggled(bool checked) ;
	void OnButtonSetCalibration(void) ;
	void OnButtonResultInfo(void) ;
	void UpdateInfo(void) ;
};

#endif // FORMJOBBASE_H
