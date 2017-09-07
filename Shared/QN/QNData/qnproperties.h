/**
 ** @file qnproperties.h
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

#ifndef QNPROPERTIES_H
#define QNPROPERTIES_H

#include "qntypes.h"
#include "qnerror.h"
#include "qnglobalexport.h"

/**
 * @brief The QNProperties class models a set of objects to which
 * generic properties can be associated
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNProperties {
private:
    Q_DISABLE_COPY(QNProperties)

    /**
     * @brief object's properties
     */
    ScopedQNVariantMap properties;

public:
    /**
     * @brief QNProperties constructor
     */
    QNProperties();

    /**
     * @brief QNProperties destructor
     */
    virtual ~QNProperties();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNProperties& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNProperties& aOther) const;

    /**
     * @brief returns the list of all properties' keys
     * @return the list of all properties' keys
     */
    virtual QStringList getAllKeys() const;

    /**
     * @brief retrieves properties by name
     * @param aKey the name of the required property
     * @return the required property
     * @note if the property is not found, the returned variant object is invalid
     * @see QVariant#isValid()
     */
    virtual QVariant getProperty(const QString& aKey) const;

    /**
     * @brief stores the provided property using the provided key
     * @param aKey the property's key
     * @param aValue the property's value
     * @note if aKey is empty or aValue is invalid, the request will be ignored
     * @result the error object resulting from this operation
     */
    virtual QNError setProperty(const QString& aKey, const QVariant& aValue);
};

#endif // QNPROPERTIES_H
