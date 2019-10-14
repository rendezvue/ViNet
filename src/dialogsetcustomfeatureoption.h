#ifndef DIALOGSETCUSTOMFEATUREOPTION_H
#define DIALOGSETCUSTOMFEATUREOPTION_H

#include <QDialog>

//API
#include "EnsembleAPI.h"


namespace Ui {
class DialogSetCustomFeatureOption;
}

class DialogSetCustomFeatureOption : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetCustomFeatureOption(QWidget *parent = 0);
    ~DialogSetCustomFeatureOption();

	void SetId(const std::string id) ;
	
protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSetCustomFeatureOption *ui;

	std::string m_str_id ;

public slots:
	void OnButtonGet(void) ;
	void OnButtonSet(void) ;

};

#endif // DIALOGSETCUSTOMFEATUREOPTION_H
