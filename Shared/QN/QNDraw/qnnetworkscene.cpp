/**
 ** @file qnnetworkscene.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-08
 **
 ** Copyright 2016 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "qnnetworkscene.h"
#include "qnnetworkmodel.h"
#include "qnnodeicon.h"
#include "qnmodel.h"
#include "qntextbox.h"
#include "qnnodeitem.h"
#include "qntextboxitem.h"
#include "qnedgeitem.h"
#include "qnmodelitem.h"
#include "qnedgestyle.h"
#include "qndrawconst.h"
#include <QLineF>


QNModelItemPtr QNNetworkScene::determineParent(QPointF aPoint, QGraphicsItemPtr aItem){
    QNModelItemPtr parent = nullptr;
    QGraphicsItemList list = this->items(aPoint, Qt::ContainsItemBoundingRect);

    //scan list to find a QNModelItem
    int size = list.size();
    for(int i=0; i<size; i++){
        QGraphicsItemPtr item = list.at(i);
        if(item->type() == QNModelItem::Type && item != aItem){
            parent = qgraphicsitem_cast<QNModelItemPtr>(item);
            break;
        }
    }

    return parent;
}

void QNNetworkScene::addItemToList(QGraphicsItemPtr aItem){
    if(aItem != nullptr){
        if(this->itemList.isNull())
            this->itemList.reset(new QGraphicsItemList());

        this->itemList->push_back(aItem);

        //add only if top-level item
        if(aItem->parentItem() == nullptr)
            this->addItem(aItem);
    }
}

QNNodeItemPtr QNNetworkScene::getNode(QString aId){
    if(this->itemList.isNull() || this->itemList->isEmpty())
        return nullptr;

    int size = this->itemList->size();
    for(int i=0; i<size; i++){
        QGraphicsItemPtr tmp = this->itemList->at(i);
        if(tmp != nullptr && dynamic_cast<QNNodeItemPtr>(tmp)){
            QNNodeItemPtr node = dynamic_cast<QNNodeItemPtr>(tmp);
            if(node->getNode()->getId() == aId)
                return node;
        }
    }

    return nullptr;
}

void QNNetworkScene::addNode(QNNodeIconPtr aNode, QGraphicsItemPtr aParent){
    if(this->network != nullptr){
        QNNodeItemPtr n = new QNNodeItem(aNode, this->oneNodeSelectedCtxMenu, this->styleMgr, aParent);
        this->addItemToList(n);
    }
}

void QNNetworkScene::removeNode(QNNodeItemPtr aNodeItem){
    if(this->itemList->contains(aNodeItem)){
        this->itemList->removeOne(aNodeItem);
        if(aNodeItem->parentItem() != nullptr)
            aNodeItem->setParentItem(nullptr);
        else
            this->removeItem(aNodeItem);

        //remove all associated edges
        QNEdgeItemListPtr edges =  aNodeItem->getEdges();
        if(edges != nullptr){
            while(!edges->isEmpty()){
                QNEdgeItemPtr a = edges->back();
                this->removeEdge(a);
            }
        }

        //remove nodeItem, nodeIcon and node;
        QNNodeIconPtr nodeIcon = aNodeItem->getNode();
        QNNodePtr node = nodeIcon->getData();
        this->network->removeIcon(nodeIcon);
        this->network->getNet()->removeNode(node);
        delete aNodeItem;
    }
}

void QNNetworkScene::addEdge(QNEdgeStylePtr aEdge){
    if(aEdge != nullptr){
        QString src = aEdge->getData()->getSrc()->getId();
        QString dst = aEdge->getData()->getDst()->getId();
        if(src != dst){
            QNNodeItemPtr srcNode = this->getNode(src);
            QNNodeItemPtr dstNode = this->getNode(dst);

            //determine parent for the edge
            QGraphicsItemPtr parent = (srcNode->parentItem() == dstNode->parentItem())? srcNode->parentItem(): nullptr;

            QNEdgeItemPtr a = new QNEdgeItem(srcNode, dstNode, aEdge, this->oneEdgeSelectedCtxMenu, this->styleMgr, parent);
            srcNode->addEdge(a);
            dstNode->addEdge(a);
            this->addItemToList(a);
        }
    }
}

void QNNetworkScene::removeEdge(QNEdgeItemPtr aEdgeItem){
    //remove edgeItem, edgeStyle and edge
    if(this->itemList->contains(aEdgeItem)){
        this->itemList->removeOne(aEdgeItem);
        if(aEdgeItem->parentItem() != nullptr)
            aEdgeItem->setParentItem(nullptr);
        else
            this->removeItem(aEdgeItem);

        QNEdgeStylePtr edgeStyle = aEdgeItem->getEdge();
        QNEdgePtr edge = edgeStyle->getData();
        this->network->removeEdge(edgeStyle);
        this->network->getNet()->removeEdge(edge);

        //clean edge reference in nodes
        aEdgeItem->getSrcNode()->removeEdge(aEdgeItem);
        aEdgeItem->getDstNode()->removeEdge(aEdgeItem);
        delete aEdgeItem;
    }
}

void QNNetworkScene::addModel(QNModelPtr aModel, QGraphicsItemPtr aParent){
    QNModelPtr actualModel = nullptr;
    if(aModel == nullptr){
        //this is the whole network case
        actualModel = this->network;

        //check if every node has a corresponding icon
        QStringList keys = this->network->getNet()->getAllNodesKeys();
        if(!keys.isEmpty()){
            foreach(QString k, keys){
                QNNodePtr n = this->network->getNet()->getNode(k);
                if(n->getIcon() == nullptr){
                    //add missing icons
                    QNNodeIconPtr nodeIcon = new QNNodeIcon(this->network);
                    nodeIcon->setData(n);
                    n->setIcon(nodeIcon);
                    nodeIcon->setId(n->getId());
                    nodeIcon->setName(n->getId());
                    nodeIcon->setFont(new QNFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR));
                    nodeIcon->setColor(DEFAULT_BG_COLOR);
                    nodeIcon->setOutlineColor(DEFAULT_FG_COLOR);
                    nodeIcon->setOutlineWidth(DEFAULT_WIDTH);
                    nodeIcon->setPosition(new QNPosition(0,0,0,0));
                    this->network->addIcon(nodeIcon);
                }
            }
        }
    }
    else {
        actualModel = aModel;
    }


    //draw nodes and models
    QStringList keys = actualModel->getAllIconsKeys();
    foreach(QString k, keys){
        QNIconPtr icon = actualModel->getIcon(k);
        if(dynamic_cast<QNNodeIconPtr>(icon)){
            QNNodeIconPtr node = dynamic_cast<QNNodeIconPtr>(icon);
            this->addNode(node, aParent);
        }
        else if(dynamic_cast<QNModelPtr>(icon)){
            QNModelPtr model = dynamic_cast<QNModelPtr>(icon);
            QNModelItemPtr modelItem = new QNModelItem(model, this->oneModelSelectedCtxMenu, this->styleMgr, aParent);
            this->addModel(model, modelItem);
            this->addItemToList(modelItem);
        }
        else{
            //not supported: ignored
        }
    }

    //draw textboxes
    QNTextBoxListPtr tblist = actualModel->getTextboxes();
    if(tblist != nullptr){
        int size = tblist->size();
        for(int i=0; i<size; i++){
            QNTextBoxPtr textbox = tblist->at(i);
            this->addTextbox(textbox, aParent);
        }
    }

    //draw edges
    if(aModel == nullptr){//this is the whole network case
        //check if all edges have a corresponding icon
        QStringList keys = this->network->getNet()->getAllEdgesKeys();
        if(!keys.isEmpty()){
            for(QString k: keys){
                QNEdgePtr edge = this->network->getNet()->getEdge(k);
                if(edge->getIcon() == nullptr){
                    //create the missing icon
                    QNEdgeStylePtr a = new QNEdgeStyle(edge);
                    edge->setIcon(a);
                    this->network->addEdge(a);
                }
                this->addEdge(edge->getIcon());
            }
        }
    }
}

void QNNetworkScene::removeModel(QNModelItemPtr aModelItem){
    if(this->itemList->contains(aModelItem)){
        this->itemList->removeOne(aModelItem);
        if(aModelItem->parentItem() != nullptr)
            aModelItem->setParentItem(nullptr);
        else
            this->removeItem(aModelItem);

        //remove all contained items
        QList<QGraphicsItemPtr> children = aModelItem->childItems();
        int size = children.size();
        for(int i=0; i<size; i++){
            QGraphicsItemPtr child = children.at(i);
            if (child->type() == QNTextBoxItem::Type){
                this->removeTextbox(qgraphicsitem_cast<QNTextBoxItemPtr>(child));
            }
            else if (child->type() == QNModelItem::Type){
                this->removeModel(qgraphicsitem_cast<QNModelItemPtr>(child));
            }
            else if (child->type() == QNNodeItem::Type){
                this->removeNode(qgraphicsitem_cast<QNNodeItemPtr>(child));
            }
        }

        //remove model
        QNModelPtr model = aModelItem->getModel();
        this->network->removeIcon(model);
        delete aModelItem;
    }
}

void QNNetworkScene::addTextbox(QNTextBoxPtr aTextbox, QGraphicsItemPtr aParent){
    QNTextBoxItemPtr t = new QNTextBoxItem(aTextbox, this->oneTextboxSelectedCtxMenu, this->styleMgr, aParent);
    this->addItemToList(t);
}

void QNNetworkScene::removeTextbox(QNTextBoxItemPtr aTextbox){
    if(this->itemList->contains(aTextbox)){
        this->itemList->removeOne(aTextbox);
        if(aTextbox->parentItem() != nullptr)
            aTextbox->setParentItem(nullptr);
        else
            this->removeItem(aTextbox);

        //remove textbox
        QNTextBoxPtr textB = aTextbox->getTextbox();
        this->network->removeTextBox(textB);
        delete aTextbox;
    }
}

void QNNetworkScene::mousePressEvent(QGraphicsSceneMouseEventPtr aEvent){
    if (aEvent->button() == Qt::LeftButton){
        switch (this->mode) {
            case Mode::InsertEdge:{
                this->tempLine = new QGraphicsLineItem(QLineF(aEvent->scenePos(), aEvent->scenePos()));
                this->tempLine->setZValue(1000);

                QPen pen;
                pen.setWidth(DEFAULT_WIDTH);
                QColor outColor = QColor("#" + QString(DEFAULT_FG_COLOR));
                pen.setColor(outColor);
                this->tempLine->setPen(pen);
                this->addItem(tempLine);
                break;
            }
            default:
                break;
        }
    }
    QGraphicsScene::mousePressEvent(aEvent);
}

void QNNetworkScene::mouseMoveEvent(QGraphicsSceneMouseEventPtr aEvent){
    switch (this->mode) {
        case Mode::InsertEdge:{
            if(this->tempLine != nullptr){
                QLineF newLine(this->tempLine->line().p1(), aEvent->scenePos());
                this->tempLine->setLine(newLine);
            }
            break;
        }
        default:
            QGraphicsScene::mouseMoveEvent(aEvent);
            break;
    }
}

void QNNetworkScene::mouseReleaseEvent(QGraphicsSceneMouseEventPtr aEvent){
    QGraphicsScene::mouseReleaseEvent(aEvent);

    switch (this->mode) {
        case Mode::InsertEdge:{
            if(this->tempLine != nullptr){
                QPointF start = this->tempLine->line().p1();
                QGraphicsItemList startItems = this->items(start, Qt::ContainsItemBoundingRect);
                if (startItems.count() && startItems.first() == this->tempLine)
                    startItems.removeFirst();

                QPointF end = this->tempLine->line().p2();
                QGraphicsItemList endItems = this->items(end, Qt::ContainsItemBoundingRect);
                if (endItems.count() && endItems.first() == this->tempLine)
                    endItems.removeFirst();

                this->removeItem(this->tempLine);
                delete this->tempLine;
                this->tempLine = nullptr;

                if (startItems.count() > 0 && endItems.count() > 0 &&
                    startItems.first()->type() == QNNodeItem::Type &&
                    endItems.first()->type() == QNNodeItem::Type &&
                    startItems.first() != endItems.first()) {
                    this->newEdge(qgraphicsitem_cast<QNNodeItemPtr>(startItems.first()),
                                  qgraphicsitem_cast<QNNodeItemPtr>(endItems.first()));
                }
                else
                    this->update();
            }
            break;
        }
        default:
            this->manageSelection();
            break;
    }

    this->mode = Mode::Navigation;
}

void QNNetworkScene::contextMenuEvent(QGraphicsSceneContextMenuEventPtr aEvent){
    this->mousePos = aEvent->scenePos();

    if(!this->manageCtxMenuClick(aEvent))
        QGraphicsScene::contextMenuEvent(aEvent);
}

void QNNetworkScene::deleteItems(){
    bool isMod = false;
    foreach (QGraphicsItemPtr item, this->selectedItems()) {
        if(this->itemList->contains(item)){
            item->setSelected(false);
            if(item->type() == QNEdgeItem::Type){
                this->removeEdge(qgraphicsitem_cast<QNEdgeItemPtr>(item));
            }
            else if (item->type() == QNTextBoxItem::Type){
                this->removeTextbox(qgraphicsitem_cast<QNTextBoxItemPtr>(item));
            }
            else if (item->type() == QNModelItem::Type){
                this->removeModel(qgraphicsitem_cast<QNModelItemPtr>(item));
            }
            else if (item->type() == QNNodeItem::Type){
                this->removeNode(qgraphicsitem_cast<QNNodeItemPtr>(item));
            }

            isMod = true;
        }
    }

    if(isMod)
        emit this->modified();

    this->clearSelection();
    this->manageSelection();
}

void QNNetworkScene::bringToFront(){
    QList<QGraphicsItemPtr> items = this->selectedItems();
    foreach(QGraphicsItemPtr selectedItem, items){
        if(selectedItem != nullptr){
            QList<QGraphicsItemPtr> overlapItems = selectedItem->collidingItems();

            double zValue = selectedItem->zValue();
            foreach (QGraphicsItemPtr item, overlapItems) {
                if (item != nullptr &&
                    item->zValue() >= zValue && (item->type() == QNNodeItem::Type ||
                                                 item->type() == QNModelItem::Type ||
                                                 item->type() == QNTextBoxItem::Type ||
                                                 item->type() == QNEdgeItem::Type))
                    zValue = item->zValue() + 0.1;
            }
            selectedItem->setZValue(zValue);
        }
    }
}

void QNNetworkScene::sendToBack(){
    QList<QGraphicsItemPtr> items = this->selectedItems();
    foreach(QGraphicsItemPtr selectedItem, items){
        if(selectedItem != nullptr){
            QList<QGraphicsItemPtr> overlapItems = selectedItem->collidingItems();

            double zValue = selectedItem->zValue();
            foreach (QGraphicsItemPtr item, overlapItems) {
                if (item != nullptr &&
                    item->zValue() <= zValue && (item->type() == QNNodeItem::Type ||
                                                 item->type() == QNModelItem::Type ||
                                                 item->type() == QNTextBoxItem::Type ||
                                                 item->type() == QNEdgeItem::Type))
                    zValue = item->zValue() - 0.1;
            }
            selectedItem->setZValue(zValue);
        }
    }
}

void QNNetworkScene::newNode(){
    QNModelItemPtr parent = this->determineParent(this->mousePos);
    if(parent != nullptr)
        this->mousePos -= parent->scenePos();

    QPoint topLeft(this->mousePos.x()-NODE_DEFAULT_W/2, this->mousePos.y()-NODE_DEFAULT_H/2);
    QPoint bottomRight(topLeft.x()+NODE_DEFAULT_W, topLeft.y()+NODE_DEFAULT_H);

    //create a new node
    QNNodePtr node = new QNNode(this->network->getNet());
    node->initId();
    this->network->getNet()->addNode(node);

    //create a new node icon
    QNModelPtr container = nullptr;
    if(parent != nullptr)
        container = parent->getModel();
    else
        container = this->network;

    QNNodeIconPtr nodeIcon = new QNNodeIcon(container);
    nodeIcon->setData(node);
    node->setIcon(nodeIcon);
    nodeIcon->setId(node->getId());
    nodeIcon->setName("New Node");
    QNFont* f = new QNFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR);
    f->setAlign(QNAlignment::leftAlign);
    nodeIcon->setFont(f);
    nodeIcon->setColor(DEFAULT_BG_COLOR);
    nodeIcon->setOutlineColor(DEFAULT_FG_COLOR);
    nodeIcon->setOutlineWidth(DEFAULT_WIDTH);
    nodeIcon->setPosition(new QNPosition(topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y()));
    container->addIcon(nodeIcon);

    //create a new node item
    QNNodeItemPtr nodeItem = new QNNodeItem(nodeIcon, this->oneNodeSelectedCtxMenu, this->styleMgr, parent);
    this->addItemToList(nodeItem);

    //restore mouse pos to the center of the scene
    this->mousePos = QPointF(0,0);
    emit this->modified();
    this->clearSelection();
    nodeItem->setSelected(true);
    this->manageSelection();
}

void QNNetworkScene::newModel(){
    QNModelItemPtr parent = this->determineParent(this->mousePos);
    if(parent != nullptr)
        this->mousePos -= parent->scenePos();

    int w = (parent != nullptr)? MODEL_DEFAULT_W/2: MODEL_DEFAULT_W;
    int h = (parent != nullptr)? MODEL_DEFAULT_H/2: MODEL_DEFAULT_H;
    QPoint topLeft(this->mousePos.x()-w/2, this->mousePos.y()-h/2);
    QPoint bottomRight(topLeft.x()+w, topLeft.y()+h);

    //create a new model
    QNModelPtr container = nullptr;
    if(parent != nullptr)
        container = parent->getModel();
    else
        container = this->network;

    QNModelPtr model = new QNModel(container);
    model->initId();
    model->setName("New Model");
    QNFont* f = new QNFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR);
    f->setAlign(QNAlignment::leftAlign);
    model->setFont(f);
    model->setColor(DEFAULT_BG_COLOR);
    model->setOutlineColor(DEFAULT_FG_COLOR);
    model->setOutlineWidth(DEFAULT_WIDTH);
    model->setPosition(new QNPosition(topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y()));
    container->addIcon(model);

    //create a new model item
    QNModelItemPtr modelItem = new QNModelItem(model, this->oneModelSelectedCtxMenu, this->styleMgr, parent);
    this->addItemToList(modelItem);

    //restore mouse pos to the center of the scene
    this->mousePos = QPointF(0,0);
    emit this->modified();
    this->clearSelection();
    modelItem->setSelected(true);
    this->manageSelection();
}

void QNNetworkScene::newTextbox(){
    QNModelItemPtr parent = this->determineParent(this->mousePos);
    if(parent != nullptr)
        this->mousePos -= parent->scenePos();

    QPoint topLeft(this->mousePos.x()-TEXTBOX_DEFAULT_W/2, this->mousePos.y()-TEXTBOX_DEFAULT_H/2);
    QPoint bottomRight(topLeft.x()+TEXTBOX_DEFAULT_W, topLeft.y()+TEXTBOX_DEFAULT_H);

    //create new textbox
    QNModelPtr container = nullptr;
    if(parent != nullptr)
        container = parent->getModel();
    else
        container = this->network;

    QNTextBoxPtr textb = new QNTextBox(container);
    textb->setCaption("New Note");
    QNFont* f = new QNFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR);
    f->setAlign(QNAlignment::leftAlign);
    textb->setFont(f);
    textb->setPosition(new QNPosition(topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y()));
    container->addTextBox(textb);

    //create new textbox item
    QNTextBoxItemPtr textbItem = new QNTextBoxItem(textb, this->oneTextboxSelectedCtxMenu, this->styleMgr, parent);
    this->addItemToList(textbItem);

    //restore mouse pos to the center of the scene
    this->mousePos = QPointF(0,0);
    emit this->modified();
    this->clearSelection();
    textbItem->setSelected(true);
    this->manageSelection();
}

void QNNetworkScene::newEdgeMode(){
    this->mode = Mode::InsertEdge;
    //    QGraphicsSceneMouseEvent press(QEvent::GraphicsSceneMousePress);
    //    press.setScenePos(this->mousePos);
    //    press.setButton(Qt::MouseButton::LeftButton);
    //    press.setButtons(Qt::MouseButton::LeftButton);
    //    this->mousePressEvent(&press);
}

void QNNetworkScene::newCycle(){
    QGraphicsItemList items = this->selectedItems();
    int size = items.size();
    for(int i=0; i<size; i++){
        QNNodeItemPtr src = qgraphicsitem_cast<QNNodeItemPtr>(items.at(i));
        if(src != nullptr){
            for(int j=0; j<size; j++){
                QNNodeItemPtr dst = qgraphicsitem_cast<QNNodeItemPtr>(items.at(j));
                if(dst != nullptr && src != dst)
                    this->newEdge(src, dst);
            }
        }
    }
}

void QNNetworkScene::selectAll(){
    if(!this->itemList.isNull()){
        QNNodeItemList nodes;
        QNModelItemList models;
        QNTextBoxItemList textboxes;
        QNEdgeItemList edges;
        int size = this->itemList->size();
        for(int i=0; i<size; i++){
            QGraphicsItemPtr it = this->itemList->at(i);
            it->setSelected(true);
            if(it->type() == QNEdgeItem::Type)
                edges.push_back(qgraphicsitem_cast<QNEdgeItemPtr>(it));
            else if(it->type() == QNNodeItem::Type)
                nodes.push_back(qgraphicsitem_cast<QNNodeItemPtr>(it));
            else if(it->type() == QNModelItem::Type)
                models.push_back(qgraphicsitem_cast<QNModelItemPtr>(it));
            else if(it->type() == QNTextBoxItem::Type)
                textboxes.push_back(qgraphicsitem_cast<QNTextBoxItemPtr>(it));
        }

        emit this->itemsSelected(nodes, models, textboxes, edges);
    }
}

void QNNetworkScene::toggleShowContent(){
    QGraphicsItemList items = this->selectedItems();
    int size = items.size();
    for(int i=0; i<size; i++){
        QGraphicsItemPtr it = items.at(i);
        QGraphicsItemPtr parent = it->parentItem();
        if(parent == nullptr || !items.contains(parent)){
            if(it->type() == QNNodeItem::Type)
                qgraphicsitem_cast<QNNodeItemPtr>(it)->toggleShowContent();
            else if(it->type() == QNModelItem::Type)
                qgraphicsitem_cast<QNModelItemPtr>(it)->toggleShowContent();
            else if(it->type() == QNTextBoxItem::Type)
                qgraphicsitem_cast<QNTextBoxItemPtr>(it)->toggleShowContent();
        }
    }
}


void QNNetworkScene::hideAllContents(){
    QGraphicsItemList items = this->selectedItems();
    int size = items.size();
    for(int i=0; i<size; i++){
        QGraphicsItemPtr it = items.at(i);
        QGraphicsItemPtr parent = it->parentItem();
        if(parent == nullptr || !items.contains(parent)){
            if(it->type() == QNNodeItem::Type)
                qgraphicsitem_cast<QNNodeItemPtr>(it)->setShowContent(false);
            else if(it->type() == QNModelItem::Type)
                qgraphicsitem_cast<QNModelItemPtr>(it)->setShowContent(false);
            else if(it->type() == QNTextBoxItem::Type)
                qgraphicsitem_cast<QNTextBoxItemPtr>(it)->setShowContent(false);
        }
    }
}

void QNNetworkScene::showAllContents(){
    QGraphicsItemList items = this->selectedItems();
    int size = items.size();
    for(int i=0; i<size; i++){
        QGraphicsItemPtr it = items.at(i);
        if(it->type() == QNNodeItem::Type)
            qgraphicsitem_cast<QNNodeItemPtr>(it)->setShowContent(true);
        else if(it->type() == QNModelItem::Type)
            qgraphicsitem_cast<QNModelItemPtr>(it)->setShowContent(true);
        else if(it->type() == QNTextBoxItem::Type)
            qgraphicsitem_cast<QNTextBoxItemPtr>(it)->setShowContent(true);
    }
}

QNNetworkScene::QNNetworkScene(QNStyleManagerPtr aStyleMgr, QObjectPtr aParent): QGraphicsScene(aParent){
    this->mode = Mode::Navigation;
    this->tempLine = nullptr;
    this->sceneCtxMenu = new QMenu();
    this->oneNodeSelectedCtxMenu = new QMenu();
    this->manyNodesSelectedCtxMenu = new QMenu();
    this->oneEdgeSelectedCtxMenu = new QMenu();
    this->manyEdgesSelectedCtxMenu = new QMenu();
    this->oneModelSelectedCtxMenu = new QMenu();
    this->manyModelsSelectedCtxMenu = new QMenu();
    this->oneTextboxSelectedCtxMenu = new QMenu();
    this->manyTextboxesSelectedCtxMenu = new QMenu();
    this->manySelectedCtxMenu = new QMenu();
    this->network = nullptr;
    this->itemList.reset();
    this->styleMgr = aStyleMgr;
}

QNNetworkScene::~QNNetworkScene(){
    this->tempLine = nullptr;
    this->network = nullptr;
    this->styleMgr = nullptr;

    if(!this->itemList.isNull()){
        this->itemList.reset();
    }

    delete this->sceneCtxMenu; this->sceneCtxMenu = nullptr;
    delete this->oneNodeSelectedCtxMenu; this->oneNodeSelectedCtxMenu = nullptr;
    delete this->manyNodesSelectedCtxMenu; this->manyNodesSelectedCtxMenu = nullptr;
    delete this->oneEdgeSelectedCtxMenu; this->oneEdgeSelectedCtxMenu = nullptr;
    delete this->manyEdgesSelectedCtxMenu; this->manyEdgesSelectedCtxMenu = nullptr;
    delete this->oneModelSelectedCtxMenu; this->oneModelSelectedCtxMenu = nullptr;
    delete this->manyModelsSelectedCtxMenu; this->manyModelsSelectedCtxMenu = nullptr;
    delete this->oneTextboxSelectedCtxMenu; this->oneTextboxSelectedCtxMenu = nullptr;
    delete this->manyTextboxesSelectedCtxMenu; this->manyTextboxesSelectedCtxMenu = nullptr;
    delete this->manySelectedCtxMenu; this->manySelectedCtxMenu = nullptr;
}

void QNNetworkScene::newEdge(QNNodeItemPtr aSource, QNNodeItemPtr aDest){
    if (aSource != nullptr && aDest != nullptr && aSource != aDest &&
        aSource->isShowingContent() && aDest->isShowingContent() &&
        this->itemList->contains(aSource) && this->itemList->contains(aDest)) {
        //start node
        QNNodeIconPtr sourceIcon = aSource->getNode();
        QNNodePtr sourceNode = sourceIcon->getData();

        //end node
        QNNodeIconPtr destIcon = aDest->getNode();
        QNNodePtr destNode = destIcon->getData();

        //create Edge
        sourceNode->addSuccessor(destNode);
        destNode->addAncestor(sourceNode);
        QNEdgePtr edge = new QNEdge(sourceNode, destNode, this->network->getNet());
        this->network->getNet()->addEdge(edge);

        //create EdgeStyle
        QNEdgeStylePtr edgeStyle = new QNEdgeStyle(edge);
        this->network->addEdge(edgeStyle);

        //determine parent for the edge
        QGraphicsItemPtr parent = (aSource->parentItem() == aDest->parentItem())? aSource->parentItem(): nullptr;

        //create EdgeItem
        QNEdgeItemPtr edgeItem = new QNEdgeItem(aSource, aDest, edgeStyle, this->oneEdgeSelectedCtxMenu, this->styleMgr, parent);
        aSource->addEdge(edgeItem);
        aDest->addEdge(edgeItem);
        this->addItemToList(edgeItem);
        this->clearSelection();
        this->clearFocus();
        emit this->modified();
        this->clearSelection();
        edgeItem->setSelected(true);
        this->manageSelection();
    }
}

void QNNetworkScene::manageMoveInside(){
    //do it for every selected item
    foreach (QGraphicsItemPtr item, this->selectedItems()) {
        int t = item->type();
        if(t == QNNodeItem::Type || t == QNModelItem::Type || t == QNTextBoxItem::Type){
            QNModelItemPtr oldParent = qgraphicsitem_cast<QNModelItemPtr>(item->parentItem());
            QNModelItemPtr newParent = this->determineParent(item->scenePos(), item);
            if(newParent != item && newParent != oldParent && (newParent == nullptr || newParent->isShowingContent())){
                QNModelPtr newModel = (newParent) ? newParent->getModel() : this->network;
                QNModelPtr oldModel = (oldParent) ? oldParent->getModel() : this->network;

                //get new position
                QPointF point;
                if(newParent != nullptr)
                    point = item->scenePos() - newParent->scenePos();
                else
                    point = item->scenePos();

                switch (t) {
                    case QNNodeItem::Type:{
                        QNNodeItemPtr node = qgraphicsitem_cast<QNNodeItemPtr>(item);

                        //move node to new model
                        if(oldModel->moveIcon(node->getNode(), newModel)){
                            //move node item
                            this->removeItem(node);
                            node->setParentItem(newParent);
                            if(newParent == nullptr){
                                this->addItem(node);
                                node->setZValue(0.2);
                            }
                            else
                                node->setZValue(newParent->zValue() + 0.2);

                            QNEdgeItemListPtr edges = node->getEdges();
                            if(edges != nullptr){
                                int size = edges->size();
                                for(int i=0; i<size; i++){
                                    QNEdgeItemPtr e = edges->at(i);
                                    QGraphicsItemPtr edgeParent = e->parentItem();
                                    QGraphicsItemPtr edgeNewParent = (e->getSrcNode()->parentItem() == e->getDstNode()->parentItem())? e->getSrcNode()->parentItem(): nullptr;
                                    if(edgeParent != edgeNewParent){
                                        //new parent
                                        if(edgeParent == nullptr && edgeNewParent != nullptr){
                                            //enter a model from the main scene
                                            this->removeItem(e);
                                            e->setParentItem(edgeNewParent);
                                        }
                                        else if(edgeParent != nullptr && edgeNewParent == nullptr){
                                            //return to main scene
                                            e->setParentItem(edgeNewParent);
                                            this->addItem(e);
                                        }
                                        else{
                                            //from a model to another
                                            e->setParentItem(edgeNewParent);
                                        }

                                        e->setZValue(0.1);
                                        e->updateEdgePos();
                                    }
                                }
                            }

                            node->setPos(point);
                        }

                        break;
                    }
                    case QNModelItem::Type:{
                        QNModelItemPtr model = qgraphicsitem_cast<QNModelItemPtr>(item);

                        //move model to new model
                        if(oldModel->moveIcon(model->getModel(), newModel)){
                            //move model item
                            this->removeItem(model);
                            model->setParentItem(newParent);
                            if(newParent == nullptr)
                                this->addItem(model);

                            model->setZValue(0.0);
                            model->setPos(point);
                        }

                        break;
                    }
                    case QNTextBoxItem::Type:{
                        QNTextBoxItemPtr textb = qgraphicsitem_cast<QNTextBoxItemPtr>(item);

                        //move textbox to new model
                        if(oldModel->moveTextBox(textb->getTextbox(), newModel)){
                            //move textbox item
                            this->removeItem(textb);
                            textb->setParentItem(newParent);
                            if(newParent == nullptr){
                                this->addItem(textb);
                                textb->setZValue(0.3);
                            }
                            else
                                textb->setZValue(newParent->zValue() + 0.3);

                            textb->setPos(point);
                        }

                        break;
                    }
                    default:
                        break;
                }

                if(newParent != nullptr)
                    newParent->setZValue(0.0);
            }
        }
    }
}

void QNNetworkScene::manageSelection(){
    //send selected signal
    QGraphicsItemList selected = this->selectedItems();
    QNNodeItemList nodes;
    QNModelItemList models;
    QNTextBoxItemList textboxes;
    QNEdgeItemList edges;
    for(QGraphicsItemPtr it: selected){
        if(it->type() == QNEdgeItem::Type)
            edges.push_back(dynamic_cast<QNEdgeItemPtr>(it));
        else if(it->type() == QNNodeItem::Type)
            nodes.push_back(dynamic_cast<QNNodeItemPtr>(it));
        else if(it->type() == QNModelItem::Type)
            models.push_back(dynamic_cast<QNModelItemPtr>(it));
        else if(it->type() == QNTextBoxItem::Type)
            textboxes.push_back(dynamic_cast<QNTextBoxItemPtr>(it));
    }

    emit this->itemsSelected(nodes, models, textboxes, edges);
}

bool QNNetworkScene::manageCtxMenuClick(QGraphicsSceneContextMenuEventPtr aEvent){
    QGraphicsItemList items = this->selectedItems();
    QMenuPtr m = nullptr;
    if(items.isEmpty()){
        //no items selected; check if the mouse click is on an item
        QGraphicsItemList list = this->items(aEvent->scenePos(), Qt::ContainsItemBoundingRect);
        int size = list.size();
        for(int i=0; i<size; i++){
            QGraphicsItemPtr tmp = list.at(i);
            switch (tmp->type()) {
                case QNNodeItem::Type:{
                    QNNodeItemPtr node = qgraphicsitem_cast<QNNodeItemPtr>(tmp);
                    m = node->getCtxMenu();
                    break;
                }
                case QNModelItem::Type:{
                    QNModelItemPtr model = qgraphicsitem_cast<QNModelItemPtr>(tmp);
                    m = model->getCtxMenu();
                    break;
                }
                case QNTextBoxItem::Type:{
                    QNTextBoxItemPtr textb = qgraphicsitem_cast<QNTextBoxItemPtr>(tmp);
                    m = textb->getCtxMenu();
                    break;
                }
                case QNEdgeItem::Type:{
                    QNEdgeItemPtr edge = qgraphicsitem_cast<QNEdgeItemPtr>(tmp);
                    m = edge->getCtxMenu();
                    break;
                }
                default:
                    //ignored
                    break;
            }
            if(m != nullptr){
                m->exec(aEvent->screenPos());
                return true;
            }
        }

        //manage click on the scene
        if(this->sceneCtxMenu != nullptr){
            this->sceneCtxMenu->exec(aEvent->screenPos());
            return true;
        }
    }
    else{
        //there are selected items
        int size = items.size();
        int nodeCount = 0;
        int edgeCount = 0;
        int modelCount = 0;
        int textbCount = 0;
        for(int i=0; i<size; i++){
            QGraphicsItemPtr tmp = items.at(i);
            switch (tmp->type()) {
                case QNNodeItem::Type:
                    nodeCount++;
                    break;
                case QNEdgeItem::Type:
                    edgeCount++;
                    break;
                case QNModelItem::Type:
                    modelCount++;
                    break;
                case QNTextBoxItem::Type:
                    textbCount++;
                    break;
                default:
                    //ignore it
                    break;
            }
        }

        if(nodeCount>0 && edgeCount==0 && modelCount==0 && textbCount==0){
            //only nodes
            if(nodeCount == 1)
                m = qgraphicsitem_cast<QNNodeItemPtr>(items.at(0))->getCtxMenu();
            else
                m = this->manyNodesSelectedCtxMenu;
        }
        else if(nodeCount==0 && edgeCount>0 && modelCount==0 && textbCount==0){
            //only edges
            if(edgeCount == 1)
                m = qgraphicsitem_cast<QNEdgeItemPtr>(items.at(0))->getCtxMenu();
            else
                m = this->manyEdgesSelectedCtxMenu;
        }
        else if(nodeCount==0 && edgeCount==0 && modelCount>0 && textbCount==0){
            //only models
            if(modelCount == 1)
                m = qgraphicsitem_cast<QNModelItemPtr>(items.at(0))->getCtxMenu();
            else
                m = this->manyModelsSelectedCtxMenu;
        }
        else if(nodeCount==0 && edgeCount==0 && modelCount==0 && textbCount>0){
            //only textboxes
            if(textbCount == 1)
                m = qgraphicsitem_cast<QNTextBoxItemPtr>(items.at(0))->getCtxMenu();

            else
                m = this->manyTextboxesSelectedCtxMenu;
        }
        else{
            //several kinds
            m = this->manySelectedCtxMenu;
        }

        if(m != nullptr){
            m->exec(aEvent->screenPos());
            return true;
        }
    }

    return false;
}

void QNNetworkScene::updateSceneCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->sceneCtxMenu != nullptr){
        foreach (QActionPtr a, this->sceneCtxMenu->actions())
            this->sceneCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->sceneCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateOneNodeSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->oneNodeSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->oneNodeSelectedCtxMenu->actions())
            this->oneNodeSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->oneNodeSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateManyNodesSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->manyNodesSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->manyNodesSelectedCtxMenu->actions())
            this->manyNodesSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->manyNodesSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateOneModelSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->oneModelSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->oneModelSelectedCtxMenu->actions())
            this->oneModelSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->oneModelSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateManyModelsSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->manyModelsSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->manyModelsSelectedCtxMenu->actions())
            this->manyModelsSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->manyModelsSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateOneTextboxSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->oneTextboxSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->oneTextboxSelectedCtxMenu->actions())
            this->oneTextboxSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->oneTextboxSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateManyTextboxesSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->manyTextboxesSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->manyTextboxesSelectedCtxMenu->actions())
            this->manyTextboxesSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->manyTextboxesSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateOneEdgeSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->oneEdgeSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->oneEdgeSelectedCtxMenu->actions())
            this->oneEdgeSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->oneEdgeSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateManyEdgesSelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->manyEdgesSelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->manyEdgesSelectedCtxMenu->actions())
            this->manyEdgesSelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->manyEdgesSelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::updateManySelectedCtxMenu(QMenuPtr aValue){
    //remove old actions
    if(this->manySelectedCtxMenu != nullptr){
        foreach (QActionPtr a, this->manySelectedCtxMenu->actions())
            this->manySelectedCtxMenu->removeAction(a);
    }

    //add new actions
    if(aValue != nullptr){
        foreach (QActionPtr a, aValue->actions())
            this->manySelectedCtxMenu->addAction(a);
    }
}

void QNNetworkScene::setNetwork(QNNetworkModelPtr aValue){
    //remove old items
    if(!this->itemList.isNull()){
        this->clear();
        this->itemList->clear();
    }

    //set new Network and populate the scene
    this->network = aValue;
    if(this->network != nullptr)
        this->addModel();
    else
        this->update();
}

QPointF QNNetworkScene::getMousePos() const{
    return this->mousePos;
}

void QNNetworkScene::setMousePos(const QPointF& aValue){
    this->mousePos = aValue;
}
