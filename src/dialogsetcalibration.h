#ifndef DIALOGSETCALIBRATION_H
#define DIALOGSETCALIBRATION_H

#include <QDialog>
#include <QMouseEvent>


//API
#include "EnsembleAPI.h"


namespace Ui {
class DialogSetCalibration;
}

class DialogSetCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetCalibration(QWidget *parent = 0);
    ~DialogSetCalibration();

	void SetId(const std::string id) ;
    std::string GetId(void) ;
	
private:
    Ui::DialogSetCalibration *ui;

	std::string m_str_id ;
	void UpdateDataCalibrationRun(float f_pixel_x, float f_pixel_y) ;
		
protected :
	void showEvent(QShowEvent *ev) override;
	void mousePressEvent(QMouseEvent *event) override;

public slots:
	void OnButtonGetChessInfo(void) ;
	void OnButtonGetCalibrationImage(void) ;
	void OnButtonAddCalibrationInfo(void) ;
	void OnButtonDelCalibrationInfo(void) ;
	void OnButtonClearCalibrationInfo(void) ;
	void OnButtonUpdateCalibrationInfo(void) ;
	void OnButtonCalibrationRun(void) ;


};

#endif // DIALOGSETCALIBRATION_H
