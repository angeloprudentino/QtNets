/**
 ** @file qnnodeitem.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-09
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

#include "qnnodeitem.h"
#include "qnnodeicon.h"
#include "qndrawconst.h"
#include "qnedgeitem.h"
#include "qnnetworkscene.h"
#include "qnstylemanager.h"
#include <QTextCursor>


QNNodeItem::QNNodeItem(QNNodeIconPtr aNode, QMenuPtr aNodeMenu, QNStyleManagerPtr aStyleMgr, QGraphicsItemPtr aParent): QNCommonItem(aStyleMgr, aParent){
    this->edges.reset();
    this->updateNode(aNode);
    this->updateCtxMenu(aNodeMenu);
}

QNNodeItem::~QNNodeItem(){
    if(!this->edges.isNull()){
        this->edges.reset();
    }
}

void QNNodeItem::updateNode(QNNodeIconPtr aNode){
    if(aNode != nullptr)
        this->icon = aNode;

    if(this->icon != nullptr){
        //show content
        this->icon->setShowContent(this->icon->isShowingContent());

        //property
        this->setLabelLen(this->icon->getLabelLen());
        this->updatePropToShow(this->icon->getPropToShow());

        //font
        QNFontPtr f = this->icon->getFont();
        if(f == nullptr){
            f = new QNFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR);
            this->icon->setFont(f);
        }
        if(this->styleMgr == nullptr || this->styleMgr->getStyle() == nullptr)
            this->updateFont(f);
        else
            this->styleMgr->updateFont(this);

        //comment
        QString comment = this->icon->getComment();
        this->setToolTip(comment);

        //position
        this->setPos(this->getTopLeft());
        this->setZValue(0.2);

        if(this->scene())
            this->scene()->update();
    }
}

void QNNodeItem::updateCtxMenu(QMenuPtr aCtxMenu){
    //remove old actions
    if(this->ctxMenu != nullptr){
        foreach (QActionPtr a, this->ctxMenu->actions())
            this->ctxMenu->removeAction(a);
    }

    //add new actions
    this->showHideAction->setText("Node expanded");
    this->ctxMenu->addAction(this->showHideAction);

    if(aCtxMenu != nullptr){
        foreach (QActionPtr a, aCtxMenu->actions())
            this->ctxMenu->addAction(a);
    }
}

QNNodeIconPtr QNNodeItem::getNode(){
    return dynamic_cast<QNNodeIconPtr>(this->icon);
}

QNEdgeItemListPtr QNNodeItem::getEdges(){
    return (this->edges.isNull() || this->edges->isEmpty()) ? nullptr : this->edges.data();
}

void QNNodeItem::addEdge(QNEdgeItemPtr aEdge){
    if(aEdge != nullptr){
        if(this->edges.isNull())
            this->edges.reset(new QNEdgeItemList());

        this->edges->push_back(aEdge);
    }
}

void QNNodeItem::removeEdge(QNEdgeItemPtr aEdge){
    if(aEdge != nullptr){
        if(!this->edges.isNull() && this->edges->contains(aEdge))
            this->edges->removeOne(aEdge);
    }
}

void QNNodeItem::setShowContent(const bool aValue){
    this->icon->setShowContent(aValue);
    this->showHideAction->setChecked(aValue);

    //update content
    this->updateNode();
    if(aValue){
        this->showHideAction->setToolTip(tr("Click to hide node's details"));
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
    else{
        this->showHideAction->setToolTip(tr("Click to show node's details"));
        this->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    }

    if(this->scene()){
        QNNetworkScenePtr netScene = dynamic_cast<QNNetworkScenePtr>(this->scene());
        if(netScene != nullptr)
            netScene->modified();

        this->scene()->update();
    }
}

int QNNodeItem::getWidth() const{
    QPoint tl = this->getTopLeft();
    QPoint br = this->getBottomRight();

    int boxW = br.x() - tl.x();
    if(!this->isShowingContent())
        boxW = (boxW < NODE_DEFAULT_W/2)? boxW: NODE_DEFAULT_W/2;

    int textW = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().width(): this->label.size().width();

    return (textW > boxW)? textW : boxW;
}

int QNNodeItem::getHeight() const{
    QPoint tl = this->getTopLeft();
    QPoint br = this->getBottomRight();

    int boxH = br.y() - tl.y();
    if(!this->isShowingContent())
        boxH = (boxH < NODE_DEFAULT_H/2)? boxH: NODE_DEFAULT_H/2;

    int textH = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().height(): this->label.size().height();

    return (textH > boxH)? textH : boxH;
}

int QNNodeItem::type() const{
    return QNNodeItem::Type;
}

void QNNodeItem::paint(QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget){
    if(this->icon != nullptr){
        QNError err(INVALID_STYLE_STATE, "");
        if(this->styleMgr != nullptr)
            err = this->styleMgr->paint(this, aPainter, aOption, aWidget);

        if(err.getCode() == INVALID_STYLE_STATE){
            aPainter->setRenderHint(QPainter::Antialiasing, true);
            aPainter->setRenderHint(QPainter::TextAntialiasing, true);

            //color
            QString colorString = ((QNNodeIconPtr)this->icon)->getColor();
            QColor fillColor = QColor("#" + colorString);
            fillColor = (aOption->state & QStyle::State_Selected) ? fillColor.dark(150) : fillColor;

            QBrush brush = aPainter->brush();
            brush.setColor(fillColor);
            brush.setStyle(Qt::SolidPattern);
            aPainter->setBrush(brush);

            //outline width
            QPen pen = aPainter->pen();
            int width = (aOption->state & QStyle::State_Selected) ? ((QNNodeIconPtr)this->icon)->getOutlineWidth()+2 : ((QNNodeIconPtr)this->icon)->getOutlineWidth();
            pen.setWidth(width);

            //outline color
            colorString = ((QNNodeIconPtr)this->icon)->getOutlineColor();
            QColor outColor = QColor("#" + colorString);
            pen.setColor(outColor);
            aPainter->setPen(pen);

            //node shape
            aPainter->drawRect(this->boundingRect());
        }
    }

    QGraphicsTextItem::paint(aPainter, aOption, aWidget);
}
