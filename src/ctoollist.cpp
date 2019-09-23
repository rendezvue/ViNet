#include "ctoollist.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

CToolList::CToolList(QWidget *parent)
    : QListWidget(parent)
{
    setDragEnabled(true);
    //setViewMode(QListView::IconMode);
    //setIconSize(QSize(m_PieceSize, m_PieceSize));
    setSpacing(2);
    //setAcceptDrops(true);
    //setDropIndicatorShown(true);

	//setPalette(Qt::transparent);
	//setAttribute(Qt::WA_TransparentForMouseEvents);
}

void CToolList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(CToolList::itemMimeType()))
        event->accept();
    else
        event->ignore();
}

void CToolList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(CToolList::itemMimeType()))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void CToolList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(CToolList::itemMimeType()))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void CToolList::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    //QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    //QPixmap pixmap = qvariant_cast<QPixmap>(item-);
    //QPixmap pixmap(item->boundingRect().size().toSize());
    QPixmap pixmap(viewport()->visibleRegion().boundingRect().size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
	painter.setOpacity(0.5);
    for(QModelIndex index: selectedIndexes()){
        painter.drawPixmap(visualRect(index), viewport()->grab(visualRect(index)));
    }

    //QPoint location = item->data(Qt::UserRole+1).toPoint();
    // dataStream << pixmap << location;
    //QString str_text = item->text();
    QVariant item_user_data = item->data(Qt::UserRole+1) ;
    int type = item_user_data.toInt() ;

    //dataStream << str_text << type ;
    //dataStream << type << pixmap ;
    dataStream << type  ;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(CToolList::itemMimeType(), itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    //drag->setHotSpot( QPoint( this->width() / 2, this->height() / 2 ) );
    drag->setHotSpot(viewport()->mapFromGlobal(QCursor::pos()));
    //drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);
	
    //drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    //drag->setPixmap(pixmap);

    drag->exec(Qt::CopyAction) ;
    //if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
    //    delete takeItem(row(item));
}

