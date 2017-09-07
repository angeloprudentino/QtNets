/**
 ** @file qncommonicon.h
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

#ifndef QNCOMMONICON_H
#define QNCOMMONICON_H

#include "qntypes.h"
#include "qnglobalexport.h"
#include "qnproperties.h"

/**
 * @brief The QNCommonIcon class models the common attributes among icons and texboxes
 * to manage them in a more abstract way
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNCommonIcon: public QNProperties{
private:
    Q_DISABLE_COPY(QNCommonIcon)

    /**
     * @brief says if this icon should show its content
     */
    bool showContent = true;

    /**
     * @brief number of shown caracters of the label
     */
    int labelLen = -1;

    /**
     * @brief property to be displayed in the item
     */
    QString propToShow;

    /**
     * @brief an optional comment associated to the icon
     */
    QString comment;

    /**
     * @brief the icon's position in the plot
     */
    ScopedQNPosition pos;

    /**
     * @brief the icon's font
     */
    ScopedQNFont font;

public:
    /**
     * @brief QNCommonIcon constructor
     */
    QNCommonIcon();

    /**
     * @brief QNCommon destructor
     */
    virtual ~QNCommonIcon();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNCommonIcon& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNCommonIcon& aOther) const;

    /**
     * @brief retrieves the icon's comment
     * @return the icon's comment
     */
    QString getComment() const;

    /**
     * @brief says if this icon is showing its content
     * @return true if this icon is showing its content, false otherwise
     */
    bool isShowingContent() const;

    /**
     * @brief retrieves the property to be displayed by the item
     * @return the property to be displayed by the item
     */
    QString getPropToShow() const;

    /**
     * @brief retrieves the number of characters shown for the label
     * @return the number of characters shown for the label
     */
    int getLabelLen() const;

    /**
     * @brief retrieves the icon's position in the plot
     * @return the icon's position in the plot (live reference)
     * @note if the icon's position does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNPositionPtr getPosition() const;

    /**
     * @brief retrieves the icon's font
     * @return the icon's font (live reference)
     * @note if the icon's font does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNFontPtr getFont() const;

    /**
     * @brief updates the icon's comment
     * @param aValue the new value for this icon's comment
     */
    void setComment(const QString& aValue);

    /**
     * @brief updates showContent properties of this icon
     * @param aValue the new value for the showContent property
     */
    void setShowContent(const bool aValue);

    /**
     * @brief updates the property to be displayed by the item modeling this icon
     * @param aProp the property to be displayed
     */
    void updatePropToShow(const QString& aProp);

    /**
     * @brief updates the number of characters shown for the label
     * @param aValue the number of characters to be shown
     * @note if aValue is negative, the full label will be shown
     */
    void setLabelLen(int aValue = -1);

    /**
     * @brief updates the icon's position in the plot
     * @param aValue the new icon's position in the plot
     * @note if aValue is null, the request will be ignored
     * @warning after the call, aValue becomes owned by this object, which controls its lifcycle
     */
    void setPosition(QNPositionPtr aValue);

    /**
     * @brief updates the icon's font
     * @param aValue the new value for the icon's font
     * @note if aValue is null, the request will be ignored
     * @warning after the call, aValue becomes owned by this object, which controls its lifcycle
     */
    void setFont(QNFontPtr aValue);

    /**
     * @brief returns the list of all properties' keys
     * @return the list of all properties' keys
     */
    QStringList getAllKeys() const Q_DECL_OVERRIDE;

    /**
     * @brief retrieves properties by name
     * @param aKey the name of the required property
     * @return the required property
     * @note if the property is not found, the returned variant object is invalid
     * @see QVariant#isValid()
     */
    QVariant getProperty(const QString& aKey) const Q_DECL_OVERRIDE;

    /**
     * @brief stores the provided property using the provided key
     * @param aKey the property's key
     * @param aValue the property's value
     * @note if aKey is empty, the request will be ignored
     * @result the error object resulting from this operation
     */
    QNError setProperty(const QString& aKey, const QVariant& aValue) Q_DECL_OVERRIDE;
};

#endif // QNCOMMONICON_H
