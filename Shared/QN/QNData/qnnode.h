/**
 ** @file qnnode.h
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

#ifndef QNNODE_H
#define QNNODE_H

#include "qnnetwork.h"
#include "qntypes.h"
#include "qnproperties.h"
#include "qnglobalexport.h"
#include "utility.h"
#include "qnerror.h"

/**
 * @brief The QNNode class models a node inside the network
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNode : public QNProperties{
private:
    Q_DISABLE_COPY(QNNode)

    /**
     * @brief node's id
     */
    QString id;

    /**
     * @brief a live reference to the container network
     */
    QNNetworkPtr net;

    /**
     * @brief the list of parents nodes (live references)
     */
    ScopedQNNodeList ancestors;

    /**
     * @brief the list of children nodes (live references)
     */
    ScopedQNNodeList successors;

    /**
     * @brief a live reference to the corresponding icon
     */
    QNNodeIconPtr icon;

    /**
     * @brief compares ancestor nodes
     * @param aOther the object to be compared with
     * @return true if ancestors are equal, false otherwise
     */
    bool compareAncestors(const QNNode& aOther) const;

    /**
     * @brief compares successor nodes
     * @param aOther the object to be compared with
     * @return true if ancestors are equal, false otherwise
     */
    bool compareSuccessors(const QNNode& aOther) const;

public:
    /**
     * @brief QNNode constructor
     * @param aNet a live reference to the container network
     * @warning passing null pointer as parameter to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNNode(QNNetworkPtr aNet);

    /**
     * @brief QNNode destructor
     */
    ~QNNode();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNNode& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNNode& aOther) const;

    /**
     * @brief retrieves a live reference to the container network
     * @return a live reference to the container network
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNetworkPtr getNet() const;

    /**
     * @brief retrieves the node's id
     * @return the node's id
     */
    QString getId() const;

    /**
     * @brief updates the id for this node
     * @param aValue the node's id
     * @note if aValue is empty, the request will be ignored
     * @return the error object resulting from this operation
     */
    QNError setId(const QString& aValue);

    /**
     * @brief initialize the node's id with a random unique value
     * @param aLen the lenght of the random string to be generated (default 20)
     * @return the error object resulting from this operation
     */
    QNError initId(const uint aLen = DEFAULT_ID_LEN);

    /**
     * @brief adds an ancestor node to the list
     * @param aAncestor an ancestor node
     * @note if aAncestor is null, the request will be ignored
     */
    void addAncestor(QNNodePtr aAncestor);

    /**
     * @brief removes the given node from the ancestors
     * @param aAncestor the node to be removed
     * @note if aAncestor is null, the request will be ignored
     */
    void removeAncestor(QNNodePtr aAncestor);

    /**
     * @brief returns a live reference to the list of ancestors of this node
     * @return a live reference to the list of ancestors of this node
     * @note if this node has no ancestors, null wil be returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeListPtr getAncestors() const;

    /**
     * @brief adds a successor node to the list
     * @param aSuccessor a successor node
     * @note if aSuccessor is null, the request will be ignored
     */
    void addSuccessor(QNNodePtr aSuccessor);

    /**
     * @brief removes the given node from the successors
     * @param aSuccessor the node to be removed
     * @note if aSuccessor is null, the request will be ignored
     */
    void removeSuccessor(QNNodePtr aSuccessor);

    /**
     * @brief returns a live reference to the list of successors of this node
     * @return a live reference to the list of successors of this node
     * @note if this node has no successors, null wil be returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeListPtr getSuccessors() const;

    /**
     * @brief retrieves the corresponding icon object
     * @return the corresponding icon object
     * @note if no icon is associated, null is returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNodeIconPtr getIcon() const;

    /**
     * @brief updates the icon associated
     * @param value the new icon object
     */
    void setIcon(QNNodeIconPtr value);

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

#endif // QNNODE_H
