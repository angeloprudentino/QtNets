/**
 ** @file qntextboxitem.h
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

#ifndef QNTEXTBOXITEM_H
#define QNTEXTBOXITEM_H

#include "qnglobalexport.h"
#include "qncommonitem.h"
#include "qndrawtypes.h"

/**
 * @brief The QNTextBoxItem class models a textbox for annotations in the scene
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNTextBoxItem: public QNCommonItem {
    Q_OBJECT

private:
    Q_DISABLE_COPY(QNTextBoxItem)

public:
    /**
     * @brief identificates user-defined types
     */
    enum {
        Type /** QNTextBoxItem type value */ = QNCommonItem::Type + 2
    };

    /**
     * @brief QNTextBoxItem constructor
     * @param aTextbox the corresponding textbox object
     * @param aTextboxMenu the contextual menu associated to this textbox
     * @param aStyleMgr the reference to the style manager
     * @param aParent the parent graphic item
     * @warning passing null pointers as parameters to this constructor (except for aParent and aStyleMgr) could
     * cause a segmentation fault later on in the program execution
     */
    QNTextBoxItem(QNTextBoxPtr aTextbox, QMenuPtr aTextboxMenu, QNStyleManagerPtr aStyleMgr = 0, QGraphicsItemPtr aParent = 0);

    /**
     * @brief updates the corresponding textbox object
     * @param aTextbox the new textbox object (optional)
     * @note if aTextbox is null and no previous value is set, no update occurs
     */
    void updateTextbox(QNTextBoxPtr aTextbox = 0);

    /**
     * @brief updates the contextual menu associated to this textbox
     * @param aCtxMenu the new contextual menu
     * @note if aCtxMenu is null, the default menu will be used
     */
    void updateCtxMenu(QMenuPtr aCtxMenu) Q_DECL_OVERRIDE;

    /**
     * @brief retrieves the corresponding textbox object
     * @return the corresponding textbox object
     */
    QNTextBoxPtr getTextbox();

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

#endif // QNTEXTBOXITEM_H
