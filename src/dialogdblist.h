#ifndef DIALOGDBLIST_H
#define DIALOGDBLIST_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "cnetworkinfo.h"
#include "pugixml.hpp"

namespace Ui {
class DialogDbList;
}

class DialogDbList : public QDialog, public CNetworkInfo
{
    Q_OBJECT

public:
    explicit DialogDbList(QWidget *parent = 0);
    ~DialogDbList();

protected :
	void showEvent(QShowEvent *ev) override;

private:
    Ui::DialogDbList *ui;
	
	QStringListModel *m_db_list_model ;
};

#endif // DIALOGDBLIST_H
