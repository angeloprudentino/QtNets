/**
 ** @file qncommonitem.cpp
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

#include "qncommonitem.h"
#include "qndrawconst.h"
#include "qnposition.h"
#include "qngraphicconst.h"
#include "qnnetworkscene.h"
#include "qnmodelitem.h"
#include "qnnodeitem.h"
#include "qntextboxitem.h"
#include "qnedgeitem.h"
#include "qnfont.h"
#include "qncommonicon.h"
#include "qnstylemanager.h"
#include "qnstyle.h"
#include <QPoint>
#include <QTextCursor>


bool QNCommonItem::isTopLeftResize(QPointF aPoint) const{
    //    int mousePosX = aPoint.x();
    //    int mousePosY = aPoint.y();
    //    int margin = MOUSE_SENSIBILTY;

    //    //check if mouse is in a certain area
    //    bool hOk = mousePosX <= margin && mousePosX >= -margin;
    //    bool vOk = mousePosY <= margin && mousePosY >= -margin;
    //    return (hOk && vOk);
    Q_UNUSED(aPoint)
    return false;
}

bool QNCommonItem::isTopRightResize(QPointF aPoint) const{
    if(!this->isShowingContent())
        return false;

    int mousePosX = aPoint.x();
    int mousePosY = aPoint.y();
    int w = this->getWidth();
    int margin = MOUSE_SENSIBILTY;

    //check if mouse is in a certain area
    bool hOk = mousePosX <= w + margin && mousePosX >= w - margin;
    bool vOk = mousePosY <= margin && mousePosY >= -margin;
    return (hOk && vOk);
}

bool QNCommonItem::isBottomLeftResize(QPointF aPoint) const{
    if(!this->isShowingContent())
        return false;

    int mousePosX = aPoint.x();
    int mousePosY = aPoint.y();
    int h = this->getHeight();
    int margin = MOUSE_SENSIBILTY;

    //check if mouse is in a certain area
    bool hOk = mousePosX <= margin && mousePosX >= -margin;
    bool vOk = mousePosY <= h + margin && mousePosY >= h - margin;
    return (hOk && vOk);
}

bool QNCommonItem::isBottomRightResize(QPointF aPoint) const{
    if(!this->isShowingContent())
        return false;

    int mousePosX = aPoint.x();
    int mousePosY = aPoint.y();
    int w = this->getWidth();
    int h = this->getHeight();
    int margin = MOUSE_SENSIBILTY;

    //check if mouse is in a certain area
    bool hOk = mousePosX <= w + margin && mousePosX >= w - margin;
    bool vOk = mousePosY <= h + margin && mousePosY >= h - margin;
    return (hOk && vOk);
}

bool QNCommonItem::manageResize(QPointF aPoint){
    bool toResize = false;

    if(this->topLeftResize){
        QPoint tl = this->getTopLeft();
        QPoint br = this->getBottomRight();
        int left = tl.x() + aPoint.x();
        int top = tl.y() + aPoint.y();
        int right = br.x();
        int bottom = br.y();
        this->updatePosition(left, top, right, bottom);
        toResize = true;
    }
    else if(this->topRightResize){
        QPoint tl = this->getTopLeft();
        QPoint br = this->getBottomRight();
        int left = tl.x();
        int top = tl.y()/* - aPoint.y()*/;
        int right = br.x() - this->getWidth() + aPoint.x();
        int bottom = br.y();
        this->updatePosition(left, top, right, bottom);
        toResize = true;
    }
    else if(this->bottomLeftResize){
        QPoint tl = this->getTopLeft();
        int left = tl.x()/* + aPoint.x()*/;
        int top = tl.y();
        int right = this->getBottomRight().x();
        int bottom = top + aPoint.y();
        this->updatePosition(left, top, right, bottom);
        toResize = true;
    }
    else if(this->bottomRightResize){
        int left = this->getTopLeft().x();
        int top = this->getTopLeft().y();
        int right = left + aPoint.x();
        int bottom = top + aPoint.y();
        this->updatePosition(left, top, right, bottom);
        toResize = true;
    }

    if(toResize){
        QGraphicsScenePtr scene = this->scene();
        if(scene != nullptr){
            scene->update();
            QNNetworkScene* netScene = dynamic_cast<QNNetworkScene *>(scene);
            if(netScene)
                emit netScene->modified();
        }
    }

    return toResize;
}

QPoint QNCommonItem::getTopLeft() const{
    QPoint res;

    if(this->icon != nullptr){
        QNPositionPtr pos = this->icon->getPosition();
        if(pos != nullptr){
            res.setX(pos->getLeft());
            res.setY(pos->getTop());
        }
    }
    return res;
}

QPoint QNCommonItem::getBottomRight() const{
    QPoint res;

    if(this->icon != nullptr){
        QNPositionPtr pos = this->icon->getPosition();
        if(pos != nullptr){
            res.setX(pos->getRight());
            res.setY(pos->getBottom());
        }
    }
    return res;
}

QVariant QNCommonItem::itemChange(GraphicsItemChange aChange, const QVariant& aValue){
    bool isChanged = false;

    if (aChange == QGraphicsItem::ItemPositionHasChanged) {
        if(this->icon != nullptr){
            QPointF oldPos = this->getTopLeft();
            QPointF newPos = aValue.toPointF(); // value is the new position.
            if(oldPos != newPos){
                int left = newPos.x();
                int top = newPos.y();
                int right = left + this->getWidth();
                int bottom = top + this->getHeight();
                this->updatePosition(left, top, right, bottom);
                //update the scene
                isChanged = true;
                if(this->scene())
                    this->scene()->update();
            }
        }
    }

    if (aChange == QGraphicsItem::ItemSelectedHasChanged) {
        if(this->icon != nullptr && this->icon->isShowingContent()){
            QString oldProp = this->icon->getProperty(this->icon->getPropToShow()).toString();
            QString newProp = this->label.toPlainText();

            if(oldProp != newProp){
                //update prop
                if(this->icon->setProperty(this->icon->getPropToShow(), newProp)){
                    //error happened. Restore old value
                    newProp = oldProp;
                    this->label.setPlainText(newProp);
                }

                //truncate label if necessary
                int ll = this->icon->getLabelLen();
                if(ll > -1){
                    if(newProp.length() > ll){
                        newProp.resize(ll);
                        newProp.append("...");
                    }
                    this->labelTrunc.setPlainText(newProp);
                    this->setDocument(&(this->labelTrunc));
                }
                else
                    this->setDocument(&(this->label));

                this->adjustSize();
                isChanged = true;
            }
        }
    }

    if(this->scene() && isChanged){
        QNNetworkScenePtr netScene = dynamic_cast<QNNetworkScenePtr>(this->scene());
        if(netScene != nullptr)
            emit netScene->modified();
    }

    return aValue;
}

void QNCommonItem::mousePressEvent(QGraphicsSceneMouseEventPtr aEvent){
    if(aEvent->button() == Qt::LeftButton){
        //check if resize mode and from which corner
        if(this->isTopLeftResize(aEvent->pos())){
            this->topLeftResize = true;
            this->topRightResize = false;
            this->bottomLeftResize = false;
            this->bottomRightResize = false;
            this->setCursor(QCursor(Qt::SizeHorCursor));
        }
        else if(this->isTopRightResize(aEvent->pos())){
            this->topLeftResize = false;
            this->topRightResize = true;
            this->bottomLeftResize = false;
            this->bottomRightResize = false;
            this->setCursor(QCursor(Qt::SizeHorCursor));
        }
        else if(this->isBottomLeftResize(aEvent->pos())){
            this->topLeftResize = false;
            this->topRightResize = false;
            this->bottomLeftResize = true;
            this->bottomRightResize = false;
            this->setCursor(QCursor(Qt::SizeVerCursor));
        }
        else if(this->isBottomRightResize(aEvent->pos())){
            this->topLeftResize = false;
            this->topRightResize = false;
            this->bottomLeftResize = false;
            this->bottomRightResize = true;
            this->setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        else{
            this->topLeftResize = false;
            this->topRightResize = false;
            this->bottomLeftResize = false;
            this->bottomRightResize = false;
            this->setCursor(QCursor(Qt::ClosedHandCursor));
        }
    }

    QGraphicsTextItem::mousePressEvent(aEvent);
}

void QNCommonItem::mouseMoveEvent(QGraphicsSceneMouseEventPtr aEvent){
    if(!this->manageResize(aEvent->pos()))
        QGraphicsTextItem::mouseMoveEvent(aEvent);
}

void QNCommonItem::mouseReleaseEvent(QGraphicsSceneMouseEventPtr aEvent){
    this->setCursor(QCursor(Qt::ArrowCursor));

    if(!this->manageResize(aEvent->pos())){
        QNNetworkScenePtr netScene = dynamic_cast<QNNetworkScenePtr>(this->scene());
        if(netScene != nullptr){
            netScene->manageMoveInside();
            //netScene->manageSelection();
        }
        QGraphicsTextItem::mouseReleaseEvent(aEvent);
    }
}

void QNCommonItem::mouseDoubleClickEvent(QGraphicsSceneMouseEventPtr aEvent){
    if(aEvent->button() == Qt::LeftButton){
        if (this->textInteractionFlags() == Qt::NoTextInteraction && this->isShowingContent()){
            this->setTextInteractionFlags(Qt::TextEditorInteraction);
            if(this->getLabelLen() > -1){
                this->setDocument(&(this->label));
                //this->updateFont();
            }
        }
    }

    QGraphicsTextItem::mouseDoubleClickEvent(aEvent);
}

void QNCommonItem::focusOutEvent(QFocusEventPtr aEvent){
    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();
    this->setTextCursor(cursor);
    if (this->textInteractionFlags() == Qt::TextEditorInteraction && this->isShowingContent()){
        this->setTextInteractionFlags(Qt::NoTextInteraction);
        if(this->getLabelLen() > -1){
            this->setDocument(&(this->labelTrunc));
            //this->updateFont();
        }
    }
    QGraphicsTextItem::focusOutEvent(aEvent);
}

void QNCommonItem::hoverEnterEvent(QGraphicsSceneHoverEventPtr aEvent){
    if(this->isTopLeftResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(this->isTopRightResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(this->isBottomLeftResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(this->isBottomRightResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else{
        this->setCursor(QCursor(Qt::ArrowCursor));
    }

    QGraphicsTextItem::hoverEnterEvent(aEvent);
}

void QNCommonItem::hoverMoveEvent(QGraphicsSceneHoverEventPtr aEvent){
    if(this->isTopLeftResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(this->isTopRightResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if(this->isBottomLeftResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if(this->isBottomRightResize(aEvent->pos())){
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else{
        this->setCursor(QCursor(Qt::ArrowCursor));
    }

    QGraphicsTextItem::hoverMoveEvent(aEvent);
}

void QNCommonItem::hoverLeaveEvent(QGraphicsSceneHoverEventPtr aEvent){
    this->setCursor(QCursor(Qt::ArrowCursor));
    QGraphicsTextItem::hoverLeaveEvent(aEvent);
}

void QNCommonItem::contextMenuEvent(QGraphicsSceneContextMenuEventPtr aEvent){
    QNNetworkScenePtr scene = dynamic_cast<QNNetworkScenePtr>(this->scene());

    if(scene != nullptr){
        if(scene->selectedItems().size() == 1){
            //only itself selected
            if(this->ctxMenu != nullptr)
                this->ctxMenu->exec(aEvent->screenPos());
            else
                QGraphicsTextItem::contextMenuEvent(aEvent);
        }
        else
            scene->manageCtxMenuClick(aEvent);
    }
    else{
        //not in a network scene
        if(this->ctxMenu != nullptr)
            this->ctxMenu->exec(aEvent->screenPos());
        else
            QGraphicsTextItem::contextMenuEvent(aEvent);
    }
}

void QNCommonItem::updatePosition(const int aLeft, const int aTop, const int aRight, const int aBottom){
    if(this->icon != nullptr){
        QNPositionPtr pos = this->icon->getPosition();
        if(pos == nullptr){
            pos = new QNPosition(aLeft, aTop, aRight, aBottom);
            this->icon->setPosition(pos);
        }
        else{
            pos->setLeft(aLeft);
            pos->setTop(aTop);
            pos->setRight(aRight);
            pos->setBottom(aBottom);
        }
    }
}

void QNCommonItem::updateLabel(const QString& aContent){
    if(this->isShowingContent()){
        QString lbl = aContent;
        this->label.setPlainText(lbl);
        int ll = this->getLabelLen();
        if(ll > -1){
            if(lbl.length() > ll){
                lbl.resize(ll);
                lbl.append("...");
            }
            this->labelTrunc.setPlainText(lbl);
            this->setDocument(&(this->labelTrunc));
        }
        else{
            this->setDocument(&(this->label));
        }
    }
    else{
        this->labelTrunc.setPlainText("[...]");

        this->setDocument(&(this->labelTrunc));
    }
}

QNCommonItem::QNCommonItem(QNStyleManagerPtr aStyleMgr, QGraphicsItemPtr aParent): QGraphicsTextItem(aParent){
    this->icon = nullptr;
    this->ctxMenu = new QMenu();
    this->styleMgr = aStyleMgr;
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    this->setAcceptHoverEvents(true);

    this->showHideAction = new QAction(this->ctxMenu);
    this->showHideAction->setCheckable(true);
    this->showHideAction->setChecked(this->isShowingContent());
    this->showHideAction->setToolTip(tr("Click to hide details"));
    connect(this->showHideAction, SIGNAL(triggered()), this, SLOT(toggleShowContent()));
}

QNCommonItem::~QNCommonItem(){
    this->icon = nullptr;
    this->styleMgr = nullptr;
    delete this->ctxMenu;
    this->ctxMenu = nullptr;
}

bool QNCommonItem::operator==(const QNCommonItem& aOther) const{
    return ((this->icon == nullptr && aOther.icon == nullptr) ||
            (this->icon != nullptr && aOther.icon != nullptr && *(this->icon) == *(aOther.icon)));
}

bool QNCommonItem::operator!=(const QNCommonItem& aOther) const{
    return !(this->operator==(aOther));
}

QMenuPtr QNCommonItem::getCtxMenu() const{
    return this->ctxMenu;
}

void QNCommonItem::updateFont(QNFontPtr aFont){
    if(aFont != nullptr){
        QTextDocument* curr = this->document();
        QTextOption option;
        QFont font(aFont->getFontName(), aFont->getFontSize());
        font.setBold(aFont->isBold());
        font.setItalic(aFont->isItalic());
        font.setStyleStrategy(QFont::ForceOutline);
        QString colorString = aFont->getFontColor();
        QColor fontColor = QColor("#" + colorString);
        switch (aFont->getAlign()) {
            case QNAlignment::rightAlign:
                option.setAlignment(Qt::AlignRight);
                break;
            case QNAlignment::centerAlign:
                option.setAlignment(Qt::AlignHCenter);
                break;
            default:
                option.setAlignment(Qt::AlignLeft);
                break;
        }

        //update label
        this->setDocument(&(this->label));
        this->setFont(font);
        this->setDefaultTextColor(fontColor);
        this->label.setDefaultTextOption(option);

        //update labelTrunc
        this->setDocument(&(this->labelTrunc));
        this->setFont(font);
        this->setDefaultTextColor(fontColor);
        this->labelTrunc.setDefaultTextOption(option);

        //restore current document object
        this->setDocument(curr);
        curr = nullptr;
    }
}

bool QNCommonItem::isShowingContent() const{
    return this->icon != nullptr && this->icon->isShowingContent();
}

QString QNCommonItem::getPropToShow() const{
    return (this->icon != nullptr)? this->icon->getPropToShow() : "empty";
}

bool QNCommonItem::updatePropToShow(const QString& aProp){
    if(this->icon != nullptr){
        QVariant p = this->icon->getProperty(aProp);
        if(!p.isValid() || (p.isValid() && p.canConvert(QMetaType::QString))){
            this->icon->updatePropToShow(aProp);
            this->updateLabel(p.toString());
            return true;
        }
    }

    return false;
}

QTextDocument& QNCommonItem::getLabel(){
    return this->label;
}

int QNCommonItem::getLabelLen() const{
    return (this->icon != nullptr)? this->icon->getLabelLen() : -1;
}

void QNCommonItem::setLabelLen(int aValue){
    if(this->icon != nullptr)
        this->icon->setLabelLen(aValue);
}

QPoint QNCommonItem::getCenter() const{
    int w = this->getWidth();
    int h = this->getHeight();

    return QPoint(w/2, h/2);
}

void QNCommonItem::scale(const double aValue){
    if(aValue > 0.0 && aValue != 1.0){
        int newW = this->getWidth()*aValue;
        int newH = this->getHeight()*aValue;

        QPoint tl = this->getTopLeft();
        int top = tl.x();
        int left = tl.y();
        int bottom = top + newH;
        int right = left + newW;

        this->updatePosition(left, top, right, bottom);

        QNNetworkScenePtr netScene = dynamic_cast<QNNetworkScenePtr>(this->scene());
        if(netScene != nullptr){
            emit netScene->modified();
            netScene->update();
        }
    }
}

QRectF QNCommonItem::boundingRect() const{
    return QRectF(0, 0, this->getWidth(), this->getHeight());
}

QPainterPath QNCommonItem::shape() const{
    QPainterPath path;
    path.addRect(this->boundingRect());
    return path;
}

int QNCommonItem::type() const{
    return QNCommonItem::Type;
}

void QNCommonItem::toggleShowContent(){
    if(this->icon != nullptr){
        bool showContent = this->icon->isShowingContent();
        this->icon->setShowContent(!showContent);
        this->setShowContent(!showContent);
    }
}
