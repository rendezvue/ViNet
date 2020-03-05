#ifndef DIALOGSETTOOLINSPECTDISTANCE_H
#define DIALOGSETTOOLINSPECTDISTANCE_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "dialogchangename.h"

#include "csettoolinfo.h"


namespace Ui {
class DialogSetToolInspectDistance;
}

class DialogSetToolInspectDistance : public QDialog, public CSetToolInfo
{
    Q_OBJECT

public:
    explicit DialogSetToolInspectDistance(QWidget *parent = 0);
    ~DialogSetToolInspectDistance();

	void SetToolId(const std::string id) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetToolInspectDistance *ui;
};

#endif // DIALOGSETTOOLINSPECTDISTANCE_H
