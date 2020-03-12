#ifndef DIALOGSETTOOLDISTANCE_H
#define DIALOGSETTOOLDISTANCE_H

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

#include "csettoolinfo.h"

namespace Ui {
class DialogSetToolDistance;
}

class DialogSetToolDistance : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolDistance(QWidget *parent = 0);
    ~DialogSetToolDistance();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolDistance *ui;

	void updatePicture(cv::Mat image) ;
	cv::Mat m_image ;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonNameChange(void) ;
    //void OnButtonGetTolRate(void) ;
	void OnButtonGetCalcDistance(void) ;

signals:
	void UpdateToolName(QString name) ;


};

#endif // DIALOGSETTOOLDISTANCE_H
