/**
 ** @file qnnodeicon.h
 ** @author Angelo Prudentino
 ** @date 2016-11-15
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

#ifndef QNNODEGRAPHICEXT_H
#define QNNODEGRAPHICEXT_H

#include "qntypes.h"
#include "qnnode.h"
#include "qnicon.h"
#include "qnglobalexport.h"
#include "utility.h"


/**
 * @brief The QNNodeIcon class models the graphic extensions associated to a node
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNodeIcon: public QNIcon {
private:
    Q_DISABLE_COPY(QNNodeIcon)

    /**
     * @brief pointer to the corresponding node
     */
    QNNodePtr data;

public:
    /**
     * @brief QNNodeIcon default constructor
     * @param aContainer teh reference to the container model
     * @warning passing null pointer as parameter to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNNodeIcon(QNModelPtr aContainer);

    /**
     * @brief QNNodeIcon destructor
     */
    ~QNNodeIcon();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNNodeIcon& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNNodeIcon& aOther) const;

    /**
     * @brief initialize the node's id with a random unique value
     * @param aLen the lenght of the random string to be generated (default 20)
     * @return the error object resulting from this operation
     */
    QNError initId(const uint aLen = DEFAULT_ID_LEN);

    /**
     * @brief retrievs the pointer to the corresponding edge
     * @return the pointer to the corresponding edge
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodePtr getData() const;

    /**
     * @brief updates the pointer to the corresponding edge
     * @param value the pointer to the new edge
     * @note if value is null, the request will be ignored
     */
    void setData(const QNNodePtr& value);

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

#endif // QNNODEGRAPHICEXT_H
