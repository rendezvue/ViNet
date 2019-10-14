#ifndef DIALOGSETCUSTOMFEATUREOPTION_H
#define DIALOGSETCUSTOMFEATUREOPTION_H

#include <QDialog>

namespace Ui {
class DialogSetCustomFeatureOption;
}

class DialogSetCustomFeatureOption : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetCustomFeatureOption(QWidget *parent = 0);
    ~DialogSetCustomFeatureOption();

private:
    Ui::DialogSetCustomFeatureOption *ui;
};

#endif // DIALOGSETCUSTOMFEATUREOPTION_H
