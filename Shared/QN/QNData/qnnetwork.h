/**
 ** @file qnnetwork.h
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

#ifndef QNNETWORK_H
#define QNNETWORK_H

#include "qnnode.h"
#include "qnedge.h"
#include "qntypes.h"
#include "qnproperties.h"
#include "qnglobalexport.h"

/**
 * @brief The QNNetwork class models the entire network
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNetwork: public QNProperties{
private:
    Q_DISABLE_COPY(QNNetwork)

    /**
     * @brief network's id
     */
    QString id;

    /**
     * @brief network's version
     */
    QString version;

    /**
     * @brief network's nodes
     */
    ScopedQNNodeMap nodes;

    /**
     * @brief network's edges
     */
    ScopedQNEdgeMap edges;

    /**
     * @brief network's graphic extension model
     */
    ScopedQNNetworkModel netModel;

    /**
     * @brief compares nodes between the networks
     * @param aOther the object to be compared with
     * @return true if nodes are the same, false otherwise
     */
    bool compareNodes(const QNNetwork& aOther) const;

    /**
     * @brief compares nodes between the networks
     * @param aOther the object to be compared with
     * @return true if nodes are the same, false otherwise
     */
    bool compareEdges(const QNNetwork& aOther) const;

public:
    /**
     * @brief QNNetwork constructor
     * @param aId the id for this network
     * @note if aId is empty, it will be ignored
     * @param aVersion network's version
     * @note if aVersion is empty, it will be ignored
     */
    QNNetwork(const QString& aId, const QString& aVersion);

    /**
     * @brief QNNetwork destructor
     */
    ~QNNetwork();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNNetwork& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNNetwork& aOther) const;

    /**
     * @brief retrieves the network's id
     * @return the id of this network
     */
    QString getId() const;

    /**
     * @brief updates the network's id
     * @param aId the new ID
     * @note if aId is empty, the request will be ignored
     */
    void setId(const QString& aId);

    /**
     * @brief retrieves the network's version
     * @return the version of this network
     */
    QString getVersion() const;

    /**
     * @brief updates the network's version
     * @param aVersion the new version
     * @note if aVersion is empty, the request will be ignored
     */
    void setVersion(const QString& aVersion);

    /**
     * @brief returns the list of all nodes' keys
     * @return the list of all nodes' keys
     */
    QStringList getAllNodesKeys() const;

    /**
     * @brief retrieves the node given its unique id
     * @param aNodeId the id of the desired node
     * @return the node reference
     * @note if the node does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodePtr getNode(const QString& aNodeId) const;

    /**
     * @brief adds a node to this network
     * @param aNode the node to be added
     * @note if aNode is null, the request will be ignored
     */
    void addNode(QNNodePtr aNode);

    /**
     * @brief removes a node from this network
     * @param aNode the node to be removed and deleted
     * @note if aNode is null, the request will be ignored
     * @param aDelete if true the given edge will be deleted after removal
     */
    void removeNode(QNNodePtr aNode, const bool aDelete = true);

    /**
     * @brief returns the list of all edges' keys
     * @return the list of all edges' keys
     */
    QStringList getAllEdgesKeys() const;

    /**
     * @brief retrieves the edge given its key
     * @param aEdgeKey the key to seedge for
     * @return the edge object corresponding to the given key
     * @note the key is made up of: source node id + " - " + destination node id
     * @note if the edge does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNEdgePtr getEdge(const QString& aEdgeKey) const;

    /**
     * @brief adds the given edge to this network
     * @param aEdge the edge to be added
     * @note if aEdge is null, the request will be ignored
     */
    void addEdge(QNEdgePtr aEdge);

    /**
     * @brief removes the given edge from the network
     * @param aEdge the edge to be removed and deleted
     * @note if aEdge is null, the request will be ignored
     * @param aDelete if true the given edge will be deleted after removal
     */
    void removeEdge(QNEdgePtr aEdge, const bool aDelete = true);

    /**
     * @brief retrieves the network's graphic extension model
     * @return the network's graphic extension model
     * @note if the network's graphic extension model does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNetworkModelPtr getNetModel() const;

    /**
     * @brief updates the network's graphic extension model
     * @param value the new network's graphic extension model
     */
    void setNetModel(QNNetworkModelPtr value);

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

    /**
     * @brief retrieves the list of nodes with the given properties
     * @param aProperties the list of properties to be seedgeed
     * @param aStrategy the filtering strategy to be used
     * @return the list of matching nodes
     * @note call this method without parameters to retrieve all nodes
     * @note if some property has only the name, that will be tested for existence only
     * @warning do not delete the returned nodes;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeList getNodesByProperties(const QNPropertyList& aProperties = QNPropertyList(), const QNFilterStrategy& aStrategy = QNFilterStrategy::AND);

    /**
     * @brief retrieves the list of edges with the given properties
     * @param aProperties the list of properties to be seedgeed
     * @param aStrategy the filtering strategy to be used
     * @return the list of matching edges
     * @note call this method without parameters to retrieve all edges
     * @note if some property has only the name, that will be tested for existence only
     * @warning do not delete the returned edges;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNEdgeList getEdgesByProperties(const QNPropertyList& aProperties = QNPropertyList(), const QNFilterStrategy& aStrategy = QNFilterStrategy::AND);
};

#endif // QNNETWORK_H
