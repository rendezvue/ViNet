#ifndef FORMPROJECT_H
#define FORMPROJECT_H

#include <QWidget>
#include <QHoverEvent>
#include <QEvent>
#include <QDebug>

//API
#include "EnsembleAPI.h"

#include "dialogchangename.h"


namespace Ui {
class FormProject;
}

class FormProject : public QWidget
{
    Q_OBJECT

public:
    explicit FormProject(QWidget *parent = 0);
    ~FormProject();

	void SetNameInfo(const std::string name) ;
	std::string GetNameInfo(void) ;

	void SetIdInfo(const std::string id) ;
    std::string GetIdInfo(void)  ;

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    bool event(QEvent * e);

    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    void hoverMove(QHoverEvent * event);

private:
    Ui::FormProject *ui;

signals:
	void UpdateList(void);
	void UpdateResultImage(QString id) ;
	void UpdateResult(QString qstr_result_xml) ;
	

public slots:
	void OnButtonDel(void);
	void OnButtonSetName(void) ;
	void OnButtonRun(void) ;


};

#endif // FORMPROJECT_H
