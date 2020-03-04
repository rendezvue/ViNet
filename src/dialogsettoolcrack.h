#ifndef DIALOGSETTOOLCRACK_H
#define DIALOGSETTOOLCRACK_H

#include <QDialog>

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "csettoolinfo.h"

namespace Ui {
class DialogSetToolCrack;
}

class DialogSetToolCrack : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolCrack(QWidget *parent = 0);
    ~DialogSetToolCrack();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolCrack *ui;

public slots:
	void OnSliderSetFeatureLevel(void) ;
    void OnSliderMove(int value) ;

//signals:
//	void UpdateInfo(QString info) ;

};

#endif // DIALOGSETTOOLCRACK_H
