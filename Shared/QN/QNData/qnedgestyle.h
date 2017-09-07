/**
 ** @file qnedgestyle.h
 ** @author Angelo Prudentino
 ** @date 2016-11-20
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

#ifndef QNEDGESTYLE_H
#define QNEDGESTYLE_H

#include "qntypes.h"
#include "qnedge.h"
#include "qnproperties.h"
#include "qnstylable.h"
#include "qnglobalexport.h"

/**
 * @brief The QNEdgeStyle class models the graphis properties that could be associated
 * to an edge object
 * @see QNEdge
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNEdgeStyle: public QNStylable, public QNProperties {
private:
    Q_DISABLE_COPY(QNEdgeStyle)

    /**
     * @brief edge's unique id
     * @note the id is made up of: source node id + " - " + destination node id
     */
    QString id;

    /**
     * @brief pointer to the corresponding edge
     */
    QNEdgePtr data;

    /**
     * @brief optional comment associated to the edge
     */
    QString comment;

public:
    /**
     * @brief QNEdgeStyle constructor
     * @param aData the pointer to the corresponding edge object
     * @warning passing null pointer as parameter to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNEdgeStyle(QNEdgePtr aData);

    /**
     * @brief QNEdgeStyle destructor
     */
    ~QNEdgeStyle();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNEdgeStyle& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNEdgeStyle& aOther) const;

    /**
     * @brief retrieves the edge's id
     * @return the edge's id
     */
    QString getId() const;

    /**
     * @brief retrieves the pointer to the corresponding edge object
     * @return the pointer to the corresponding edge object
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNEdgePtr getData() const;

    /**
     * @brief updates the pointer to the corresponding edge object
     * @param aData the pointer to the corresponding edge object
     * @note if aData is null, the request will be ignored
     */
    void setData(QNEdgePtr aData);

    /**
     * @brief retrieves the optional comment associated to the edge
     * @return the optional comment associated to the edge
     */
    QString getComment() const;

    /**
     * @brief updates the comment associated to this edge-style object
     * @param value the new comment
     */
    void setComment(const QString& value);

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

#endif // QNEDGESTYLE_H
