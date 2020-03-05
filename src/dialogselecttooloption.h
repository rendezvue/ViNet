#ifndef DIALOGSELECTTOOLOPTION_H
#define DIALOGSELECTTOOLOPTION_H

#include <QDialog>
#include <QWidget>
#include <QListWidget>


//API
#include "EnsembleAPI.h"
#include "censemble.h"

//xml parser
#include "pugixml.hpp"

namespace Ui {
class DialogSelectToolOption;
}

class DialogSelectToolOption : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectToolOption(QWidget *parent = 0);
    ~DialogSelectToolOption();

	void SetToolType(const int type) ;
	void SetToolId(const std::string id) ;


protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogSelectToolOption *ui;

	int m_i_type ;
	std::string m_str_id ;

public slots:
	void OnButtonUpdateList(void) ;
	void OnButtonAddOption(void) ;

signals:
    void UpdateList(void);

};

#endif // DIALOGSELECTTOOLOPTION_H
