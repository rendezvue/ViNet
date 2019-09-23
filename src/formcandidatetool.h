#ifndef FORMCANDIDATETOOL_H
#define FORMCANDIDATETOOL_H

#include <QWidget>

namespace Ui {
class FormCandidateTool;
}

class FormCandidateTool : public QWidget
{
    Q_OBJECT

public:
    explicit FormCandidateTool(QWidget *parent = 0);
    ~FormCandidateTool();

    void SetBackground(QPixmap &image);
    void SetText(const std::string text) ;

private:
    Ui::FormCandidateTool *ui;
};

#endif // FORMCANDIDATETOOL_H
