/**
 ** @file qnnetworkmodel.h
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

#ifndef QNNETWORKMODEL_H
#define QNNETWORKMODEL_H

#include "qntypes.h"
#include "qnmodel.h"
#include "qnedgestyle.h"
#include "qnglobalexport.h"

/**
 * @brief The QNNetworkModel class models the graphic extension model associated to a network
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNetworkModel : public QNModel{
private:
    Q_DISABLE_COPY(QNNetworkModel)

    /**
     * @brief graphic extension's version
     */
    QString version;

    /**
     * @brief a live reference to the container network
     */
    QNNetworkPtr net;

    /**
     * @brief the map of ALL icons contained in this network
     */
    ScopedQNIconMap allIcons;

    /**
     * @brief the list of ALL textboxes belonging to this network
     */
    ScopedQNTextBoxList allTextboxes;

    /**
     * @brief map of edge-style objects
     */
    ScopedQNEdgeStyleMap edges;

    /**
     * @brief compare this netModel's icons with the given ones
     * @param aOther the object to be compared with
     * @return true if icons are the same, false otherwise
     */
    bool compareAllIcons(const QNNetworkModel& aOther) const;

    /**
     * @brief compare this netModel's edges with the given ones
     * @param aOther the object to be compared with
     * @return true if edges are the same, false otherwise
     */
    bool compareEdges(const QNNetworkModel& aOther) const;

    /**
     * @brief compare this netModel's textboxes with the given ones
     * @param aOther the object to be compared with
     * @return true if textboxes are the same, false otherwise
     */
    bool compareAllTextboxes(const QNNetworkModel& aOther) const;

public:
    /**
     * @brief QNNetworkModel default constructor
     * @param aNet the reference to the network instance
     * @warning passing null pointer as parameter to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNNetworkModel(QNNetworkPtr aNet);

    /**
     * @brief QNNetworkModel destructor
     */
    ~QNNetworkModel();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNNetworkModel& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNNetworkModel& aOther) const;

    /**
     * @brief retrieves the graphic extension's version
     * @return the graphic extension's version
     */
    QString getVersion() const;

    /**
     * @brief retrieves a live reference to this network instance
     * @return a live reference to this network instance
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNNetworkPtr getNet() const;

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
    QNEdgeStylePtr getEdge(const QString& aEdgeKey) const;

    /**
     * @brief updates the graphic extension's version
     * @param aValue the new graphic extension's version
     * @note if aValue is empty, the request will be ignored
     */
    void setVersion(const QString& aValue);

    /**
     * @brief adds the given edge to this network
     * @param aEdge the edge to be added
     * @note if aEdge is null, the request will be ignored
     */
    void addEdge(QNEdgeStylePtr aEdge);

    /**
     * @brief removes the given edge from the network
     * @param aEdge the edge to be removed
     * @note if aEdge is null, the request will be ignored
     * @param aDelete if true the given edge will be deleted after removal
     */
    void removeEdge(QNEdgeStylePtr aEdge, const bool aDelete = true);

    /**
     * @brief adds an icon to the icons map of all icons
     * @param aIcon the icon to be added
     * @note if aIcon is null, the request will be ignored
     */
    void addIconToAll(QNIconPtr aIcon);

    /**
     * @brief removes the given icon from the map of all icons
     * @param aIcon the icon to be removed
     * @return true if the icon has been removed and deleted
     * @note if aIcon is null, the request will be ignored
     */
    bool removeIconFromAll(QNIconPtr aIcon);

    /**
     * @brief adds a textbox to the list of all textboxes
     * @param aTextBox the textbox to be added
     * @note if aTextBox is null, the request will be ignored
     */
    void addTextBoxToAll(QNTextBoxPtr aTextBox);

    /**
     * @brief removes the given textbox from the list of all textboxes
     * @param aTextBox the textbox to be removed
     * @return true if the textbox has been removed and deleted
     * @note if aTextBox is null, the request will be ignored
     */
    bool removeTextBoxFromAll(QNTextBoxPtr aTextBox);

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
     * @brief retrieves the list of icons with the given properties
     * @param aProperties the list of properties to be searched
     * @param aStrategy the filtering strategy to be used
     * @return the list of matching icons
     * @note call this method without parameters to retrieve all icons
     * @note if some property has only the name, that will be tested for existence only
     * @warning do not delete the returned icons;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNIconList getIconsByProperties(const QNPropertyList& aProperties = QNPropertyList(), const QNFilterStrategy& aStrategy = QNFilterStrategy::AND);

    /**
     * @brief retrieves the list of textboxes with the given properties
     * @param aProperties the list of properties to be searched
     * @param aStrategy the filtering strategy to be used
     * @return the list of matching textboxes
     * @note call this method without parameters to retrieve all textboxes
     * @note if some property has only the name, that will be tested for existence only
     * @warning do not delete the returned textboxes;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNTextBoxList getTextboxesByProperties(const QNPropertyList& aProperties = QNPropertyList(), const QNFilterStrategy& aStrategy = QNFilterStrategy::AND);

    /**
     * @brief retrieves the list of edges with the given properties
     * @param aProperties the list of properties to be searched
     * @param aStrategy the filtering strategy to be used
     * @return the list of matching edges
     * @note call this method without parameters to retrieve all edges
     * @note if some property has only the name, that will be tested for existence only
     * @warning do not delete the returned edges;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNEdgeStyleList getEdgesByProperties(const QNPropertyList& aProperties = QNPropertyList(), const QNFilterStrategy& aStrategy = QNFilterStrategy::AND);

    /**
     * @brief retrieves the list of stylable elements with the given classes
     * @param aClasses the list of classes to be searched
     * @param aStrategy the filtering strategy to be used
     * @return the list of matching stylable elements
     * @note call this method without parameters to retrieve all stylable elements
     * @warning do not delete the returned edges;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNStylableList getStylablesByClass(const QStringList& aClasses = QStringList(), const QNFilterStrategy& aStrategy = QNFilterStrategy::AND);
};

#endif // QNNETWORKMODEL_H
