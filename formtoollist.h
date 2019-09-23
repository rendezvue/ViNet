#ifndef FORMTOOLLIST_H
#define FORMTOOLLIST_H

#include <QWidget>
#include <string>

#include "opencv2/opencv.hpp"
#include "EnsembleCommon.h"

namespace Ui {
class FormToolList;
}

class FormToolList : public QWidget
{
    Q_OBJECT

public:
    explicit FormToolList(QWidget *parent = 0);
    ~FormToolList();

    void SetName(const std::string name) ;
    std::string GetName(void) ;

    void SetTypeNumber(const int typenumber) ;
    int GetTypeNumber(void) ;

private:
    Ui::FormToolList *ui;
};

#endif // FORMTOOLLIST_H
