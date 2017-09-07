/**
 ** @file qntextboxitem.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-10
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

#include "qntextboxitem.h"
#include "qntextbox.h"
#include "qngraphicconst.h"
#include "qnnetworkscene.h"
#include "qnstylemanager.h"
#include "qndrawconst.h"
#include <QTextCursor>


#define TEXTBOX_MARGIN 3
#define TEXTBOX_ARC 13


QNTextBoxItem::QNTextBoxItem(QNTextBoxPtr aTextbox, QMenuPtr aTextboxMenu, QNStyleManagerPtr aStyleMgr, QGraphicsItemPtr aParent): QNCommonItem(aStyleMgr, aParent){
    this->updateTextbox(aTextbox);
    this->updateCtxMenu(aTextboxMenu);
}

void QNTextBoxItem::updateTextbox(QNTextBoxPtr aTextbox){
    if(aTextbox != nullptr)
        this->icon = aTextbox;

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
        QString comment = icon->getComment();
        this->setToolTip(comment);

        //position
        QPoint tl = this->getTopLeft();
        this->setPos(tl);
        this->setZValue(0.3);

        if(this->scene())
            this->scene()->update();
    }
}

void QNTextBoxItem::updateCtxMenu(QMenuPtr aCtxMenu){
    //remove old actions
    if(this->ctxMenu != nullptr){
        foreach (QActionPtr a, this->ctxMenu->actions())
            this->ctxMenu->removeAction(a);
    }

    //add new actions
    this->showHideAction->setText("Note expanded");
    this->ctxMenu->addAction(this->showHideAction);

    if(aCtxMenu != nullptr){
        foreach (QActionPtr a, aCtxMenu->actions())
            this->ctxMenu->addAction(a);
    }
}

QNTextBoxPtr QNTextBoxItem::getTextbox(){
    return dynamic_cast<QNTextBoxPtr>(this->icon);
}

void QNTextBoxItem::setShowContent(const bool aValue){
    this->icon->setShowContent(aValue);
    this->showHideAction->setChecked(aValue);

    //update content
    this->updateTextbox();
    if(aValue){
        this->showHideAction->setToolTip(tr("Click to hide textbox's details"));
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
    else{
        this->showHideAction->setToolTip(tr("Click to show textbox's details"));
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

int QNTextBoxItem::getWidth() const{
    QPoint tl = this->getTopLeft();
    QPoint br = this->getBottomRight();

    int boxW = br.x() - tl.x();
    if(!this->isShowingContent())
        boxW = (boxW < TEXTBOX_DEFAULT_W/2)? boxW: TEXTBOX_DEFAULT_W/2;

    int textW = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().width(): this->label.size().width();
    return (textW+TEXTBOX_ARC > boxW)? textW+TEXTBOX_ARC : boxW;
}

int QNTextBoxItem::getHeight() const{
    QPoint tl = this->getTopLeft();
    QPoint br = this->getBottomRight();

    int boxH = br.y() - tl.y();
    if(!this->isShowingContent())
        boxH = (boxH < TEXTBOX_DEFAULT_H/2)? boxH: TEXTBOX_DEFAULT_H/2;

    int textH = ((this->getLabelLen()>-1 && this->textInteractionFlags() == Qt::NoTextInteraction) || !this->isShowingContent())?
                    this->labelTrunc.size().height(): this->label.size().height();

    return (textH > boxH)? textH : boxH;
}

int QNTextBoxItem::type() const{
    return QNTextBoxItem::Type;
}

void QNTextBoxItem::paint(QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget){
    if(this->icon != nullptr){
        QNError err(INVALID_STYLE_STATE, "");
        if(this->styleMgr != nullptr)
            err = this->styleMgr->paint(this, aPainter, aOption, aWidget);

        if(err.getCode() == INVALID_STYLE_STATE){
            aPainter->setRenderHint(QPainter::Antialiasing, true);
            aPainter->setRenderHint(QPainter::TextAntialiasing, true);

            //color
            QColor fillColor = QColor("#" + QString(DEFAULT_BG_COLOR));
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
            QColor outColor = QColor("#" + QString(DEFAULT_FONT_COLOR));
            pen.setColor(outColor);
            aPainter->setPen(pen);

            //shape
            int w = this->getWidth();
            int h = this->getHeight();
            QPoint topLeft(0,0);
            QPoint bottomLeft(0,h);
            QPoint bottomRight(w,h);
            QPoint topRight1(w,TEXTBOX_ARC);
            QPoint topRight2(w-TEXTBOX_ARC,0);
            QPolygon body;
            body << topLeft << bottomLeft << bottomRight << topRight1 << topRight2;
            aPainter->drawPolygon(body);
            QPolygon tri;
            tri << topRight2 << QPoint(w-TEXTBOX_ARC,TEXTBOX_ARC) << topRight1;
            aPainter->drawPolygon(tri);
        }
    }

    QGraphicsTextItem::paint(aPainter, aOption, aWidget);
}
