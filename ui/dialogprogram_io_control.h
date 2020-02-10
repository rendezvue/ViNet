#ifndef DIALOGPROGRAM_IO_CONTROL_H
#define DIALOGPROGRAM_IO_CONTROL_H

#include <QDialog>

namespace Ui {
class dialogprogram_io_control;
}

class dialogprogram_io_control : public QDialog
{
    Q_OBJECT

public:
    explicit dialogprogram_io_control(QWidget *parent = 0);
    ~dialogprogram_io_control();

private:
    Ui::dialogprogram_io_control *ui;
};

#endif // DIALOGPROGRAM_IO_CONTROL_H
