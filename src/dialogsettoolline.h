#ifndef DIALOGSETTOOLLINE_H
#define DIALOGSETTOOLLINE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "cmat2qimage.h"
#include "ImgDec.h"
#include "CImageBuf2Mat.h"
//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "dialogchangename.h"

#include "csetuserregion.h"

#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolLine;
}

class DialogSetToolLine : public QDialog, public CSetUserRegion, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolLine(QWidget *parent = 0);
    ~DialogSetToolLine();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolLine *ui;

	void updatePicture(cv::Mat image, cv::Rect rect_user=cv::Rect()) ;
	cv::Mat m_image ;

	CSetUserRegion m_cls_set_user_region ;

	cv::Point m_pt_start ;
	cv::Point m_pt_end ;

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

signals:
	void UpdateToolName(QString name) ;
	void UpdateToolObjectImage(void);

};

#endif // DIALOGSETTOOLLINE_H
