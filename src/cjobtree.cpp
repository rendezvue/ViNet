#include "cjobtree.h"
#include "mainwindow.h"

CJobTree::CJobTree(QWidget *parent) :
    QTreeWidget(parent)
{
    m_p_item_candidate = NULL ;
    m_drag_countrow_for_redorder = -1 ;
	m_dropIndicatorRect = QRect() ;
/*
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
*/
    //setStyle(new CJobTreeStyle(style()));
    //setItemDelegate(new MyStyledItemDelegate(this));

    //setDropIndicatorShown(true);
    //setAcceptDrops(true);

	setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setMouseTracking(true);

    //setDragDropMode(QAbstractItemView::InternalMove);

    /*
    QMap<QString, QStringList> treeMap;
    treeMap.insert("Asia", QStringList() << "Beijing Capital International Airport"
                   << "Dubai International Airport" << "Hong Kong International Airport"
                   << "Shanghai Pudong International Airport" << "Soekarno-Hatta International Airport"
                   << "Singapore Changi Airport");
    treeMap.insert("Europe", QStringList() << "Heathrow Airport" << "Charles de Gaulle Airport"
                   << "Amsterdam Airport Schiphol" << "Frankfurt Airport"
                   << "Istanbul Ataturk Airport" << "Adolfo Suarez Madrid–Barajas Airport");

    QTreeWidgetItem *parentItem = Q_NULLPTR;
    QTreeWidgetItem *childItem = Q_NULLPTR;

    QMapIterator<QString, QStringList> i(treeMap);
    while (i.hasNext()) {
        i.next();
        parentItem = new QTreeWidgetItem(widget);
        parentItem->setText(0, i.key());
        foreach (const auto& str, i.value()) {
           childItem = new QTreeWidgetItem;
           childItem->setText(0, str);
           parentItem->addChild(childItem);
        }
    }
    */

	//setStyleSheet(".QFrame{border: 1px solid black; border-radius: 3px;}");
	
}

void CJobTree::dropEvent(QDropEvent *event)
{
	int cur_index = m_drag_countrow_for_redorder ;
	int target_index = -1 ;
	
	if( m_p_item_candidate != NULL )
	{
		if( indexFromItem(m_p_item_candidate).isValid() )
		{
			//index
			QModelIndex index = indexFromItem(m_p_item_candidate) ;
			target_index = index.row() ;
		}
	}

    qDebug("CJobTree : %s : target_index=%d", __func__, target_index) ;
    //delCandidateItem() ;

	delCandidateItem() ;
	
    if (event->mimeData()->hasFormat(CToolList::itemMimeType()))		//Add a New Base job or Tool
    {
        //--
        //target
        QPoint pos = event->pos() ;
        QTreeWidgetItem* item = this->itemAt(pos);

		QByteArray itemData = event->mimeData()->data(CToolList::itemMimeType());
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        int item_from_type = -1;
        dataStream >> item_from_type;		
		int item_to_type = item->type() ;
		
        qDebug("dropEvent : New Job = item_from_type:%d -> item_to_type:%d", item_from_type, item_to_type) ;

        QVariant item_user_data = item->data(0, Qt::UserRole+1) ;
        QString job_target_id = item_user_data.toString() ;
        std::string str_target_id = job_target_id.toUtf8().constData() ;

        qDebug("dropEvent : project id = %s", str_target_id.c_str()) ;

		if( Ensemble_Network_IsOnline() & ENSEMBLE_CONNECT_CONTROL_PORT )
		{
			if( item_to_type == JobType::JOB_TYPE_PROJECT )
			{
				if( item_from_type >= JobType::JOB_TYPE_BASE && item_from_type < JobType::JOB_TYPE_BASE+10000 )
				{
					qDebug("call API : Ensemble_Job_Add_New(%s)", str_target_id.c_str()) ;
					
					Ensemble_Job_Add_New(str_target_id, item_from_type) ;
				}
			}
			else if( item_to_type == JobType::JOB_TYPE_BASE )
			{
				if( item_from_type >= JobType::JOB_TYPE_TOOL && item_from_type < JobType::JOB_TYPE_TOOL+10000 )
				{
                    qDebug("call API : Ensemble_Tool_Add_New (target id = %s)", str_target_id.c_str()) ;
					
					Ensemble_Tool_Add_New(str_target_id, item_from_type) ;
				}
			}

#if 0			
			else if( item->type() == JobType::JOB_TYPE_BASE )
			{
                Ensemble_Tool_Add_New(str_job_base_id, type) ;

				qDebug("call API : EnsembleAddTool(%s, %d)", str_job_project_id.c_str(), type) ;
			}
			else if( item->type() == JobType::JOB_TYPE_TOOL )
			{
				qDebug("call API : EnsembleInsertTool : Type Tool") ;
				
				//parent
				QTreeWidgetItem* parent_item = item->parent() ;

				if( parent_item != nullptr )
                {
                    if( parent_item->type() == JobType::JOB_TYPE_BASE )
                    {
						QVariant parent_item_user_data = parent_item->data(0, Qt::UserRole+1) ;
				        QString parent_job_base_id = parent_item_user_data.toString() ;
				        std::string str_parent_job_base_id = parent_job_base_id.toUtf8().constData() ;

                        Ensemble_Tool_Insert(str_parent_job_base_id, target_index, type) ;

						qDebug("call API : EnsembleInsertTool : Type Tool : parent id=%s, target_index=%d, type=%d", str_parent_job_base_id.c_str(), target_index, type) ;
						
                	}
				}
				
			}
#endif			
		}
				
        m_dropIndicatorRect = QRect() ;

        event->setDropAction(Qt::CopyAction);
        event->accept();

        emit SignalDropDone();
    }
	else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
	{
		QByteArray itemData = event->mimeData()->data(CJobTree::itemMimeType());
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        std::string str_tool_id ;
		QString qstr_item_tool_id ;
        //dataStream >> text >> type  ;
        dataStream >> qstr_item_tool_id ;

		str_tool_id = qstr_item_tool_id.toStdString() ;
        //std::string str_text =text.toUtf8().constData();

        qDebug("%s : dropEvent : move id = %s", __func__, str_tool_id.c_str()) ;

		QPoint pos = event->pos() ;
        QTreeWidgetItem* item = this->itemAt(pos);
		
		//Move
		if( Ensemble_Network_IsOnline() & ENSEMBLE_CONNECT_CONTROL_PORT )
		{
#if 0
			if( item->type() == JobType::JOB_TYPE_TOOL )
			{
				//parent
				QTreeWidgetItem* parent_item = item->parent() ;

				if( parent_item != nullptr )
                {
                	
                    if( parent_item->type() == JobType::JOB_TYPE_BASE )
                    {
						QVariant parent_item_user_data = parent_item->data(0, Qt::UserRole+1) ;
				        QString parent_job_base_id = parent_item_user_data.toString() ;
				        std::string str_parent_job_base_id = parent_job_base_id.toUtf8().constData() ;

						//qDebug("%s : dropEvent : call EnsembleMoveTool API : str_parent_job_base_id=%s,cur_index=%d,target_index=%d", __func__, str_parent_job_base_id.c_str(), cur_index, target_index) ;
						EnsembleMoveTool(str_parent_job_base_id, cur_index, target_index) ;
						
                	}
				}
			}
#else
			QVariant parent_item_user_data ; //= parent_item->data(0, Qt::UserRole+1) ;
	        QString parent_job_base_id ; // = parent_item_user_data.toString() ;
	        std::string str_parent_job_base_id ; // = parent_job_base_id.toUtf8().constData() ;

			if( item->type() == JobType::JOB_TYPE_TOOL )
			{
				//parent
				QTreeWidgetItem* parent_item = item->parent() ;

				if( parent_item != nullptr )
                {                	
                    if( parent_item->type() == JobType::JOB_TYPE_BASE )
                    {
						parent_item_user_data = parent_item->data(0, Qt::UserRole+1) ;
				        
                	}
				}
			}
			else if( item->type() == JobType::JOB_TYPE_BASE )
			{
				parent_item_user_data = item->data(0, Qt::UserRole+1) ;
			}

			parent_job_base_id = parent_item_user_data.toString() ;
	        str_parent_job_base_id = parent_job_base_id.toUtf8().constData() ;						

			if( str_parent_job_base_id.size() > 0 )
			{
                Ensemble_Tool_Move(str_parent_job_base_id, cur_index, target_index) ;
			}
#endif
		}
	
		//finish
		m_dropIndicatorRect = QRect() ;

		event->setDropAction(Qt::CopyAction);
        event->accept();
		
        emit SignalDropDone();
	}
    else
    {
        m_drag_countrow_for_redorder = -1 ;
        m_dropIndicatorRect = QRect() ;
        event->ignore();
    }

	this->viewport()->update();
	
    /*
    if( event->source() == this )
    {
        event->setDropAction;
         QTreeWidget::dropEvent(event);
    }
    else
    {

    }

    QModelIndex droppedIndex = indexAt( event->pos() );
    qDebug("%s : event pos = %d, %d", __func__, event->pos().x(), event->pos().y()) ;

    event->mimeData();

    QString str_test = droppedIndex.model()->objectName() ;

    qDebug("%s : str_test = %s", __func__, str_test) ;

    QModelIndex curIndex = this->indexAt(event->pos());
    curIndex = this->model()->index(curIndex.row(), curIndex.column(), curIndex.parent());

    //TreeItem *curItem = urnTree->getItem(curIndex);
    //cout << "Current index is: " << curIndex.row() << "," << curIndex.column() << endl;
    //cout << "Current item on drop is: " << curItem->data(0).toString().toStdString() << endl;

    if( !droppedIndex.isValid() )
        return;

    QTreeWidget::dropEvent(event);
    */
}

#if 1
void CJobTree::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug("CJobTree : %s", __func__) ;
    delCandidateItem() ;

    if (event->mimeData()->hasFormat(CToolList::itemMimeType()) || event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
    {
       	//m_drag_countrow_for_redorder = -1 ;
        m_dropIndicatorRect = QRect() ;

    	event->acceptProposedAction(); 
        event->accept();
    }
    else
    {
        m_drag_countrow_for_redorder = -1 ;
        m_dropIndicatorRect = QRect() ;
        event->ignore();
    }
}

void CJobTree::dragLeaveEvent(QDragLeaveEvent *event)
{
    //QRect updateRect = highlightedRect;
    //highlightedRect = QRect();
    //update(updateRect);
    delCandidateItem() ;

    m_drag_countrow_for_redorder = -1 ;
    m_dropIndicatorRect = QRect() ;
    event->accept();
}

void CJobTree::delCandidateItem(void)
{
	qDebug("CJobTree : %s", __func__) ;

    if( m_p_item_candidate != NULL )
    {
    	qDebug("Delete Candidate Item 0") ;
		
    	//if( indexFromItem(m_p_item_candidate).isValid() )
    	{
	        qDebug("Delete Candidate Item 1") ;
	        //delete
	        //m_p_item_candidate->parent()->removeChild(item->parent()->indexOfChild(item);
	        delete m_p_item_candidate->parent()->takeChild(m_p_item_candidate->parent()->indexOfChild(m_p_item_candidate));

			qDebug("Delete Candidate Item 2") ;
	        m_p_item_candidate = NULL ;
			qDebug("Delete Candidate Item 3") ;
    	}
    }

}

void CJobTree::dragMoveEvent(QDragMoveEvent *event)
{
#if 1
    //QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

	// Call base class -- this shows the drop indicators
    //setDropIndicatorShown(true);

	//clear item
	/*
	for( int i = 0; i < topLevelItemCount(); ++i )
	{
	   QTreeWidgetItem *item = topLevelItem( i );

	   // Do something with item ...
	   QRect rect = visualRect(indexFromItem(item)) ;

       item->setBackground(0, QColor(255,255,255));
	}
	*/

    if (event->mimeData()->hasFormat(CToolList::itemMimeType()) || event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
    {
        event->acceptProposedAction();

        QPoint pos = event->pos() ;
        QTreeWidgetItem* item = this->itemAt(pos);

        if (item == NULL || !indexFromItem(item).isValid())
        {
            //qDebug("%s, item numm", __func__) ;
            delCandidateItem() ;

			m_dropIndicatorRect = QRect() ;
            event->ignore();
        }
        else
        {
        	//get type
        	#if 1
	    	QByteArray itemData ;
			if( event->mimeData()->hasFormat(CToolList::itemMimeType()) )		itemData = event->mimeData()->data(CToolList::itemMimeType());
			else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )	itemData = event->mimeData()->data(CJobTree::itemMimeType());
			
	        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
	        int item_from_type = -1;
	        dataStream >> item_from_type;		
			int item_to_type = item->type() ;

			qDebug("dragMoveEvent : type=%d -> type=%d", item_from_type, item_to_type) ;
			#else

			int item_type = item->type() ;
			qDebug("dragMoveEvent : item_type=%d", item_type) ;
			#endif
			
	        std::string str_candidate_text = "+ Add New Tool" ;
			if( item_to_type == JobType::JOB_TYPE_PROJECT )
			{
				str_candidate_text = "+ Add New Job" ;
			}
			
	        if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
	        {
	            str_candidate_text = "-> Move Here" ;
	        }
			
            if( !item->parent())
            {
                //top-level item
                //top-level device information node
                delCandidateItem() ;

                m_dropIndicatorRect = QRect() ;
                event->ignore();
            }
            else
            {
            	bool b_enable_insert_item = false ;
				if( (item_to_type / JobType::JOB_TYPE_PROJECT) == 1 && (item_to_type % JobType::JOB_TYPE_PROJECT) == 0  )
				{
					if( item_from_type >= JobType::JOB_TYPE_BASE && item_from_type < JobType::JOB_TYPE_BASE + 10000)
					{
						qDebug("From Base To Project") ;
						b_enable_insert_item = true ;
					}
				}
				else if( (item_to_type / JobType::JOB_TYPE_BASE) == 1 && (item_to_type % JobType::JOB_TYPE_BASE) == 0  )
				{
					if( item_from_type >= JobType::JOB_TYPE_TOOL && item_from_type < JobType::JOB_TYPE_TOOL + 10000)
					{
						qDebug("From Tool To Base") ;
						b_enable_insert_item = true ;
					}
				}

				
				qDebug("b_enable_insert_item = %d", b_enable_insert_item) ;
				
                if(  b_enable_insert_item && visualRect(indexFromItem(item)).adjusted(-1, -1, 1, 1).contains(pos, false) ) //&& item->type() == JobType::JOB_TYPE_BASE )
                {
                    //qDebug("%s, visual rect true", __func__) ;

                    //setDropIndicatorShown(true);
                    //QTreeView::dragMoveEvent(event);

                    //QModelIndex dropIndex = indexAt(event->pos());
                    DropIndicatorPosition dropIndicator = dropIndicatorPosition();

                    QModelIndex item_index = indexFromItem(item) ;
                    QRect rect = visualRect(item_index) ;
                    QRect rect_left = visualRect(item_index.sibling(item_index.row(), 0)) ;
                    QRect rect_right = visualRect(item_index.sibling(item_index.row(), columnCount() - 1)) ;

                    if( dropIndicator == QAbstractItemView::AboveItem )
                    {
                        m_dropIndicatorRect = QRect(rect_left.left(), rect_left.top(), rect_right.right() - rect_left.left(), 0) ;
                        event->accept() ;
                    }
                    else if( dropIndicator == QAbstractItemView::BelowItem )
                    {
                        m_dropIndicatorRect = QRect(rect_left.left(), rect_left.bottom(), rect_right.right() - rect_left.left(), 0) ;
                        event->accept() ;
                    }
                    else if( dropIndicator == QAbstractItemView::OnItem )
                    {
                        m_dropIndicatorRect = QRect(rect_left.left(), rect_left.top(), rect_right.right() - rect_left.left(), rect.height()) ;
                        event->accept() ;
                    }
                    else
                    {
                        m_dropIndicatorRect = QRect() ;
                    }

					//check sub at last
                    int child_count = item->childCount() ;
                    int candidate_index = -1 ;
                    for (int i = 0; i < child_count ; ++i)
                    {
                        QTreeWidgetItem* child_item = item->child(i) ;

                        if( child_item->type() == JobType::JOB_TYPE_CANDIDATE )
                        {
                            candidate_index = i ;
                            break ;
                        }
                    }

                    //last count
                    bool b_new_candidate = true ;
                    if( candidate_index >=0 )
                    {
                    	if( event->mimeData()->hasFormat(CToolList::itemMimeType()))
                    	{
                            if( candidate_index != (child_count-1) )
                            {
                                delCandidateItem() ;
                            }
                            else
                            {
                                b_new_candidate = false ;
                            }
                    	}
						else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
                    	{
                    		if( candidate_index != 0 )
                            {
                                delCandidateItem() ;
                            }
                            else
                            {
                                b_new_candidate = false ;
                            }
						}
                    }


                    if( b_new_candidate == true )
                    {
                        //new candidate item
                        QTreeWidgetItem *treeChileItem = new QTreeWidgetItem(JobType::JOB_TYPE_CANDIDATE);

                        FormCandidateTool *theWidgetItem = new FormCandidateTool;
                        QSize item_size = theWidgetItem->size() ;
                        treeChileItem->setSizeHint(0, item_size);

                        if( event->mimeData()->hasFormat(CToolList::itemMimeType()))
                        {
                            item->addChild(treeChileItem);
                        }
                        else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
                        {
                            item->insertChild( 0, treeChileItem );

                        }

                        item->setExpanded(true);

                        theWidgetItem->SetText(str_candidate_text);

                        this->setItemWidget(treeChileItem, 0, theWidgetItem);

                        //link
                        m_p_item_candidate = treeChileItem ;
                    }					

#if 0
					if( item->type() == JobType::JOB_TYPE_PROJECT )
					{
						//check sub at last
                        int child_count = item->childCount() ;
                        int candidate_index = -1 ;
                        for (int i = 0; i < child_count ; ++i)
                        {
                            QTreeWidgetItem* child_item = item->child(i) ;

                            if( child_item->type() == JobType::JOB_TYPE_CANDIDATE )
                            {
                                candidate_index = i ;
                                break ;
                            }
                        }

                        //last count
                        bool b_new_candidate = true ;
                        if( candidate_index >=0 )
                        {
                        	if( event->mimeData()->hasFormat(CToolList::itemMimeType()))
                        	{
	                            if( candidate_index != (child_count-1) )
	                            {
	                                delCandidateItem() ;
	                            }
	                            else
	                            {
	                                b_new_candidate = false ;
	                            }
                        	}
							else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
                        	{
                        		if( candidate_index != 0 )
	                            {
	                                delCandidateItem() ;
	                            }
	                            else
	                            {
	                                b_new_candidate = false ;
	                            }
							}
                        }


                        if( b_new_candidate == true )
                        {
                            //new candidate item
                            QTreeWidgetItem *treeChileItem = new QTreeWidgetItem(JobType::JOB_TYPE_CANDIDATE);

                            FormCandidateTool *theWidgetItem = new FormCandidateTool;
                            QSize item_size = theWidgetItem->size() ;
                            treeChileItem->setSizeHint(0, item_size);

                            if( event->mimeData()->hasFormat(CToolList::itemMimeType()))
                            {
                                item->addChild(treeChileItem);
                            }
                            else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
                            {
                                item->insertChild( 0, treeChileItem );

                            }

                            item->setExpanded(true);

                            theWidgetItem->SetText(str_candidate_text);

                            this->setItemWidget(treeChileItem, 0, theWidgetItem);

                            //link
                            m_p_item_candidate = treeChileItem ;
                        }
					}
                    else if( item->type() == JobType::JOB_TYPE_BASE )
                    {
                        //check sub at last
                        int child_count = item->childCount() ;
                        int candidate_index = -1 ;
                        for (int i = 0; i < child_count ; ++i)
                        {
                            QTreeWidgetItem* child_item = item->child(i) ;

                            if( child_item->type() == JobType::JOB_TYPE_CANDIDATE )
                            {
                                candidate_index = i ;
                                break ;
                            }
                        }

                        //last count
                        bool b_new_candidate = true ;
                        if( candidate_index >=0 )
                        {
                        	if( event->mimeData()->hasFormat(CToolList::itemMimeType()))
                        	{
	                            if( candidate_index != (child_count-1) )
	                            {
	                                delCandidateItem() ;
	                            }
	                            else
	                            {
	                                b_new_candidate = false ;
	                            }
                        	}
							else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
                        	{
                        		if( candidate_index != 0 )
	                            {
	                                delCandidateItem() ;
	                            }
	                            else
	                            {
	                                b_new_candidate = false ;
	                            }
							}
                        }


                        if( b_new_candidate == true )
                        {
                            //new candidate item
                            QTreeWidgetItem *treeChileItem = new QTreeWidgetItem(JobType::JOB_TYPE_CANDIDATE);

                            FormCandidateTool *theWidgetItem = new FormCandidateTool;
                            QSize item_size = theWidgetItem->size() ;
                            treeChileItem->setSizeHint(0, item_size);

                            if( event->mimeData()->hasFormat(CToolList::itemMimeType()))
                            {
                                item->addChild(treeChileItem);
                            }
                            else if( event->mimeData()->hasFormat(CJobTree::itemMimeType()) )
                            {
                                item->insertChild( 0, treeChileItem );

                            }

                            item->setExpanded(true);

                            theWidgetItem->SetText(str_candidate_text);

                            this->setItemWidget(treeChileItem, 0, theWidgetItem);

                            //link
                            m_p_item_candidate = treeChileItem ;
                        }

                    }
                    else if( item->type() == JobType::JOB_TYPE_TOOL )
                    {
                        //qDebug("Item is TOOL");

                        QTreeWidgetItem* parent = item->parent();

                        if( parent != nullptr )
                        {
                            //qDebug("Parent");

                            if( parent->type() == JobType::JOB_TYPE_BASE )
                            {
                                //qDebug("Parent is BASE");

                                int index = parent->indexOfChild(item);

                                QTreeWidgetItem* next_child = parent->child(index+1);
								QModelIndex next_child_index = QModelIndex() ;
								
                                bool b_new_candidate = true ;
                                if( next_child != nullptr )
                                {
                                    //check
                                    if( next_child->type() == JobType::JOB_TYPE_CANDIDATE )
                                    {
                                        //qDebug("Tool Candidate : Next Child is Candidate item");
                                        b_new_candidate = false ;
                                    }
                                    else
                                    {
                                        delCandidateItem() ;
                                    }

									next_child_index = indexFromItem(next_child) ;
                                }

								if( m_drag_countrow_for_redorder >= 0 )
								{
									//int cur_index = countRow(item_index) ;
									//int next_index = countRow(next_child_index) ;
									int cur_index = item_index.row() ;
									int next_index = next_child_index.row() ;
									
	                                if( cur_index > 0 )
									{
										//qDebug("%s : m_drag_countrow_for_redorder(%d), cur_index(%d), next_index(%d)", __func__, m_drag_countrow_for_redorder, cur_index, next_index) ;
										
										//The position to move is now your own position.
										if( m_drag_countrow_for_redorder == cur_index || m_drag_countrow_for_redorder == next_index)
										{
											b_new_candidate = false;

											delCandidateItem() ;
										}
									}
								}

                                if( b_new_candidate )
                                {
                                    //new candidate item
                                    QTreeWidgetItem *treeChileItem = new QTreeWidgetItem(JobType::JOB_TYPE_CANDIDATE);
                                    //parent->addChild(treeChileItem);
                                    //parent->insertChild( index+1, treeChileItem );
                                    //parent->insertChild(index+1, treeChileItem);
                                    //item->setExpanded(true);

                                    FormCandidateTool *theWidgetItem = new FormCandidateTool;
                                    QSize item_size = theWidgetItem->size() ;
                                    treeChileItem->setSizeHint(0, item_size);

                                    theWidgetItem->SetText(str_candidate_text);

                                    parent->insertChild( index+1, treeChileItem );

                                    this->setItemWidget(treeChileItem, 0, theWidgetItem);

                                    //link
                                    m_p_item_candidate = treeChileItem ;

                                    //parent->insertChild(index-1, child);
                                    //delCandidateItem() ;
                                }

                            }
                        }

#if 0
                        QTreeWidgetItem* parent = item->parent();

                        if( parent != nullptr )
                        {
                            if( parent->type() == JobType::JOB_TYPE_BASE )
                            {
                                int index = parent->indexOfChild(item);

                                //qDebug("Tool Candidate : index=%d", index);

                                QTreeWidgetItem* next_child = parent->takeChild(index+1);

                                bool b_new_candidate = true ;
                                if( next_child != nullptr )
                                {
                                    //check
                                    if( next_child->type() == JobType::JOB_TYPE_CANDIDATE )
                                    {
                                        qDebug("Tool Candidate : Next Child is Candidate item");
                                        b_new_candidate = false ;
                                    }
                                    else
                                    {
                                        delCandidateItem() ;
                                    }
                                }

#if 0
                                if( b_new_candidate )
                                {
                                    //new candidate item
                                    QTreeWidgetItem *treeChileItem = new QTreeWidgetItem(parent, JobType::JOB_TYPE_CANDIDATE);
                                    parent->addChild(treeChileItem);
                                    //parent->insertChild( index+1, treeChileItem );
                                    //parent->insertChild(index+1, treeChileItem);
                                    //item->setExpanded(true);

                                    FormCandidateTool *theWidgetItem = new FormCandidateTool;
                                    QSize item_size = theWidgetItem->size() ;
                                    treeChileItem->setSizeHint(0, item_size);

                                    this->setItemWidget(treeChileItem, 0, theWidgetItem);

                                    //link
                                    m_p_item_candidate = treeChileItem ;

                                    //parent->insertChild(index-1, child);
                                    //delCandidateItem() ;
                                }
#endif
                            }
                        }
#endif
                    }
                    else
                    {
                        delCandidateItem() ;
                    }
#endif

                    /*
                    bool bAbove = false; // boolean for the case when you are above an item

                    if (!dropIndex.parent().isValid() && dropIndex.row() != -1)
                    {
                        switch (dropIndicator)
                        {
                        case QAbstractItemView::AboveItem:
                            // manage a boolean for the case when you are above an item
                            m_dropIndicatorRect = QRect(rect_left.left(), rect_left.top(), rect_right.right() - rect_left.left(), 0) ;
                            bAbove = true;
                            break;
                        case QAbstractItemView::BelowItem:
                            // something when being below an item
                            break;
                        case QAbstractItemView::OnItem:
                            // you're on an item, maybe add the current one as a child
                            break;
                        case QAbstractItemView::OnViewport:
                            // you are not on your tree
                            break;
                        }

                        if(bAbove) // you are above an item
                        {
                            // manage this case
                        }
                    }
                    */

                    //item->setBackground(0, QColor(255,0,0));
                    event->setDropAction(Qt::CopyAction);
                    event->accept();
                }
                else
                {
                    //qDebug("%s, visual rect false", __func__) ;
                    delCandidateItem() ;

                    m_drag_countrow_for_redorder = -1 ;
                    m_dropIndicatorRect = QRect() ;
                    event->ignore();
                }
            }
        }

#if 0
        //item = self.itemAt(pos)

        //event->acceptProposedAction();
        //QTreeView::dragMoveEvent(event);

        qDebug("CJobTree : %s", __func__) ;
		//QPixmap pixmap(viewport()->visibleRegion().boundingRect().size());
		
		DropIndicatorPosition position = dropIndicatorPosition();

        qDebug("position = %d", position) ;
		
        //highlightedRect = targetSquare(event->pos());
        //event->setDropAction(Qt::MoveAction);
        event->setDropAction(Qt::MoveAction);
        event->accept();

        //QTreeView::dragMoveEvent(event);
#endif
    }
    else
    {
        //highlightedRect = QRect();
        delCandidateItem() ;

        m_drag_countrow_for_redorder = -1 ;
		m_dropIndicatorRect = QRect() ;
        event->ignore();
    }

    //update(updateRect);
#else
    setDropIndicatorShown(true);
    QTreeView::dragMoveEvent(event);

#endif

    this->viewport()->update();
}
#endif

#if 1
void CJobTree::paintEvent( QPaintEvent * event )
{
    QPainter painter(viewport());
    drawTree(&painter, event->region());
	
    paintDropIndicator(&painter) ;
}
#endif

void CJobTree::paintDropIndicator(QPainter* painter)
{
    //qDebug("CJobTree : %s", __func__) ;

    //if( this->state() == QAbstractItemView::DraggingState )
    if( m_dropIndicatorRect.isValid() )
    {
        //qDebug("CJobTree : %s - paint", __func__) ;

        QStyleOption opt ;
        opt.init(this);
        opt.rect = m_dropIndicatorRect ;

        QRect rect = opt.rect ;

		if(rect.height() == 0)
        {
            painter->drawEllipse(opt.rect.topLeft(), 3, 3);
            painter->drawLine(QPoint(opt.rect.topLeft().x()+3, opt.rect.topLeft().y()), opt.rect.topRight());
        } else {
            painter->drawRoundedRect(opt.rect, 5, 5);
        }
    }
}

void CJobTree::startDrag(Qt::DropActions /*supportedActions*/)
{
    QTreeWidgetItem *item = currentItem();

    //m_drag_countrow_for_redorder = countRow(indexFromItem(item)) ;
    QModelIndex index = indexFromItem(item) ;
	m_drag_countrow_for_redorder = index.row() ;

	qDebug("%s : set m_drag_countrow_for_redorder = %d", __func__, m_drag_countrow_for_redorder) ;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap pixmap(viewport()->visibleRegion().boundingRect().size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
	painter.setOpacity(0.5);
    for(QModelIndex index: selectedIndexes()){
        painter.drawPixmap(visualRect(index), viewport()->grab(visualRect(index)));
    }

#if 1
    QVariant item_user_data = item->data(0, Qt::UserRole+1) ;

    //dataStream << str_text << type ;
    dataStream << item_user_data.toString() ;
	
#endif

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(CJobTree::itemMimeType(), itemData);

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


int CJobTree::countRow(const QModelIndex & index)
{
    int count  = -1;

    if(index.isValid())
    {
        count = (index.row() + 1) + countRow(index.parent());

        const QModelIndex parent = index.parent();
        if(parent.isValid())
        {
            for(int r = 0; r < index.row(); ++r)
                count += model()->rowCount(parent.child(r, 0));
        }
    }

    return count;
}

