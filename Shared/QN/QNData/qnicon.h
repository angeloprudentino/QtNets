/**
 ** @file qnicon.h
 ** @author Angelo Prudentino
 ** @date 2016-11-17
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

#ifndef QNICON_H
#define QNICON_H

#include "qntypes.h"
#include "qnposition.h"
#include "qnfont.h"
#include "qngraphicconst.h"
#include "qnglobalexport.h"
#include "qncommonicon.h"
#include "qnstylable.h"
#include "qnerror.h"


/**
 * @brief The QNIcon class models a generic icon that can be drawn on the plot
 * @note this class is not copyable
 */
class QNIcon: public QNCommonIcon, public QNStylable {
private:
    Q_DISABLE_COPY(QNIcon)

private:
    /**
     * @brief icon's unique id
     */
    QString id;

    /**
     * @brief icon's name
     */
    QString name;

    /**
     * @brief icon's background color
     * @note default is white
     */
    QString color;

    /**
     * @brief icon's outline color
     * @note default is black
     */
    QString outlineColor;

    /**
     * @brief icon's outline width
     * @note default is 1
     */
    int outlineWidth;

    /**
     * @brief reference to the container model
     */
    QNModelPtr container;

public:
    /**
     * @brief QNIcon constructor
     * @param aContainer teh reference to the container model
     * @warning passing null pointer as parameter to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNIcon(QNModelPtr aContainer);

    /**
     * @brief QNIcon destructor
     */
    virtual ~QNIcon();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNIcon& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNIcon& aOther) const;

    /**
     * @brief retrieves the icon's id
     * @return the icon's id
     */
    QString getId() const;

    /**
     * @brief retrieves the icon's name
     * @return the icon's name
     */
    QString getName() const;

    /**
     * @brief retrieves the icon's background color
     * @return the icon's background color
     */
    QString getColor() const;

    /**
     * @brief retrieves the icon's outline color
     * @return the icon's outline color
     */
    QString getOutlineColor() const;

    /**
     * @brief retrieves the icon's outline width
     * @return the icon's outline width
     */
    int getOutlineWidth() const;

    /**
     * @brief retrieves the reference to the container model
     * @return the reference to the container model
     * @note if the icon's container does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNModelPtr getContainer() const;

    /**
     * @brief updates the icon's id
     * @param value the new value for this icon's id
     * @return the error object resulting from this operation
     * @note if value is empty, the request will be ignored
     */
    virtual QNError setId(const QString& value);

    /**
     * @brief updates the icon's name
     * @param value the new value for this icon's name
     * @note if value is empty, the request will be ignored
     */
    void setName(const QString& value);

    /**
     * @brief updates the icon's background color
     * @param value the new value for this icon's background color
     * @note if value is empty, the request will be ignored
     */
    void setColor(const QString& value);

    /**
     * @brief updates the icon's outline color
     * @param value the new value for this icon's outline color
     * @note if value is empty, the request will be ignored
     */
    void setOutlineColor(const QString& value);

    /**
     * @brief updates the icon's outline width
     * @param value the new value for this icon's outline width
     */
    void setOutlineWidth(int value);

    /**
     * @brief updates the icon's container reference
     * @param value the new  icon's container
     * @note if value is null, the request will be ignored
     */
    void setContainer(QNModelPtr value);

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

#endif // QNICON_H
