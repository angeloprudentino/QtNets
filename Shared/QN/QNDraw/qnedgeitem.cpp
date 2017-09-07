/**
 ** @file qnedgeitem.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-13
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

#include "qnedgeitem.h"
#include "qnnodeitem.h"
#include "qnedgestyle.h"
#include "qndrawconst.h"
#include "qngraphicconst.h"
#include "qnnetworkscene.h"
#include "qnmodelitem.h"
#include "qnnodeitem.h"
#include "qntextboxitem.h"
#include "qnedgeitem.h"
#include "qnstylemanager.h"
#include "qnstyle.h"
#include "qnnodeicon.h"
#include "qnnetworkmodel.h"
#include <math.h>

#define LABEL_MARGIN_H 8
#define LABEL_MARGIN_V -5

QLineF QNEdgeItem::determineLine(){
    QPointF srcPos;
    QPointF dstPos;
    if(this->parentItem() != nullptr){
        srcPos = this->srcNode->pos();
        dstPos = this->dstNode->pos();
    }
    else{
        srcPos = this->srcNode->scenePos();
        dstPos = this->dstNode->scenePos();
    }

    //self-loop
    if(this->srcNode == this->dstNode){
        return QLineF(dstPos, srcPos);
    }
    else{
        QPointF srcPoint;
        QPointF dstPoint;

        //about dst
        int dstW = this->dstNode->getWidth();
        int dstH = this->dstNode->getHeight();
        QPointF dstTL = dstPos;
        QPointF dstBL(dstTL.x(), dstTL.y()+dstH);
        QPoint dstTR(dstTL.x()+dstW, dstTL.y());
        QPointF dstBR(dstTL.x()+dstW, dstTL.y()+dstH);
        QPointF dstC(dstTL.x()+dstW/2, dstTL.y()+dstH/2);

        //about src
        int srcW = this->srcNode->getWidth();
        int srcH = this->srcNode->getHeight();
        QPointF srcTL = srcPos;
        QPointF srcBL(srcTL.x(), srcTL.y()+srcH);
        QPointF srcTR(srcTL.x()+srcW, srcTL.y());
        QPointF srcBR(srcTL.x()+srcW, srcTL.y()+srcH);
        QPointF srcC(srcTL.x()+srcW/2, srcTL.y()+srcH/2);

        //center to center line
        QLineF cToC(dstC, srcC);

        //determine dst intersection
        QList<QLineF> dstEdges;
        dstEdges.push_back(QLineF(dstTL, dstTR)); //top
        dstEdges.push_back(QLineF(dstBL, dstBR)); //bottom
        dstEdges.push_back(QLineF(dstTR, dstBR)); //right
        dstEdges.push_back(QLineF(dstTL, dstBL)); //left
        QLineF::IntersectType intersectType;
        bool found = false;
        for(QLineF edge: dstEdges){
            intersectType = cToC.intersect(edge, &dstPoint);
            if (intersectType == QLineF::BoundedIntersection){
                found = true;
                break;
            }
        }
        if(!found)
            dstPoint = dstBR;


        //determine src intersection
        QList<QLineF> srcEdges;
        srcEdges.push_back(QLineF(srcTL, srcTR)); //top
        srcEdges.push_back(QLineF(srcBL, srcBR)); //bottom
        srcEdges.push_back(QLineF(srcTR, srcBR)); //right
        srcEdges.push_back(QLineF(srcTL, srcBL)); //left
        found = false;
        for(QLineF edge: srcEdges){
            intersectType = cToC.intersect(edge, &srcPoint);
            if (intersectType == QLineF::BoundedIntersection){
                found = true;
                break;
            }
        }
        if(!found)
            srcPoint = srcTL;

        return QLineF(dstPoint, srcPoint);
    }
}

bool QNEdgeItem::isMoveSrcMode(QPointF aPoint) const{
    if(!this->srcNode->isShowingContent() || !this->dstNode->isShowingContent())
        return false;

    int mousePosX = aPoint.x();
    int mousePosY = aPoint.y();
    int srcX = this->line().p2().x();
    int srcY = this->line().p2().y();
    int margin = MOUSE_SENSIBILTY;

    //check if mouse is in a certain area
    bool hOk = mousePosX <= srcX + margin && mousePosX >= srcX - margin;
    bool vOk = mousePosY <= srcY + margin && mousePosY >= srcY - margin;
    return (hOk && vOk);
}

bool QNEdgeItem::isMoveDstMode(QPointF aPoint) const{
    if(!this->srcNode->isShowingContent() || !this->dstNode->isShowingContent())
        return false;

    int mousePosX = aPoint.x();
    int mousePosY = aPoint.y();
    int srcX = this->line().p1().x();
    int srcY = this->line().p1().y();
    int margin = MOUSE_SENSIBILTY;

    //check if mouse is in a certain area
    bool hOk = mousePosX <= srcX + margin && mousePosX >= srcX - margin;
    bool vOk = mousePosY <= srcY + margin && mousePosY >= srcY - margin;
    return (hOk && vOk);
}

void QNEdgeItem::manageMoveEdge(){
    bool toUpdate = false;

    QNNetworkScenePtr scene = dynamic_cast<QNNetworkScenePtr>(this->scene());
    if(scene != nullptr){
        if(this->moveSrcMode){
            QPointF start = this->mousePoint;//this->line().p2();
            QGraphicsItemList startItems = scene->items(start, Qt::ContainsItemBoundingRect);
            if (startItems.count() && startItems.first() == this)
                startItems.removeFirst();

            if (startItems.count() > 0 &&
                startItems.first()->type() == QNNodeItem::Type &&
                startItems.first() != this->srcNode) {
                //found new src. Now update structures
                QNNodeItemPtr newSrcNode = qgraphicsitem_cast<QNNodeItemPtr>(startItems.first());

                if(newSrcNode->isShowingContent() && this->dstNode->isShowingContent()){
                    //edge
                    QNEdgePtr e = this->edge->getData();
                    QNNetworkPtr net = e->getNet();
                    net->removeEdge(e, false);
                    e->setSrc(newSrcNode->getNode()->getData());
                    net->addEdge(e);

                    //edgestyle
                    QNNetworkModelPtr netModel = net->getNetModel();
                    netModel->removeEdge(this->edge, false);
                    this->edge->setData(e);
                    netModel->addEdge(this->edge);

                    //old src node
                    QNNodePtr dst = this->dstNode->getNode()->getData();
                    this->srcNode->removeEdge(this);
                    QNNodePtr old = this->srcNode->getNode()->getData();
                    old->removeSuccessor(dst);
                    dst->removeAncestor(old);

                    //new src node
                    this->srcNode = newSrcNode;
                    this->srcNode->addEdge(this);
                    QNNodePtr newSrc = this->srcNode->getNode()->getData();
                    newSrc->addSuccessor(dst);
                    dst->addAncestor(newSrc);

                    toUpdate = true;
                }
            }

        }
        else if(this->moveDstMode){
            QPointF end = this->mousePoint;//this->line().p1();
            QGraphicsItemList endItems = scene->items(end, Qt::ContainsItemBoundingRect);
            if (endItems.count() && endItems.first() == this)
                endItems.removeFirst();

            if (endItems.count() > 0 &&
                endItems.first()->type() == QNNodeItem::Type &&
                endItems.first() != this->dstNode) {
                //found new dst. Now update structures
                QNNodeItemPtr newDstNode = qgraphicsitem_cast<QNNodeItemPtr>(endItems.first());

                if(newDstNode->isShowingContent() && this->srcNode->isShowingContent()){
                    //edge
                    QNEdgePtr e = this->edge->getData();
                    QNNetworkPtr net = e->getNet();
                    net->removeEdge(e, false);
                    e->setDst(newDstNode->getNode()->getData());
                    net->addEdge(e);

                    //edgestyle
                    QNNetworkModelPtr netModel = net->getNetModel();
                    netModel->removeEdge(this->edge, false);
                    this->edge->setData(e);
                    netModel->addEdge(this->edge);

                    //old dst node
                    QNNodePtr src = this->srcNode->getNode()->getData();
                    this->dstNode->removeEdge(this);
                    QNNodePtr old = this->dstNode->getNode()->getData();
                    src->removeSuccessor(old);
                    old->removeAncestor(src);

                    //new dst node
                    this->dstNode = newDstNode;
                    this->dstNode->addEdge(this);
                    QNNodePtr newDst = this->dstNode->getNode()->getData();
                    src->addSuccessor(newDst);
                    newDst->addAncestor(src);

                    toUpdate = true;
                }
            }
        }

        if(toUpdate){
            QGraphicsItemPtr edgeOldParent = this->parentItem();
            QGraphicsItemPtr edgeNewParent = (this->getSrcNode()->parentItem() == this->getDstNode()->parentItem())? this->getSrcNode()->parentItem(): nullptr;
            if(edgeOldParent != edgeNewParent){
                //new parent
                if(edgeOldParent == nullptr && edgeNewParent != nullptr){
                    //enter a model from the main scene
                    scene->removeItem(this);
                    this->setParentItem(edgeNewParent);
                }
                else if(edgeOldParent != nullptr && edgeNewParent == nullptr){
                    //return to main scene
                    scene->removeItem(this);
                    this->setParentItem(edgeNewParent);
                    scene->addItem(this);
                }
                else{
                    //from a model to another
                    this->setParentItem(edgeNewParent);
                }
            }

            this->setZValue(0.1);
            this->updateEdgePos();
            emit scene->modified();
        }
    }
}

void QNEdgeItem::hoverEnterEvent(QGraphicsSceneHoverEventPtr aEvent){
    if(this->moveSrcMode || this->moveDstMode){
        this->setCursor(QCursor(Qt::OpenHandCursor));
    }
    else
        QGraphicsLineItem::hoverEnterEvent(aEvent);
}

void QNEdgeItem::hoverMoveEvent(QGraphicsSceneHoverEventPtr aEvent){
    if(this->moveSrcMode || this->moveDstMode){
        this->setCursor(QCursor(Qt::OpenHandCursor));
    }
    else
        QGraphicsLineItem::hoverMoveEvent(aEvent);
}

void QNEdgeItem::hoverLeaveEvent(QGraphicsSceneHoverEventPtr aEvent){
    this->setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsLineItem::hoverLeaveEvent(aEvent);
}

void QNEdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEventPtr aEvent){
    QNNetworkScenePtr scene = dynamic_cast<QNNetworkScenePtr>(this->scene());

    if(scene != nullptr){
        if(scene->selectedItems().size() == 1){
            //only itself selected
            if(this->ctxMenu != nullptr)
                this->ctxMenu->exec(aEvent->screenPos());
            else
                QGraphicsLineItem::contextMenuEvent(aEvent);
        }
        else
            scene->manageCtxMenuClick(aEvent);
    }
    else{
        //not in a network scene
        if(this->ctxMenu != nullptr)
            this->ctxMenu->exec(aEvent->screenPos());
        else
            QGraphicsLineItem::contextMenuEvent(aEvent);
    }
}

void QNEdgeItem::mousePressEvent(QGraphicsSceneMouseEventPtr aEvent){
    if(aEvent->button() == Qt::LeftButton){
        //check if resize mode and from which corner
        if(this->isMoveSrcMode(aEvent->pos())){
            this->moveSrcMode = true;
            this->moveDstMode = false;
            this->setCursor(QCursor(Qt::ClosedHandCursor));
        }
        else if(this->isMoveDstMode(aEvent->pos())){
            this->moveSrcMode = false;
            this->moveDstMode = true;
            this->setCursor(QCursor(Qt::ClosedHandCursor));
        }
        else{
            this->moveSrcMode = false;
            this->moveDstMode = false;
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    QGraphicsLineItem::mousePressEvent(aEvent);
}

void QNEdgeItem::mouseMoveEvent(QGraphicsSceneMouseEventPtr aEvent){
    this->mousePoint = aEvent->scenePos();

    if(this->moveSrcMode){
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        QLineF newLine(this->line().p1(), aEvent->pos());
        this->setLine(newLine);
    }
    else if(this->moveDstMode){
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        QLineF newLine(aEvent->pos(), this->line().p2());
        this->setLine(newLine);
    }
    else{
        this->setCursor(QCursor(Qt::ArrowCursor));
        QGraphicsLineItem::mouseMoveEvent(aEvent);
    }
}

void QNEdgeItem::mouseReleaseEvent(QGraphicsSceneMouseEventPtr aEvent){
    this->manageMoveEdge();
    this->moveSrcMode = false;
    this->moveDstMode = false;
    this->setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsLineItem::mouseReleaseEvent(aEvent);
    //    QNNetworkScenePtr netScene = dynamic_cast<QNNetworkScenePtr>(this->scene());
    //    if(netScene != nullptr){
    //        //send selected signal
    //        netScene->manageSelection();
    //    }
}

QNEdgeItem::QNEdgeItem(QNNodeItemPtr aSrcNode, QNNodeItemPtr aDstNode, QNEdgeStylePtr aEdge,
                       QMenuPtr aEdgeMenu, QNStyleManagerPtr aStyleMgr, QGraphicsItemPtr aParent): QGraphicsLineItem(aParent){
    this->srcNode = aSrcNode;
    this->dstNode = aDstNode;
    this->edge = nullptr;
    this->ctxMenu = new QMenu();
    this->styleMgr = aStyleMgr;
    //    this->label = nullptr;
    this->updateEdge(aEdge);
    this->updateEdgeMenu(aEdgeMenu);

    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setAcceptHoverEvents(true);
}

QNEdgeItem::~QNEdgeItem(){
    this->srcNode = nullptr;
    this->dstNode = nullptr;
    this->edge = nullptr;
    delete this->ctxMenu;
    this->ctxMenu = nullptr;
    this->styleMgr = nullptr;
}

bool QNEdgeItem::operator==(const QNEdgeItem& aOther) const{
    return this->moveSrcMode == aOther.moveSrcMode &&
            this->moveDstMode == aOther.moveDstMode &&
            this->mousePoint == aOther.mousePoint &&
            ((this->srcNode == nullptr && aOther.srcNode == nullptr) ||
             (this->srcNode != nullptr && aOther.srcNode != nullptr && *(this->srcNode) == *(aOther.srcNode))) &&
            ((this->dstNode == nullptr && aOther.dstNode == nullptr) ||
             (this->dstNode != nullptr && aOther.dstNode != nullptr && *(this->dstNode) == *(aOther.dstNode))) &&
            //            ((this->srcNode == nullptr && aOther.srcNode == nullptr) ||
            //             (this->srcNode != nullptr && aOther.srcNode != nullptr &&
            //              (this->srcNode->getNode() == nullptr && aOther.srcNode->getNode() == nullptr) ||
            //              (this->srcNode->getNode() != nullptr && aOther.srcNode->getNode() != nullptr && *(this->srcNode->getNode()) == *(aOther.srcNode->getNode())))) &&
            //            ((this->dstNode == nullptr && aOther.dstNode == nullptr) ||
            //             (this->dstNode != nullptr && aOther.dstNode != nullptr &&
            //              (this->dstNode->getNode() == nullptr && aOther.dstNode->getNode() == nullptr) ||
            //              (this->dstNode->getNode() != nullptr && aOther.dstNode->getNode() != nullptr && *(this->dstNode->getNode()) == *(aOther.dstNode->getNode())))) &&
            ((this->edge == nullptr && aOther.edge == nullptr) ||
             (this->edge != nullptr && aOther.edge != nullptr && *(this->edge) == *(aOther.edge)));
}

bool QNEdgeItem::operator!=(const QNEdgeItem& aOther) const{
    return !(this->operator==(aOther));
}

void QNEdgeItem::updateEdge(QNEdgeStylePtr aEdge){
    if(aEdge != nullptr){
        this->edge = aEdge;

        //comment
        QString comment = this->edge->getComment();
        this->setToolTip(comment);
        //        if(this->label == nullptr){
        //            this->label = new QGraphicsTextItem(this);
        //            this->label->setFlag(QGraphicsItem::ItemIsMovable);
        //            this->label->setFlag(QGraphicsItem::ItemIsSelectable);
        //            this->label->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        //        }
        //        this->label->setPlainText(comment);

        //position
        this->setZValue(0.1);
        this->updateEdgePos();
    }
}

void QNEdgeItem::updateEdgeMenu(QMenuPtr aEdgeMenu){
    //remove old actions
    if(this->ctxMenu != nullptr){
        foreach (QActionPtr a, this->ctxMenu->actions())
            this->ctxMenu->removeAction(a);
    }

    //add new actions
    if(aEdgeMenu != nullptr){
        foreach (QActionPtr a, aEdgeMenu->actions())
            this->ctxMenu->addAction(a);
    }
}

void QNEdgeItem::updateEdgePos(){
    QLineF line = this->determineLine();
    this->setLine(line);
    //    QPointF c = QPointF(line.p1().x() + line.dx()/2 + LABEL_MARGIN_H,
    //                        line.p1().y() + line.dy()/2 + LABEL_MARGIN_V);
    //    this->label->setPos(c);

    if(this->scene())
        this->scene()->update();
}

QMenuPtr QNEdgeItem::getCtxMenu() const{
    return this->ctxMenu;
}

QNNodeItemPtr QNEdgeItem::getSrcNode() const{
    return this->srcNode;
}

QNNodeItemPtr QNEdgeItem::getDstNode() const{
    return this->dstNode;
}

QNEdgeStylePtr QNEdgeItem::getEdge() const{
    return this->edge;
}

QRectF QNEdgeItem::boundingRect() const{
    double penCompensation = (this->pen().width() + 20) / 2.0;
    QLineF line = this->line();

    return QRectF(line.p1(), QSizeF(line.dx(),line.dy()))
            .normalized()
            .adjusted(-penCompensation, -penCompensation, penCompensation, penCompensation);
}

int QNEdgeItem::type() const{
    return QNEdgeItem::Type;
}

void QNEdgeItem::paint(QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget){
    if(this->edge != nullptr){
        QLineF line;
        if(this->moveSrcMode || this->moveDstMode){
            line = this->line();
        }
        else{
            line = this->determineLine();
            this->setLine(line);
        }

        QNError err(INVALID_STYLE_STATE, "");
        if(this->styleMgr != nullptr)
            err = this->styleMgr->paint(this, aPainter, aOption, aWidget);

        if(err.getCode() == INVALID_STYLE_STATE){
            aPainter->setRenderHint(QPainter::Antialiasing, true);
            aPainter->setRenderHint(QPainter::TextAntialiasing, true);

            //color
            QColor fillColor = QColor("#" + QString(DEFAULT_FG_COLOR));
            fillColor = (aOption->state & QStyle::State_Selected) ? fillColor.dark(150) : fillColor;

            QBrush brush = aPainter->brush();
            brush.setColor(fillColor);
            brush.setStyle(Qt::SolidPattern);
            aPainter->setBrush(brush);

            //outline width
            QPen pen = aPainter->pen();
            int width = (aOption->state & QStyle::State_Selected) ? DEFAULT_WIDTH+2 : DEFAULT_WIDTH;
            pen.setWidth(width);

            //outline color
            QColor outColor = QColor("#" + QString(DEFAULT_FG_COLOR));
            pen.setColor(outColor);
            aPainter->setPen(pen);

            //line
            this->setPen(pen);
            this->setLine(line);

            //arrow
            const double Pi = 3.14;
            double angle = ::acos(line.dx() / line.length());
            if (line.dy() >= 0)
                angle = (Pi * 2) - angle;


            QPointF arrowP1 = line.p1() + QPointF(sin(angle + Pi/3) * ARROW_SIZE,
                                                  cos(angle + Pi/3) * ARROW_SIZE);
            QPointF arrowP2 = line.p1() + QPointF(sin(angle + Pi - Pi/3) * ARROW_SIZE,
                                                  cos(angle + Pi - Pi/3) * ARROW_SIZE);

            QPolygonF arrow;
            arrow << line.p1() << arrowP1 << arrowP2;
            aPainter->drawPolygon(arrow);

            //        //self-loop
            //        if(this->srcNode == this->dstNode){
            //            int srcW = this->srcNode->getWidth();
            //            int srcH = this->srcNode->getHeight();
            //            QPointF srcTL = this->srcNode->scenePos();

            //            int randW = 25;//rand()%srcW;
            //            int randH = 20;//rand()%srcH;
            //            QPointF tl(srcTL.x()+srcW/2, srcTL.y()-randH);
            //            QPointF br(srcTL.x()+srcW+randW, srcTL.y()+srcH/2);
            //            QRectF r(tl, br);
            //            painter->setBrush(QBrush());
            //            painter->drawRect(r);
            //        }
        }
    }

    QGraphicsLineItem::paint(aPainter, aOption, aWidget);
}
