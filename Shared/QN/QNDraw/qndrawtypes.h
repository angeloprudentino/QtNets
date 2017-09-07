/**
 ** @file qndrawtypes.h
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

#ifndef QNDRAWTYPES_H
#define QNDRAWTYPES_H

#include <QList>
#include <QScopedPointer>
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QFocusEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QTextDocument>
#include <QMenu>
#include <QAction>
#include <QPointF>


class QNCommonItem;
class QNNodeItem;
class QNTextBoxItem;
class QNModelItem;
class QNEdgeItem;
class QNNetworkScene;
class QNNetworkView;

/**
 * @brief Represents a pointer to a float-precise point in the scene
 * @see QWidget
 */
typedef QPointF* QPointFPtr;

/**
 * @brief Represents a pointer to a widget
 * @see QWidget
 */
typedef QWidget* QWidgetPtr;

/**
 * @brief Represents a pointer to a menu object
 * @see QMenu
 */
typedef QMenu* QMenuPtr;

/**
 * @brief Represents a pointer to a menu action object
 * @see QAction
 */
typedef QAction* QActionPtr;

/**
 * @brief Represents a pointer to a view instance
 * @see QGraphicsView
 */
typedef QGraphicsView* QGraphicsViewPtr;

/**
 * @brief Represents a pointer to a graphic text item
 * @see QGraphicsTextItem
 */
typedef QGraphicsTextItem* QGraphicsTextItemPtr;

/**
 * @brief Represents a pointer to a graphic line item
 * @see QGraphicsLineItem
 */
typedef QGraphicsLineItem* QGraphicsLineItemPtr;

/**
 * @brief Represents a pointer to a graphic item
 * @see QGraphicsItem
 */
typedef QGraphicsItem* QGraphicsItemPtr;

/**
 * @brief Represent a list of graphic items
 * @see QGraphicsItemPtr
 */
typedef QList<QGraphicsItemPtr> QGraphicsItemList;

/**
 * @brief Represent a unique pointer to a list of graphic items
 * @see QGraphicsItemList
 */
typedef QScopedPointer<QGraphicsItemList> ScopedGraphicsItemList;

/**
 * @brief Represents a pointer to a graphic scene
 * @see QGraphicsScene
 */
typedef QGraphicsScene* QGraphicsScenePtr;

/**
 * @brief Represents a pointer to a graphic scene's context menu event
 * @see QGraphicsSceneContextMenuEvent
 */
typedef QGraphicsSceneContextMenuEvent* QGraphicsSceneContextMenuEventPtr;

/**
 * @brief Represents a pointer to a graphic scene's mouse hover event
 * @see QGraphicsSceneHoverEvent
 */
typedef QGraphicsSceneHoverEvent* QGraphicsSceneHoverEventPtr;

/**
 * @brief Represents a pointer to a graphic scene's mouse event
 * @see QGraphicsSceneMouseEvent
 */
typedef QGraphicsSceneMouseEvent* QGraphicsSceneMouseEventPtr;

/**
 * @brief Represents a pointer to a graphic scene's wheel event
 * @see QWheelEvent
 */
typedef QWheelEvent* QWheelEventPtr;

/**
 * @brief Represents a pointer to a graphic scene's focus event
 * @see QFocusEvent
 */
typedef QFocusEvent* QFocusEventPtr;

/**
 * @brief Represents a pointer to a painter object
 * @see QPainter
 */
typedef QPainter* QPainterPtr;

/**
 * @brief Represents a const pointer to a graphic item's option object
 * @see QStyleOptionGraphicsItem
 */
typedef const QStyleOptionGraphicsItem* QStyleOptionGraphicsItemConstPtr;

/**
 * @brief Represent a pointer to a network view
 * @see QNNetworkView
 */
typedef QNNetworkView* QNNetworkViewPtr;
/**

 * @brief Represent a pointer to a network scene
 * @see QNNetworkScene
 */
typedef QNNetworkScene* QNNetworkScenePtr;

/**
 * @brief Represent a pointer to a common item
 * @see QNCommonItem
 */
typedef QNCommonItem* QNCommonItemPtr;

/**
 * @brief Represent a pointer to a node item
 * @see QNNodeItem
 */
typedef QNNodeItem* QNNodeItemPtr;

/**
 * @brief Represent a list of node items
 * @see QNNodeItemPtr
 */
typedef QList<QNNodeItemPtr> QNNodeItemList;

/**
 * @brief Represent a pointer to a textbox item
 * @see QNTextBoxItem
 */
typedef QNTextBoxItem* QNTextBoxItemPtr;

/**
 * @brief Represent a list of textbox items
 * @see QNTextBoxItemPtr
 */
typedef QList<QNTextBoxItemPtr> QNTextBoxItemList;

/**
 * @brief Represent a pointer to a model item
 * @see QNModelItem
 */
typedef QNModelItem* QNModelItemPtr;

/**
 * @brief Represent a list of model items
 * @see QNModelItemPtr
 */
typedef QList<QNModelItemPtr> QNModelItemList;

/**
 * @brief Represent a pointer to an edge item
 * @see QNEdgeItem
 */
typedef QNEdgeItem* QNEdgeItemPtr;

/**
 * @brief Represent a list of edge items
 * @see QNEdgeItemPtr
 */
typedef QList<QNEdgeItemPtr> QNEdgeItemList;

/**
 * @brief Represent a pointer to a list of edge items
 * @see QNEdgeItemList
 */
typedef QNEdgeItemList* QNEdgeItemListPtr;

/**
 * @brief Represent a unique pointer to a list of edge items
 * @see QNEdgeItemList
 */
typedef QScopedPointer<QNEdgeItemList> ScopedEdgeItemList;

#endif // QNDRAWTYPES_H
