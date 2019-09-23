#ifndef DIALOGSETTOOLDIAMETERINSPECT_H
#define DIALOGSETTOOLDIAMETERINSPECT_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

//API
#include "EnsembleAPI.h"

#include "csettoolinfo.h"

namespace Ui {
class DialogSetToolDiameterInspect;
}

class DialogSetToolDiameterInspect : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolDiameterInspect(QWidget *parent = 0);
    ~DialogSetToolDiameterInspect();

	void SetToolId(const std::string id) ;

	float GetBaseValue(void) ;
	float GetToleranceMin(void) ;
	float GetToleranceMax(void) ;
	
protected :
	void showEvent(QShowEvent *ev) override;

public slots:
	void OnButtonGetBaseDiameter(void) ;
	void OnComboGetPreset(int index) ;
	void OnButtonGetTolerance(void) ;
	void OnButtonSetTolerance(void) ;
	
private:
    Ui::DialogSetToolDiameterInspect *ui;
};

#endif // DIALOGSETTOOLDIAMETERINSPECT_H
