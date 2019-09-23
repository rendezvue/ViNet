#ifndef DIALOGSETTOOLANGLEINSPECT_H
#define DIALOGSETTOOLANGLEINSPECT_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

//API
#include "EnsembleAPI.h"

#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolAngleInspect;
}

class DialogSetToolAngleInspect : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolAngleInspect(QWidget *parent = 0);
    ~DialogSetToolAngleInspect();

	void SetToolId(const std::string id) ;

	float GetBaseValue(void) ;
	float GetToleranceMin(void) ;
	float GetToleranceMax(void) ;
	
protected :
	void showEvent(QShowEvent *ev) override;

public slots:
	void OnButtonGetBaseAngle(void) ;
	void OnComboGetPreset(int index) ;
	void OnButtonGetTolerance(void) ;
	void OnButtonSetTolerance(void) ;
	
private:
    Ui::DialogSetToolAngleInspect *ui;
};

#endif // DIALOGSETTOOLANGLEINSPECT_H
