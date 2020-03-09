#ifndef CJOBTREE_H
#define CJOBTREE_H

#include <QApplication>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDropEvent>

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QProxyStyle>
#include <QStyledItemDelegate>

#include "ctoollist.h"
#include "formcandidatetool.h"

#include "EnsembleCommon.h"

#include "EnsembleAPI.h"


#if 0
class CJobTreeStyle: public QProxyStyle
{
public:
    CJobTreeStyle(QStyle* style = 0) : QProxyStyle(style) {}

    void drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const
    {
        //qDebug("%s", __func__) ;
        //qDebug("element : (%d)%d", QStyle::PE_IndicatorItemViewItemDrop, element);
		
        if(element == QStyle::PE_IndicatorItemViewItemDrop)
        {
            painter->setRenderHint(QPainter::Antialiasing, true);

            QPalette palette;
            //QColor(90,108,217)
            QColor c(palette.highlightedText().color());
            QPen pen(c);
            pen.setWidth(2);
            c.setAlpha(50);
            QBrush brush(c);

            painter->setPen(pen);
            painter->setBrush(brush);
            if(option->rect.height() == 0)
            {
                painter->drawEllipse(option->rect.topLeft(), 3, 3);
                painter->drawLine(QPoint(option->rect.topLeft().x()+3, option->rect.topLeft().y()), option->rect.topRight());
            } else {
                painter->drawRoundedRect(option->rect, 5, 5);
            }
        } else {
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }

    }
};
#endif

/*
class MyStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit MyStyledItemDelegate(QObject *parent = 0) : QStyledItemDelegate(parent){}

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        //background
        QColor bgColor;
        int bgColorType(0);
        bgColorType = index.data(Qt::UserRole+9).toInt();//custom flag I set to determine which color i want

        //color logic
        if(bgColorType == 0)
            bgColor = QColor(Qt::transparent);//default is transparent to retain alternate row colors
        else if(bgColorType == 1)
            bgColor = qRgba(237, 106, 106, 255);//red
        else if(bgColorType == 2)
            bgColor = qRgba(241, 167, 226, 255);//pink
        //etc...

        QStyleOptionViewItem opt(option);

        if(option.state & QStyle::State_Selected)//check if item is selected
        {
            //more color logic
            if(bgColorType == 0)
                bgColor = qRgba(190, 220, 240, 255);
            else
                bgColor = qRgba(bgColor.red()-25, bgColor.green()-25, bgColor.blue()-25, 255);

            //background color won't show on selected items unless you do this
            opt.palette.setBrush(QPalette::Highlight, QBrush(bgColor));
        }

        if(option.state & QStyle::State_MouseOver)//check if item is hovered
        {
            //more color logic
            bgColor = qRgba(bgColor.red()-25, bgColor.green()-25, bgColor.blue()-25, 255);

            if(option.state & QStyle::State_Selected)//check if it is hovered AND selected
            {
                //more color logic
                if(bgColorType == 0)
                {
                    bgColor = qRgba(148, 200, 234, 255);
                }

                //background color won't show on selected items unless you do this
                opt.palette.setBrush(QPalette::Highlight, QBrush(bgColor));
            }
        }


        //set the backgroundBrush to our color. This affects unselected items.
        opt.backgroundBrush = QBrush(bgColor);

        //draw the item background
        option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

        //icon
        QRect iconRect = option.rect;
        iconRect.setLeft(iconRect.left()+3);//offset it a bit to the right
        //draw in icon, this can be grabbed from Qt::DecorationRole
        //altho it appears icons must be set with setIcon()
        option.widget->style()->drawItemPixmap(painter, iconRect, Qt::AlignLeft | Qt::AlignVCenter, QIcon(index.data(Qt::DecorationRole).value<QIcon>()).pixmap(16, 16));

        //text
        QRect textRect = option.rect;
        textRect.setLeft(textRect.left()+25);//offset it a bit to the right
        //draw in text, this can be grabbed from Qt::DisplayRole
        option.widget->style()->drawItemText(painter, textRect, Qt::AlignLeft | Qt::AlignVCenter, option.palette, true, index.data(Qt::DisplayRole).toString());
    }
} ;
*/

class CJobTree : public QTreeWidget
{
    Q_OBJECT

public:
    explicit CJobTree(QWidget *parent = 0);
    //explicit CJobTree(QObject *parent = 0) {} ;
    //virtual ~CJobTree() {};

	static QString itemMimeType() { return QStringLiteral("job/x-job-list"); }
	
protected:
#if 1
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
#endif
    void dropEvent(QDropEvent *event) override;
	void startDrag(Qt::DropActions supportedActions) override;
    void paintEvent(QPaintEvent * event) override;
   //void mouseMoveEvent(QMouseEvent* event) override;

private: 
    QRect m_dropIndicatorRect;
	int m_drag_countrow_for_redorder;

	void paintDropIndicator(QPainter* painter) ;
    struct Item
    {
        QString type;
    };

	QTreeWidgetItem* m_p_item_candidate ;
    void delCandidateItem(void) ;

	int countRow(const QModelIndex & index) ;
		
signals :
    void SignalDropDone(void) ;
};

#endif // CJOBTREE_H
