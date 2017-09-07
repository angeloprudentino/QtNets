/**
 ** @file qnedge.h
 ** @author Angelo Prudentino
 ** @date 2016-11-06
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

#ifndef QNEDGE_H
#define QNEDGE_H

#include "qnnetwork.h"
#include "qnnode.h"
#include "qntypes.h"
#include "qnproperties.h"
#include "qnglobalexport.h"

/**
 * @brief The QNEdge class models the connection between two nodes in the network
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNEdge: public QNProperties {
private:
    Q_DISABLE_COPY(QNEdge)

    /**
     * @brief edge's id
     */
    QString id;

    /**
     * @brief a live reference to to the source node
     */
    QNNodePtr src;

    /**
     * @brief a live reference to the destination node
     */
    QNNodePtr dst;

    /**
     * @brief a live reference to the container network
     */
    QNNetworkPtr net;

    /**
     * @brief a live reference to the corresponding icon
     */
    QNEdgeStylePtr icon;

    /**
     * @brief updates edge's ID
     */
    void updateID();

public:

    /**
     * @brief QNEdge constructor
     * @param aId the id for the edge
     * @param aNet a live container network reference
     * @note aId must be: source node id + " - " + destination node id
     * @note use this constructor if you need to set the edge's id before
     * the corresponding nodes are actually available
     * @warning passing a null network to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNEdge(const QString& aId, QNNetworkPtr aNet);

    /**
     * @brief QNEdge constructor
     * @param aSrc a live reference to the source node
     * @param aDst a live reference to the destination node
     * @param aNet a live container network reference
     * @warning passing null pointers as parameters to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNEdge(QNNodePtr aSrc, QNNodePtr aDst, QNNetworkPtr aNet);

    /**
     * @brief QNEdge destructor
     */
    ~QNEdge();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNEdge& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNEdge& aOther) const;

    /**
     * @brief retrieves this edge's id
     * @return the id for this edge
     */
    QString getId() const;

    /**
     * @brief retrieves a live reference to the source of this edge
     * @return a live reference to the source of this edge
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodePtr getSrc() const;

    /**
     * @brief updates source node of this edge
     * @param value the new source node
     * @note if value is null, the request will be ignored
     */
    void setSrc(const QNNodePtr& value);

    /**
     * @brief retrieves a live reference to the destination of this edge
     * @return a live reference to the destination of this edge
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodePtr getDst() const;

    /**
     * @brief updates destination node of this edge
     * @param value the new destination node
     * @note if value is null, the request will be ignored
     */
    void setDst(const QNNodePtr& value);

    /**
     * @brief retrieves the corresponding icon object
     * @return the corresponding icon object
     * @note if no icon is associated, null is returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNEdgeStylePtr getIcon() const;

    /**
     * @brief updates the icon associated
     * @param value the new icon object
     */
    void setIcon(QNEdgeStylePtr value);

    /**
     * @brief retrieves the network reference
     * @return the container network reference
     * @note if no container is associated, null is returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNetworkPtr getNet() const;

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

#endif // QNEDGE_H
