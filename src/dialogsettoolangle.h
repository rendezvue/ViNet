#ifndef DIALOGSETTOOLANGLE_H
#define DIALOGSETTOOLANGLE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>


#include "ImgDec.h"

//API
#include "EnsembleAPI.h"

#include "dialogchangename.h"

#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolAngle;
}

class DialogSetToolAngle : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolAngle(QWidget *parent = 0);
    ~DialogSetToolAngle();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolAngle *ui;

	void updatePicture(cv::Mat image) ;
	cv::Mat m_image ;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonNameChange(void) ;
	//void OnButtonGetTolRate(void) ;
	void OnButtonGetCalcAngle(void) ;

signals:
	void UpdateToolName(QString name) ;

};

#endif // DIALOGSETTOOLANGLE_H
