/**
 ** @file qncommonitem.h
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

#ifndef QNCOMMONITEM_H
#define QNCOMMONITEM_H

#include "qnglobalexport.h"
#include "qndrawtypes.h"
#include "qntypes.h"
#include "qnstyletypes.h"
#include <QTextDocument>


/**
 * @brief The QNCommonItem class models a generic item in the scene
 * @note this class is not copyable
 */
class QNCommonItem: public QGraphicsTextItem {
    Q_OBJECT

private:
    Q_DISABLE_COPY(QNCommonItem)

    /**
     * @brief tells if the node is resize mode from top left corner
     */
    bool topLeftResize = false;

    /**
     * @brief tells if the node is resize mode from top right corner
     */
    bool topRightResize = false;

    /**
     * @brief tells if the node is resize mode from bottom left corner
     */
    bool bottomLeftResize = false;

    /**
     * @brief tells if the node is resize mode from bottom right corner
     */
    bool bottomRightResize = false;

    /**
     * @brief manages resize operations
     * @param aPoint the point to which resize
     * @return true if the resize took place
     * @note this method emits QNNetworkScene#modified() signal
     */
    bool manageResize(QPointF aPoint);

protected:
    /**
     * @brief the corresponding common item (could be a model, a node or a textbox)
     */
    QNCommonIconPtr icon;

    /**
     * @brief the contextual menu associated to this item
     */
    QMenuPtr ctxMenu;

    /**
     * @brief style manager reference
     */
    QNStyleManagerPtr styleMgr;

    /**
     * @brief menu action to toggle showContent property
     */
    QActionPtr showHideAction;

    /**
     * @brief the item's label
     */
    QTextDocument label;

    /**
     * @brief the item's truncated label
     */
    QTextDocument labelTrunc;

    /**
     * @brief tells if resize mode from top left corner could be activated
     * @param aPoint the current cursor point
     * @return true if resize mode from top left corner could be activated; false otherwise
     */
    bool isTopLeftResize(QPointF aPoint) const;

    /**
     * @brief tells if resize mode from top right corner could be activated
     * @param aPoint the current cursor point
     * @return true if resize mode from top right corner could be activated; false otherwise
     */
    bool isTopRightResize(QPointF aPoint) const;

    /**
     * @brief tells if resize mode from bottom left corner could be activated
     * @param aPoint the current cursor point
     * @return true if resize mode from bottom left corner could be activated; false otherwise
     */
    bool isBottomLeftResize(QPointF aPoint) const;

    /**
     * @brief tells if resize mode from bottom right corner could be activated
     * @param aPoint the current cursor point
     * @return true if resize mode from bottom right corner could be activated; false otherwise
     */
    bool isBottomRightResize(QPointF aPoint) const;

    /**
     * @brief retrieves the top left point of the bounding rectangle with respect to the whole plot
     * @return the (absolute) top left point of the bounding rectangle
     */
    QPoint getTopLeft() const;

    /**
     * @brief retrieves the bottom right point of the bounding rectangle with respect to the whole plot
     * @return the (absolute) bottom right point of the bounding rectangle
     */
    QPoint getBottomRight() const;

    /**
     * @brief itemChange custom implementation
     * @param aChange the kind of change occured
     * @param aValue the entity of the change
     * @return aValue is returned
     * @see QGraphicsTextItem
     * @note this method emits QNNetworkScene#modified() signal
     */
    QVariant itemChange(GraphicsItemChange aChange, const QVariant& aValue) Q_DECL_OVERRIDE;

    /**
     * @brief mousePressEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void mousePressEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseMoveEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void mouseMoveEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseReleaseEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseDoubleClickEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief focusOutEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void focusOutEvent(QFocusEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief hoverEnterEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void hoverEnterEvent(QGraphicsSceneHoverEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief hoverMoveEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void hoverMoveEvent(QGraphicsSceneHoverEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief hoverLeaveEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief contextMenuEvent custom implementation
     * @param aEvent the delivered event
     * @see QGraphicsTextItem
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief updates item position in the scene
     * @param aLeft top x coordinate
     * @param aTop top y coordinate
     * @param aRight bottom x coordinate
     * @param aBottom bottom y coordinate
     */
    void updatePosition(const int aLeft, const int aTop, const int aRight, const int aBottom);

    /**
     * @brief updates the content of the label
     * @param aContent the new content
     */
    void updateLabel(const QString& aContent);

public:
    /**
     * @brief identificates user-defined types
     */
    enum {
        Type /** QNCommonItem type value */ = UserType + 17
    };

    /**
     * @brief QNCommonItem constructor
     * @param aStyleMgr the reference to the style manager
     * @param aParent the parent graphic item
     */
    QNCommonItem(QNStyleManagerPtr aStyleMgr = 0, QGraphicsItemPtr aParent = 0);

    /**
     * @brief QNCommonItem destructor
     */
    virtual ~QNCommonItem();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNCommonItem& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNCommonItem& aOther) const;

    /**
     * @brief retrieves the context menu associated to this item
     * @return the context menu
     * @note: if no context menu is associated, the returned pointer will be null
     */
    QMenuPtr getCtxMenu() const;

    /**
     * @brief updates the contextual menu associated to this item
     * @param aCtxMenu the new contextual menu
     * @note if aCtxMenu is null, the default menu will be used
     */
    virtual void updateCtxMenu(QMenuPtr aCtxMenu) = 0;

    /**
     * @brief updates text font options
     * @param aFont the font to be set;
     */
    void updateFont(QNFontPtr aFont);

    /**
     * @brief says if this icon is showing its content
     * @return true if this icon is showing its content, false otherwise
     */
    bool isShowingContent() const;

    /**
     * @brief updates showContent properties of this icon
     * @param aValue the new value for the showContent property
     */
    virtual void setShowContent(const bool aValue) = 0;

    /**
     * @brief retrieves the property to be displayed by the item
     * @return the property to be displayed by the item
     */
    QString getPropToShow() const;

    /**
     * @brief updates the property to be displayed by the item
     * @param aProp the property to be displayed
     * @return true if the property can be used; false otherwise
     */
    bool updatePropToShow(const QString& aProp);

    /**
     * @brief retrieves this item's label
     * @return this item's label
     */
    QTextDocument& getLabel();

    /**
     * @brief retrieves the number of characters shown for the label
     * @return the number of characters shown for the label
     */
    int getLabelLen() const;

    /**
     * @brief updates the number of characters shown for the label
     * @param aValue the number of characters to be shown
     * @note if aValue is negative, the full label will be shown
     */
    void setLabelLen(int aValue = -1);

    /**
     * @brief retrieves the width of the bounding rectangle
     * @return the width of the bounding rectangle
     */
    virtual int getWidth() const = 0;

    /**
     * @brief retrieves the height of the bounding rectangle
     * @return the height of the bounding rectangle
     */
    virtual int getHeight() const = 0;

    /**
     * @brief retrieves the center point of the bounding rectangle
     * @return the top center of the bounding rectangle
     */
    QPoint getCenter() const;

    /**
     * @brief scales the item by the given value
     * @param aValue the scale value
     * @note the scale operation will be performed only if aValue is greater than 0
     * @note this method emits QNNetworkScene#modified() signal
     */
    void scale(const double aValue);

    /**
     * @brief boundingRect custom implementation
     * @return the bounding rectangle
     * @see QGraphicsTextItem
     */
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    /**
     * @brief shape custom implementation
     * @return the exact shape as a path
     * @see QGraphicsTextItem
     */
    QPainterPath shape() const Q_DECL_OVERRIDE;

    /**
     * @brief type custom implementation
     * @return the item's type
     * @see QGraphicsTextItem
     */
    int type() const Q_DECL_OVERRIDE;

public slots:
    /**
     * @brief toggles showContent properties of this icon
     */
    void toggleShowContent();
};

#endif // QNCOMMONITEM_H
