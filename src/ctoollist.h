#ifndef CTOOLLIST_H
#define CTOOLLIST_H

#include <QListWidget>

typedef struct _ToolInfo
{
	std::string name = "" ;
	int type = -1 ;
	std::string type_name = "" ;
} ToolInfo;

class CToolList : public QListWidget
{
public:
    explicit CToolList(QWidget *parent = nullptr);

    static QString itemMimeType() { return QStringLiteral("job/x-item-list"); }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;
};

#endif // CTOOLLIST_H
