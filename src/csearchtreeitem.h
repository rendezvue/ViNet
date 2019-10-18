#ifndef CSEARCHTREEITEM_H
#define CSEARCHTREEITEM_H

#include <QTreeWidget>
#include "cjobtree.h"

#include "formdeviceinfo.h"
#include "formjobbase.h"
#include "formjobtool.h"
#include "formtooloption.h"
#include "formproject.h"

class CSearchTreeItem
{
public:
    CSearchTreeItem();

    QTreeWidgetItem *GetItem(CJobTree *p_tree, const std::string id) ;

private:
    QTreeWidgetItem *traverseNode(QTreeWidgetItem *item, const std::string id) ;
};

#endif // CSEARCHTREEITEM_H
