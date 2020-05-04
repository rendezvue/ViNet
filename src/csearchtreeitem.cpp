#include "csearchtreeitem.h"

CSearchTreeItem::CSearchTreeItem()
{

}

// myWidget is a QTreeWidgetItem*
QTreeWidgetItem *CSearchTreeItem::traverseNode(QTreeWidgetItem *item, const std::string id)
{
    QTreeWidgetItem *p_ret_item = NULL ;
	
	QVariant item_user_data = item->data(0, Qt::UserRole+1) ;
	QString qstr_item_id = item_user_data.toString() ;
	std::string str_item_id = qstr_item_id.toUtf8().constData() ;

	//qDebug("traverseNode item id = %s", str_item_id.c_str()) ;

	if( str_item_id == id )
    {
        p_ret_item = item ;
        return p_ret_item ;
    }

#if 0	
    FormProject* p_FromProject = (FormProject*)(item) ;
    FormJobBase* p_FromJobBase = (FormJobBase*)(item) ;
    FormJobTool* p_FromJobTool = (FormJobTool*)(item) ;
    FormToolOption* p_FromToolOption = (FormToolOption*)(item) ;

    //Check Project
    //qDebug("traverseNode Check Project") ;
    if( p_FromProject )
    {
    	//qDebug("traverseNode This is Project Form") ;
		
        if( p_FromProject->GetIdInfo() == id )
        {
        	//qDebug("Same ID") ;
			
            p_ret_item = item ;

			//qDebug("return") ;
			
            return p_ret_item ;
        }
    }

    //Check Job
    //qDebug("traverseNode Check Job") ;
    if( p_FromJobBase )
    {
    	//qDebug("traverseNode This is Job Form") ;
		
        if( p_FromJobBase->GetIdInfo() == id )
        {
            p_ret_item = item ;
            return p_ret_item ;
        }
    }

    //Check Tool
    //qDebug("traverseNode Check Tool") ;
    if( p_FromJobTool )
    {
    	//qDebug("traverseNode This is Tool Form") ;
		
        if( p_FromJobTool->GetIdInfo() == id )
        {
            p_ret_item = item ;
            return p_ret_item ;
        }
    }

    //Check Tool Option
    //qDebug("traverseNode Check Tool Option") ;
    if( p_FromToolOption )
    {
    	//qDebug("traverseNode This is Option Form") ;
		
        if( p_FromToolOption->GetIdInfo() == id )
        {
            p_ret_item = item ;
            return p_ret_item ;
        }
    }
#endif

	//qDebug("traverseNode item->childCount()=%d", item->childCount()) ;
	
    // Do something with item
    for (int i = 0; i < item->childCount(); ++i)
    {
        p_ret_item = traverseNode(item->child(i), id);

        if( p_ret_item ) return p_ret_item ;
    }

    return p_ret_item ;
}

QTreeWidgetItem *CSearchTreeItem::GetItem(CJobTree *p_tree, const std::string id)
{
    QTreeWidgetItem *p_ret_item = NULL ;

	//qDebug("Get Tree Item p_tree->topLevelItemCount()=%d", p_tree->topLevelItemCount()) ;
	
    for (int i = 0; i < p_tree->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = p_tree->topLevelItem(i);
        p_ret_item = traverseNode(item, id);

        if( p_ret_item ) return p_ret_item ;
    }

    return p_ret_item ;
}
