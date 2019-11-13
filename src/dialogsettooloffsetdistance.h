#ifndef DIALOGSETTOOLOFFSETDISTANCE_H
#define DIALOGSETTOOLOFFSETDISTANCE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "cmat2qimage.h"

#include "ImgDec.h"

//API
#include "EnsembleAPI.h"

#include "dialogchangename.h"

#include "csetuserregion.h"
#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolOffsetDistance;
}

class DialogSetToolOffsetDistance : public QDialog, public CSetUserRegion, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolOffsetDistance(QWidget *parent = 0);
    ~DialogSetToolOffsetDistance();

	void SetToolId(const std::string id) ;

private:
    Ui::DialogSetToolOffsetDistance *ui;
	void updatePicture(cv::Mat image, cv::Rect rect_user=cv::Rect()) ;
	cv::Mat m_image ;

	CSetUserRegion m_cls_set_user_region ;

protected :
	void showEvent(QShowEvent *ev) override;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonNameChange(void) ;

	void OnButtonRegionGet(void) ;
	void OnButtonRegionSet(void) ;

	void OnButtonChangeDirection(void) ;

	void OnButtonSelectObject(void) ;
	void OnButtonResetObject(void) ;

	//mouse event
	void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
	void UpdateToolName(QString name) ;
	void UpdateToolObjectImage(void);

};

#endif // DIALOGSETTOOLOFFSETDISTANCE_H
