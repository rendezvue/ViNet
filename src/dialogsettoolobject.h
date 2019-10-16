#ifndef DIALOGSETTOOLOBJECT_H
#define DIALOGSETTOOLOBJECT_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>


#include "ImgDec.h"

//API
#include "EnsembleAPI.h"

#include "dialogchangename.h"
#include "dialogsetcustomfeatureoption.h"

#include "csetuserregion.h"

#include "csettoolinfo.h"

namespace Ui {
class DialogSetToolObject;
}

class DialogSetToolObject : public QDialog, public CSetUserRegion, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolObject(QWidget *parent = 0);
    ~DialogSetToolObject();

    void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolObject *ui;
	
	void updatePicture(cv::Mat image, cv::Rect rect_user=cv::Rect()) ;
	void updatePictureCenterLine(cv::Mat image, cv::Point pt_user=cv::Point()) ;
	cv::Mat m_image ;

	CSetUserRegion m_cls_set_user_region ;
	
public slots:
	void OnButtonGetImage(void) ;
    void OnButtonNameChange(void) ;
	void OnSliderSetFeatureLevel(void) ;
    void OnSliderMove(int value) ;

	void OnButtonSelectObject(void) ;
	void OnButtonResetObject(void) ;

	void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

	//ref point
	void OnButtonSelectRefPoint(void) ;
	void OnButtonResetRefPoint(void) ;
	
	void OnCheckFeatureUseCustomOption(bool checked) ;

signals:
	void UpdateToolName(QString name) ;
	void UpdateToolObjectImage(void);

};

#endif // DIALOGSETTOOLOBJECT_H
