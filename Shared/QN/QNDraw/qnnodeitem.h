/**
 ** @file qnnodeitem.h
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

#ifndef QNNODEITEM_H
#define QNNODEITEM_H

#include "qncommonitem.h"
#include "qnglobalexport.h"
#include "qndrawtypes.h"


/**
 * @brief The QNNodeItem class models a network's node in the scene
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNodeItem: public QNCommonItem {
    Q_OBJECT

private:
    Q_DISABLE_COPY(QNNodeItem)

    /**
     * @brief the list of edges attached to this node
     */
    ScopedEdgeItemList edges;

public:
    /**
     * @brief identificates user-defined types
     */
    enum {
        Type /** QNNodeItem type value */ = QNCommonItem::Type + 1
    };

    /**
     * @brief QNNodeItem constructor
     * @param aNode the corresponding node icon
     * @param aNodeMenu the contextual menu associated to this node
     * @param aStyleMgr the reference to the style manager
     * @param aParent the parent graphic item
     * @warning passing null pointers as parameters to this constructor (except for aParent and aStyleMgr) could
     * cause a segmentation fault later on in the program execution
     */
    QNNodeItem(QNNodeIconPtr aNode, QMenuPtr aNodeMenu, QNStyleManagerPtr aStyleMgr = 0, QGraphicsItemPtr aParent = 0);

    /**
     * @brief QNNodeItem destructor
     */
    ~QNNodeItem();

    /**
     * @brief updates the corresponding node icon
     * @param aNode the new node icon (optional)
     * @note if aNode is null and no previous value is set, no update occurs
     */
    void updateNode(QNNodeIconPtr aNode = 0);

    /**
     * @brief updates the contextual menu associated to this node
     * @param aCtxMenu the new contextual menu
     * @note if aCtxMenu is null, the default menu will be used
     */
    void updateCtxMenu(QMenuPtr aCtxMenu) Q_DECL_OVERRIDE;

    /**
     * @brief retrieves the corresponding node icon object
     * @return the corresponding node icon object
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeIconPtr getNode();

    /**
     * @brief retrieves the list of all edges attached to this node
     * @return the list of all edges attached to this node
     * @note if no edges are attached, the returned pointer will be null
     */
    QNEdgeItemListPtr getEdges();

    /**
     * @brief attaches an edge to this node item
     * @param aEdge the edge to be attached
     */
    void addEdge(QNEdgeItemPtr aEdge);

    /**
     * @brief detaches the passed edge from this node item
     * @param aEdge the edge to be detached
     */
    void removeEdge(QNEdgeItemPtr aEdge);

    /**
     * @brief updates showContent properties of this icon
     * @param aValue the new value for the showContent property
     */
    void setShowContent(const bool aValue) Q_DECL_OVERRIDE;

    /**
     * @brief retrieves the width of the bounding rectangle
     * @return the width of the bounding rectangle
     */
    int getWidth() const Q_DECL_OVERRIDE;

    /**
     * @brief retrieves the height of the bounding rectangle
     * @return the height of the bounding rectangle
     */
    int getHeight() const Q_DECL_OVERRIDE;

    /**
     * @brief type custom implementation
     * @return the item's type
     * @see QGraphicsTextItem
     */
    int type() const Q_DECL_OVERRIDE;

    /**
     * @brief paint custom implementation
     * @param aPainter the painter object
     * @param aOption item options
     * @param aWidget NOT USED
     * @see QGraphicsTextItem
     */
    void paint(QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget = Q_NULLPTR) Q_DECL_OVERRIDE;
};

#endif // QNNODEITEM_H
