#ifndef CANIMATEDLABEL_H
#define CANIMATEDLABEL_H

#include <QWidget>
#include <QLabel>

class CAnimatedLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    CAnimatedLabel(QWidget *parent = 0)
    {
    }

    void setColor (QColor color) ;
    QColor color() ;
};

#endif // CANIMATEDLABEL_H
