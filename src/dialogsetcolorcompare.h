#ifndef DIALOGSETCOLORCOMPARE_H
#define DIALOGSETCOLORCOMPARE_H

#include <QDialog>

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "csettoolinfo.h"

namespace Ui {
class DialogSetColorCompare;
}

class DialogSetColorCompare : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetColorCompare(QWidget *parent = 0);
    ~DialogSetColorCompare();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;
	
private:
    Ui::DialogSetColorCompare *ui;

public slots:
	void OnSliderSetFeatureLevel(void) ;
	void OnSliderMove(int value) ;

};

#endif // DIALOGSETCOLORCOMPARE_H
