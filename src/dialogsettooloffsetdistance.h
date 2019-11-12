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

#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolOffsetDistance;
}

class DialogSetToolOffsetDistance : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolOffsetDistance(QWidget *parent = 0);
    ~DialogSetToolOffsetDistance();

	void SetToolId(const std::string id) ;

private:
    Ui::DialogSetToolOffsetDistance *ui;
	void updatePicture(cv::Mat image) ;
	cv::Mat m_image ;

protected :
	void showEvent(QShowEvent *ev) override;

public slots:
	void OnButtonGetImage(void) ;
	void OnButtonNameChange(void) ;

signals:
	void UpdateToolName(QString name) ;

};

#endif // DIALOGSETTOOLOFFSETDISTANCE_H
