#ifndef FORMJOBBASECODE_H
#define FORMJOBBASECODE_H

#include <QWidget>

namespace Ui {
class FormJobBaseCode;
}

class FormJobBaseCode : public QWidget
{
    Q_OBJECT

public:
    explicit FormJobBaseCode(QWidget *parent = 0);
    ~FormJobBaseCode();

private:
    Ui::FormJobBaseCode *ui;
};

#endif // FORMJOBBASECODE_H
