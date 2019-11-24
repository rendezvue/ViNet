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
	cv::Mat m_image ;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonNameChange(void) ;

signals:
	void UpdateToolName(QString name) ;

};

#endif // DIALOGSETCODE_H
