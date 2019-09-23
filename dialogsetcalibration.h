#ifndef DIALOGSETCALIBRATION_H
#define DIALOGSETCALIBRATION_H

#include <QDialog>

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

protected :
	void showEvent(QShowEvent *ev) override;

public slots:
	void OnButtonGetChessInfo(void) ;


};

#endif // DIALOGSETCALIBRATION_H
