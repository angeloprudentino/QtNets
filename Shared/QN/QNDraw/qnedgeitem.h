/**
 ** @file qnedgeitem.h
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

#ifndef QNARROWITEM_H
#define QNARROWITEM_H

#include "qnglobalexport.h"
#include "qndrawtypes.h"
#include "qntypes.h"
#include "qnstyletypes.h"
#include "qncommonitem.h"

/**
 * @brief The QNEdgeItem class models an edge between two nodes in the scene
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNEdgeItem: public QGraphicsLineItem {
private:
    Q_DISABLE_COPY(QNEdgeItem)

    /**
     * @brief the corresponding edge object
     */
    QNEdgeStylePtr edge;

    /**
     * @brief the contextual menu associated to this edge
     */
    QMenuPtr ctxMenu;

    /**
     * @brief the source node item
     */
    QNNodeItemPtr srcNode;

    /**
     * @brief the destination node item
     */
    QNNodeItemPtr dstNode;

    /**
     * @brief style manager reference
     */
    QNStyleManagerPtr styleMgr;

    /**
     * @brief the cursor point in scene coordinates
     * tracked during edge movement
     */
    QPointF mousePoint;

//    /**
//     * @brief edge's label
//     */
//    QGraphicsTextItemPtr label;

    /**
     * @brief determines if the edge source can be moved
     */
    bool moveSrcMode = false;

    /**
     * @brief determines if the edge destination can be moved
     */
    bool moveDstMode = false;

    /**
     * @brief determine the line to be drawn for this edge
     * @return the line to draw
     */
    QLineF determineLine();

    /**
     * @brief tells if the edge source can be moved
     * @param aPoint the current cursor point
     * @return true if the edge source can be moved; false otherwise
     */
    bool isMoveSrcMode(QPointF aPoint) const;

    /**
     * @brief tells if the edge destination can be moved
     * @param aPoint the current cursor point
     * @return true if the edge destination can be moved; false otherwise
     */
    bool isMoveDstMode(QPointF aPoint) const;

    /**
     * @brief manages the movement of the edge to other nodes
     */
    void manageMoveEdge();

protected:
    /**
     * @brief hoverEnterEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void hoverEnterEvent(QGraphicsSceneHoverEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief hoverMoveEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void hoverMoveEvent(QGraphicsSceneHoverEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief hoverLeaveEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief contextMenuEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mousePressEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void mousePressEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseMoveEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void mouseMoveEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseReleaseEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsLineItem
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

public:
    /**
     * @brief identifies user-defined types
     */
    enum {
        Type /** QNEdgeItem type value */ = UserType + 16
    };

    /**
     * @brief QNEdgeItem constructor
     * @param aSrcNode the source node item
     * @param aDstNode the destination node item
     * @param aEdge the corresponding edge object
     * @param aEdgeMenu the contextual menu associated to this edge
     * @param aStyleMgr the reference to the style manager
     * @param aParent the parent graphic item for this edge
     * @note to make things works properly, aParent should be the same for the source and
     * the destination node too
     * @warning passing null pointers as parameters to this constructor (except for aParent) could
     * cause a segmentation fault later on in the program execution
     */
    QNEdgeItem(QNNodeItemPtr aSrcNode, QNNodeItemPtr aDstNode, QNEdgeStylePtr aEdge,
               QMenuPtr aEdgeMenu, QNStyleManagerPtr aStyleMgr = 0, QGraphicsItemPtr aParent = 0);

    /**
     * @brief QNEdgeItem destructor
     */
    ~QNEdgeItem();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNEdgeItem& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNEdgeItem& aOther) const;

    /**
     * @brief updates the corresponding edge object
     * @param aEdge the new edge object (optional)
     * @note if aEdge is null and no previous value is set, no update occurs
     */
    void updateEdge(QNEdgeStylePtr aEdge = 0);

    /**
     * @brief updates the edge's contextual menu
     * @param aEdgeMenu the new contextual menu
     * @note if aEdgeMenu is null, the default menu will be used
     */
    void updateEdgeMenu(QMenuPtr aEdgeMenu);

    /**
     * @brief updates the edge in the scene
     */
    void updateEdgePos();

    /**
     * @brief retrieves the context menu associated to this edge
     * @return the context menu
     * @note: if no context menu is associated, the returned pointer will be null
     */
    QMenuPtr getCtxMenu() const;

    /**
     * @brief retrieves the source node reference
     * @return the source node reference
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeItemPtr getSrcNode() const;

    /**
     * @brief retrieves the destination node reference
     * @return the destination node reference
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeItemPtr getDstNode() const;

    /**
     * @brief retrieves the corresponding edge object
     * @return the corresponding edge object
     */
    QNEdgeStylePtr getEdge() const;

    /**
     * @brief boundingRect custom implementation
     * @return the bounding rectangle
     * @see QGraphicsLineItem
     */
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    /**
     * @brief type custom implementation
     * @return the item's type
     * @see QGraphicsLineItem
     */
    int type() const Q_DECL_OVERRIDE;

    /**
     * @brief paint custom implementation
     * @param aPainter the painter object
     * @param aOption item options
     * @param aWidget NOT USED
     * @see QGraphicsLineItem
     */
    void paint(QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget = Q_NULLPTR) Q_DECL_OVERRIDE;
};

#endif // QNARROWITEM_H
