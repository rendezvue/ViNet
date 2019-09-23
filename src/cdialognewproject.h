#ifndef CDIALOGNEWPROJECT_H
#define CDIALOGNEWPROJECT_H

#include <QDialog>
#include <string>

namespace Ui {
class CDialogNewProject;
}

class CDialogNewProject : public QDialog
{
    Q_OBJECT

public:
    explicit CDialogNewProject(QWidget *parent = 0);
    ~CDialogNewProject();

    std::string GetName(void) ;

private:
    Ui::CDialogNewProject *ui;
};

#endif // CDialogNewProject_H
