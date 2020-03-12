#ifndef DIALOGSETTOOLCOLOR_H
#define DIALOGSETTOOLCOLOR_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>


#include "ImgDec.h"
#include "CImageBuf2Mat.h"
//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "dialogchangename.h"

#include "csettoolinfo.h"

#include "cmat2qimage.h"


namespace Ui {
class DialogSetToolColor;
}

class DialogSetToolColor : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolColor(QWidget *parent = 0);
    ~DialogSetToolColor();

	void SetToolId(const std::string id) ;

	int GetBaseValue(void) ;
	int GetToleranceMin(void) ;
	int GetToleranceMax(void) ;
	
protected :
	void showEvent(QShowEvent *ev) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    Ui::DialogSetToolColor *ui;

	void updatePicture(cv::Mat image) ;
	cv::Mat m_image ;

	void CheckedUseRed(void) ;
	void CheckedUseGreen(void) ;
	void CheckedUseBlue(void) ;

    int m_i_set_color ;
    int m_i_s_value ;
    int m_i_e_value ;
	
	
public slots:
	void OnButtonGetImage(void) ;
	void OnButtonGetRedHistogramImage(void) ;
	void OnButtonGetGreenHistogramImage(void) ;
	void OnButtonGetBlueHistogramImage(void) ;

	void OnButtonGetRedHistogramRange(void) ;
	void OnButtonSetRedHistogramRange(void) ;
	void OnButtonGetGreenHistogramRange(void) ;
	void OnButtonSetGreenHistogramRange(void) ;
	void OnButtonGetBlueHistogramRange(void) ;
	void OnButtonSetBlueHistogramRange(void) ;
	
	void OnButtonGetBasePixelCount(void) ;

	void OnButtonGetTolerance(void) ;
	void OnButtonSetTolerance(void) ;
	
	void OnButtonSetUseRed(bool checked) ;
	void OnButtonSetUseGreen(bool checked) ;
	void OnButtonSetUseBlue(bool checked) ;

	void OnComboGetPreset(int index) ;
	
};

#endif // DIALOGSETTOOLCOLOR_H
