/**
 ** @file qnmodelitem.h
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

#ifndef QNMODELITEM_H
#define QNMODELITEM_H

#include "qncommonitem.h"
#include "qnglobalexport.h"
#include "qndrawtypes.h"

/**
 * @brief The QNModelItem class models a sub-gruop of items in the scene
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNModelItem: public QNCommonItem {
    Q_OBJECT

private:
    Q_DISABLE_COPY(QNModelItem)

public:
    /**
     * @brief identificates user-defined types
     */
    enum {
        Type /** QNModelItem type value */ = QNCommonItem::Type + 3
    };

    /**
     * @brief QNModelItem constructor
     * @param aModel the corresponding submodel object
     * @param aModelMenu the contextual menu associated to this submodel
     * @param aStyleMgr the reference to the style manager
     * @param aParent the parent graphic item
     * @warning passing null pointers as parameters to this constructor (except for aParent and aStyleMgr) could
     * cause a segmentation fault later on in the program execution
     */
    QNModelItem(QNModelPtr aModel, QMenuPtr aModelMenu, QNStyleManagerPtr aStyleMgr = 0, QGraphicsItemPtr aParent = 0);

    /**
     * @brief updates the corresponding submodel object
     * @param aModel the new submodel object (optional)
     * @note if aModel is null and no previous value is set, no update occurs
     */
    void updateModel(QNModelPtr aModel = 0);

    /**
     * @brief updates the contextual menu associated to this submodel
     * @param aCtxMenu the new contextual menu
     * @note if aCtxMenu is null, the default menu will be used
     */
    void updateCtxMenu(QMenuPtr aCtxMenu) Q_DECL_OVERRIDE;

    /**
     * @brief retrieves the corresponding model object
     * @return the corresponding model object
     */
    QNModelPtr getModel();

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
#endif // QNMODELITEM_H
