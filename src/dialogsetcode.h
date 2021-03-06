#ifndef DIALOGSETCODE_H
#define DIALOGSETCODE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "cmat2qimage.h"

#include "ImgDec.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "EnsembleCommon.h"


#include "dialogchangename.h"

#include "csettoolinfo.h"


namespace Ui {
class DialogSetCode;
}

class DialogSetCode : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetCode(QWidget *parent = 0);
    ~DialogSetCode();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetCode *ui;

	void updatePicture(cv::Mat image, cv::Rect rect_user=cv::Rect()) ;
	void SetObjectImage(cv::Mat image) ;
	cv::Mat m_image ;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonNameChange(void) ;
	void OnButtonGetPaddingRate(void) ;
	void OnButtonSetPaddingRate(void) ;

signals:
	void UpdateToolName(QString name) ;
	void UpdateToolObjectImage(void);

};

#endif // DIALOGSETCODE_H
