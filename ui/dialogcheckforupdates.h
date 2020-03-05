#ifndef DIALOGCHECKFORUPDATES_H
#define DIALOGCHECKFORUPDATES_H

#include <QDialog>
#include <QMessageBox>
#include <string>
#include <vector>

namespace Ui {
class dialogcheckforupdates;
}

class dialogcheckforupdates : public QDialog
{
    Q_OBJECT

public:
    explicit dialogcheckforupdates(QWidget *parent = 0);
    ~dialogcheckforupdates();

private slots:
    void on_pushButton_Checkforupdates_clicked();

    void on_pushButton_Apply_Restart_clicked();

private:
    Ui::dialogcheckforupdates *ui;
	std::vector<std::string> split(std::string str, char delimiter);

};

#endif // DIALOGCHECKFORUPDATES_H
