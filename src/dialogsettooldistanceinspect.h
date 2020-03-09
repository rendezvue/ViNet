#ifndef DIALOGSETTOOLDISTANCEINSPECT_H
#define DIALOGSETTOOLDISTANCEINSPECT_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolDistanceInspect;
}

class DialogSetToolDistanceInspect : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolDistanceInspect(QWidget *parent = 0);
    ~DialogSetToolDistanceInspect();

	void SetToolId(const std::string id) ;

	float GetBaseValue(void) ;
	float GetToleranceMin(void) ;
	float GetToleranceMax(void) ;
	
protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolDistanceInspect *ui;

public slots:
	void OnButtonGetBaseDistance(void) ;
	void OnComboGetPreset(int index) ;
	void OnButtonGetTolerance(void) ;
	void OnButtonSetTolerance(void) ;
	

};

#endif // DIALOGSETTOOLDISTANCEINSPECT_H
