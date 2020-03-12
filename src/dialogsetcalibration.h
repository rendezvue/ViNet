#ifndef DIALOGSETCALIBRATION_H
#define DIALOGSETCALIBRATION_H

#include <QDialog>
#include <QMouseEvent>
#include <QStringList>
#include <QStringListModel>


#include "cmat2qimage.h"
#include "CImageBuf2Mat.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "pugixml.hpp"

#include "dialogbasecameraconfig.h"


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

	QStringListModel *m_calibration_copy_list_model ;
		
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

	void OnButtonCalibrationCopyListUpdate(void) ;
	void OnButtonCalibrationCopy(void) ;

	void OnButtonCameraConfig(void) ;


};

#endif // DIALOGSETCALIBRATION_H
