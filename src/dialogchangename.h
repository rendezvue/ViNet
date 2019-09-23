#ifndef DIALOGCHANGENAME_H
#define DIALOGCHANGENAME_H

#include <QDialog>

namespace Ui {
class DialogChangeName;
}

class DialogChangeName : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChangeName(QWidget *parent = 0);
    ~DialogChangeName();

    void SetId(const std::string id) ;
    void SetName(const std::string name) ;
    std::string GetName(void) ;

private:
    Ui::DialogChangeName *ui;
};

#endif // DIALOGCHANGENAME_H
