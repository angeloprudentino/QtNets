/**
 ** @file qnnetworkview.cpp
 ** @author Angelo Prudentino
 ** @date 2017-01-08
 **
 ** Copyright 2017 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
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

#include "qnnetworkview.h"
#include <qmath.h>

#define Z_FACTOR 10
#define WHEEL_ZOOM 0.3
#define R_MAX 360
#define R_MIN -360


void QNNetworkView::updateMatrix(){
    double scale = qPow(2, this->zoomLevel);

    QMatrix matrix;
    matrix.scale(scale, scale);
    matrix.rotate(this->rotateLevel);

    this->setMatrix(matrix);
}

void QNNetworkView::wheelEvent(QWheelEventPtr aEvent){
    if (aEvent->modifiers() & Qt::ControlModifier) {
        if (aEvent->delta() > 0)
            this->zoomIn(WHEEL_ZOOM);
        else
            this->zoomOut(WHEEL_ZOOM);
        aEvent->accept();
    } else
        QGraphicsView::wheelEvent(aEvent);
}

QNNetworkView::QNNetworkView(QNNetworkScenePtr aScene, QWidgetPtr aParent): QGraphicsView((QGraphicsScenePtr)aScene, aParent){
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setRenderHint(QPainter::TextAntialiasing, true);
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setInteractive(true);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void QNNetworkView::restoreZoom(){
    this->zoomLevel = 0;
    this->updateMatrix();
}

void QNNetworkView::zoomIn(const double aValue){
    this->zoomLevel += aValue;
    this->updateMatrix();
}

void QNNetworkView::zoomOut(const double aValue){
    this->zoomLevel -= aValue;
    this->updateMatrix();
}

void QNNetworkView::restoreRotate(){
    this->rotateLevel = 0;
    this->updateMatrix();
}

void QNNetworkView::rotateRight(const double aValue){
    double tmp = this->rotateLevel + aValue;
    this->rotateLevel = (tmp <= R_MAX)? tmp: R_MAX;
    this->updateMatrix();
}

void QNNetworkView::rotateLeft(const double aValue){
    double tmp = this->rotateLevel - aValue;
    this->rotateLevel = (tmp >= R_MIN)? tmp: R_MIN;
    this->updateMatrix();
}
