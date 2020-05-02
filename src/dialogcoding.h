#ifndef DIALOGCODING_H
#define DIALOGCODING_H

#include <QDialog>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "EnsembleCommon.h"

//API
#include "EnsembleAPI.h"
#include "censemble.h"

#include "dialogchangename.h"


namespace Ui {
class DialogCoding;
}

class DialogCoding : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCoding(QWidget *parent = 0);
    ~DialogCoding();

	void SetId(const std::string id) ;
    std::string GetId(void) ;

protected :
	void showEvent(QShowEvent *ev) override;

private:
	std::string m_str_id ;
	
private:
    Ui::DialogCoding *ui;

public slots:
    void OnButtonNameChange(void) ;	
	void OnButtonUpload(void) ;	
	void OnButtonDownload(void) ;	
	void OnButtonUploadAndRun(void) ;	

signals:
	void UpdateBaseName(QString name) ;	
};

#endif // DIALOGCODING_H
