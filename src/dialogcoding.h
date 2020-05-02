#ifndef DIALOGCODING_H
#define DIALOGCODING_H

#include <QDialog>

namespace Ui {
class DialogCoding;
}

class DialogCoding : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCoding(QWidget *parent = 0);
    ~DialogCoding();

private:
    Ui::DialogCoding *ui;
};

#endif // DIALOGCODING_H
