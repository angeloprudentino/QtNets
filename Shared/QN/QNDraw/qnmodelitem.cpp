/**
 ** @file qnmodelitem.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-12
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

#include "qnmodelitem.h"
#include "qnnodeitem.h"
#include "qnedgeitem.h"
#include "qntextboxitem.h"
#include "qnmodel.h"
#include "qnnetworkscene.h"
#include "qnstylemanager.h"
#include "qndrawconst.h"
#include <QTextDocument>

#define MODEL_ARCH 20
#define DEFAULT_TEXT_H 23
#define MODEL_MIN_H_MARGIN 200
#define MODEL_MIN_V_MARGIN 100
#define NAME_TRUNC_LEN 10


QNModelItem::QNModelItem(QNModelPtr aModel, QMenuPtr aModelMenu, QNStyleManagerPtr aStyleMgr, QGraphicsItemPtr aParent): QNCommonItem(aStyleMgr, aParent){
    this->updateModel(aModel);
    this->updateCtxMenu(aModelMenu);
}

void QNModelItem::updateModel(QNModelPtr aModel){
    if(aModel != nullptr)
        this->icon = aModel;

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
        QPoint tl = this->getTopLeft();
        this->setPos(tl);
        this->setZValue(0.0);

        if(this->scene())
            this->scene()->update();
    }
}

void QNModelItem::updateCtxMenu(QMenuPtr aCtxMenu){
    //remove old actions
    if(this->ctxMenu != nullptr){
        foreach (QActionPtr a, this->ctxMenu->actions())
            this->ctxMenu->removeAction(a);
    }

    //add new actions
    this->showHideAction->setText("Model expanded");
    this->ctxMenu->addAction(this->showHideAction);

    if(aCtxMenu != nullptr){
        foreach (QActionPtr a, aCtxMenu->actions())
            this->ctxMenu->addAction(a);
    }
}

QNModelPtr QNModelItem::getModel(){
    return dynamic_cast<QNModelPtr>(this->icon);
}

void QNModelItem::setShowContent(const bool aValue){
    this->icon->setShowContent(aValue);
    this->showHideAction->setChecked(aValue);

    //update content
    this->updateModel();
    QGraphicsItemList childs = this->childItems();
    if(aValue){
        this->showHideAction->setToolTip(tr("Click to hide model's details"));
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

        foreach (QGraphicsItemPtr child, childs) {
            child->setVisible(true);
            switch (child->type()) {
                case QNNodeItem::Type:{
                    QNNodeItemPtr node = qgraphicsitem_cast<QNNodeItemPtr>(child);
                    node->updateNode();
                    break;
                }
                case QNModelItem::Type:{
                    QNModelItemPtr model = qgraphicsitem_cast<QNModelItemPtr>(child);
                    model->updateModel();
                    break;
                }
                case QNTextBoxItem::Type:{
                    QNTextBoxItemPtr textb = qgraphicsitem_cast<QNTextBoxItemPtr>(child);
                    textb->updateTextbox();
                    break;
                }
                case QNEdgeItem::Type:{
                    QNEdgeItemPtr edge = qgraphicsitem_cast<QNEdgeItemPtr>(child);
                    edge->updateEdgePos();
                    break;
                }
                default:
                    //ignored
                    break;
            }
            child->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
        }
    }
    else{
        this->showHideAction->setToolTip(tr("Click to show model's details"));

        this->setFlag(QGraphicsItem::ItemIsMovable, false);
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
        foreach (QGraphicsItemPtr child, childs) {
            child->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
            child->setVisible(false);
            int t = child->type();
            if(t == QNNodeItem::Type || t == QNModelItem::Type || t == QNTextBoxItem::Type)
                child->setPos(this->getCenter());
        }
    }

    if(this->scene()){
        QNNetworkScenePtr netScene = dynamic_cast<QNNetworkScenePtr>(this->scene());
        if(netScene != nullptr)
            netScene->modified();

        this->scene()->update();
    }
}

int QNModelItem::getWidth() const{
    QPoint tl = this->getTopLeft();
    QPoint br = this->getBottomRight();

    int boxW = br.x() - tl.x();
    if(!this->isShowingContent())
        boxW = (boxW < MODEL_DEFAULT_W/2)? boxW: MODEL_DEFAULT_W/2;

    int textW = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().width(): this->label.size().width();
    int textH = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().height(): this->label.size().height();
    int arch = ((double)textH / DEFAULT_TEXT_H)*MODEL_ARCH;

    return (textW+MODEL_MIN_H_MARGIN+arch > boxW)? textW+MODEL_MIN_H_MARGIN+arch : boxW;
}

int QNModelItem::getHeight() const{
    QPoint tl = this->getTopLeft();
    QPoint br = this->getBottomRight();

    int boxH = br.y() - tl.y();
    if(!this->isShowingContent())
        boxH = (boxH < MODEL_DEFAULT_H/2)? boxH: MODEL_DEFAULT_H/2;

    int textH = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().height(): this->label.size().height();

    return (textH+MODEL_MIN_V_MARGIN > boxH)? textH+MODEL_MIN_V_MARGIN : boxH;
}

int QNModelItem::type() const{
    return QNModelItem::Type;
}

void QNModelItem::paint(QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget){
    if(this->icon != nullptr){
        QNError err(INVALID_STYLE_STATE, "");
        if(this->styleMgr != nullptr)
            err = this->styleMgr->paint(this, aPainter, aOption, aWidget);

        if(err.getCode() == INVALID_STYLE_STATE){
            aPainter->setRenderHint(QPainter::Antialiasing, true);
            aPainter->setRenderHint(QPainter::TextAntialiasing, true);

            //color
            QString colorString = ((QNModelPtr)this->icon)->getColor();
            QColor fillColor = QColor("#" + colorString);
            fillColor = (aOption->state & QStyle::State_Selected) ? fillColor.dark(150) : fillColor;

            QBrush brush = aPainter->brush();
            brush.setColor(fillColor);
            brush.setStyle(Qt::SolidPattern);
            aPainter->setBrush(brush);

            //outline width
            QPen pen = aPainter->pen();
            int width = (aOption->state & QStyle::State_Selected) ? ((QNModelPtr)this->icon)->getOutlineWidth()+2 : ((QNModelPtr)this->icon)->getOutlineWidth();
            pen.setWidth(width);

            //outline color
            colorString = ((QNModelPtr)this->icon)->getOutlineColor();
            QColor outColor = QColor("#" + colorString);
            pen.setColor(outColor);
            aPainter->setPen(pen);

            //shape
            int w = this->getWidth();
            int h = this->getHeight();
            int textW = this->label.size().width();
            int textH = this->label.size().height();
            int arch = ((double)textH / DEFAULT_TEXT_H)*MODEL_ARCH;
            QPoint topLeft(0,0);
            QPoint bottomLeft(0,textH);
            QPoint bottomRight(textW+arch, textH);
            QPoint topRight(textW,0);
            QPolygon nameSlot;
            nameSlot << topLeft << bottomLeft << bottomRight << topRight;
            aPainter->drawPolygon(nameSlot);

            topLeft = QPoint(0,textH);
            bottomLeft = QPoint(0,h);
            bottomRight = QPoint(w,h);
            topRight = QPoint(w,textH);
            QPolygon body;
            body << topLeft << bottomLeft << bottomRight << topRight;
            aPainter->drawPolygon(body);
        }
    }

    QGraphicsTextItem::paint(aPainter, aOption, aWidget);
}
