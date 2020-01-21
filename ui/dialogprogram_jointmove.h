#ifndef DIALOGPROGRAM_JOINTMOVE_H
#define DIALOGPROGRAM_JOINTMOVE_H

#include <QDialog>

namespace Ui {
class dialogprogram_jointmove;
}

class dialogprogram_jointmove : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_jointmove(QWidget *parent = 0);
    ~dialogprogram_jointmove();

private:
    Ui::dialogprogram_jointmove *ui;
};

#endif // DIALOGPROGRAM_JOINTMOVE_H
