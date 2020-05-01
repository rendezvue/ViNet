#ifndef FORMTOOLOPTIONWITHPROGRESS_H
#define FORMTOOLOPTIONWITHPROGRESS_H

#include <QWidget>

namespace Ui {
class FormToolOptionWithProgress;
}

class FormToolOptionWithProgress : public QWidget
{
    Q_OBJECT

public:
    explicit FormToolOptionWithProgress(QWidget *parent = 0);
    ~FormToolOptionWithProgress();

private:
    Ui::FormToolOptionWithProgress *ui;
};

#endif // FORMTOOLOPTIONWITHPROGRESS_H
